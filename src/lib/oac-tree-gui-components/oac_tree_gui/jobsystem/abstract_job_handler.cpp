/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "abstract_job_handler.h"

#include "abstract_domain_runner.h"
#include "domain_event_dispatcher_context.h"
#include "job_log.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/iterate_helper.h>
#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/operation/breakpoint_controller.h>
#include <oac_tree_gui/operation/breakpoint_helper.h>
#include <oac_tree_gui/pvmonitor/workspace_monitor_helper.h>
#include <oac_tree_gui/transform/domain_procedure_builder.h>
#include <oac_tree_gui/transform/procedure_item_job_info_builder.h>
#include <oac_tree_gui/transform/transform_from_domain.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/procedure.h>
#include <sup/oac-tree/workspace.h>

#include <QDebug>

namespace oac_tree_gui
{

AbstractJobHandler::AbstractJobHandler(JobItem *job_item)
    : m_procedure_item_builder(std::make_unique<ProcedureItemJobInfoBuilder>())
    , m_job_log(new JobLog(this))
    , m_job_item(job_item)
{
  if (!job_item)
  {
    throw RuntimeException("JobItem is not initialised");
  }
}

AbstractJobHandler::~AbstractJobHandler() = default;

void AbstractJobHandler::Start()
{
  if (!IsRunning())
  {
    m_job_log->ClearLog();
  }
  m_domain_runner->Start();
}

void AbstractJobHandler::Pause()
{
  m_domain_runner->Pause();
}

void AbstractJobHandler::Step()
{
  m_domain_runner->Step();
}

void AbstractJobHandler::Stop()
{
  m_domain_runner->Stop();
  m_breakpoint_controller->ResetCurrentActiveBreakpoint();
}

void AbstractJobHandler::Reset()
{
  m_domain_runner->Reset();
  m_breakpoint_controller->ResetCurrentActiveBreakpoint();
}

bool AbstractJobHandler::IsRunning() const
{
  return m_domain_runner->IsBusy();
}

RunnerStatus AbstractJobHandler::GetRunnerStatus() const
{
  return GetRunnerStatusFromDomain(m_domain_runner->GetJobState());
}

JobLog *AbstractJobHandler::GetJobLog() const
{
  return m_job_log;
}

void AbstractJobHandler::OnToggleBreakpointRequest(InstructionItem *instruction)
{
  if (IsRunning())
  {
    return;
  }
  ToggleBreakpointStatus(*instruction);

  // update domain breakpoint
  const std::size_t instruction_index = m_procedure_item_builder->GetIndex(instruction);
  SetDomainBreakpoint(instruction_index, GetBreakpointStatus(*instruction));
}

JobItem *AbstractJobHandler::GetJobItem()
{
  return m_job_item;
}

ProcedureItem *AbstractJobHandler::GetExpandedProcedure() const
{
  return m_job_item->GetExpandedProcedure();
}

AbstractDomainRunner *AbstractJobHandler::GetDomainRunner()
{
  return m_domain_runner.get();
}

DomainEventDispatcherContext AbstractJobHandler::CreateEventDispatcherContext()
{
  DomainEventDispatcherContext result;

  result.process_instruction_state_updated = [this](const InstructionStateUpdatedEvent &event)
  { OnInstructionStateUpdated(event); };

  result.process_variable_updated = [this](const VariableUpdatedEvent &event)
  { OnVariableUpdatedEvent(event); };

  result.process_job_state_changed = [this](const JobStateChangedEvent &event)
  { OnJobStateChanged(event); };

  result.process_log_event = [this](const LogEvent &event) { onLogEvent(event); };

  result.active_instruction_changed_event = [this](const ActiveInstructionChangedEvent &event)
  { OnActiveInstructionChangedEvent(event); };

  result.breakpoint_hit_updated = [this](const BreakpointHitEvent &event)
  { OnBreakpointHitEvent(event); };

  return result;
}

void AbstractJobHandler::Setup(std::unique_ptr<AbstractDomainRunner> runner)
{
  SetupBreakpointController();

  m_domain_runner = std::move(runner);

  SetupExpandedProcedureItem();
}

ProcedureItemJobInfoBuilder *AbstractJobHandler::GetItemBuilder()
{
  return m_procedure_item_builder.get();
}

void AbstractJobHandler::OnInstructionStateUpdated(const InstructionStateUpdatedEvent &event)
{
  if (auto *item = m_procedure_item_builder->GetInstruction(event.index); item)
  {
    item->SetStatus(::sup::oac_tree::StatusToString(event.state.m_execution_status));
    emit InstructionStatusChanged(item);
  }
  else
  {
    qWarning() << "Error in AbstractJobHandler: can't find domain instruction counterpart";
  }
}

void AbstractJobHandler::OnJobStateChanged(const JobStateChangedEvent &event)
{
  m_job_item->SetStatus(static_cast<RunnerStatus>(event.state));
}

void AbstractJobHandler::onLogEvent(const oac_tree_gui::LogEvent &event)
{
  m_job_log->Append(event);
}

void AbstractJobHandler::OnActiveInstructionChangedEvent(const ActiveInstructionChangedEvent &event)
{
  std::vector<InstructionItem *> items;
  for (auto instruction_index : event.instr_idx)
  {
    if (auto *item = m_procedure_item_builder->GetInstruction(instruction_index); item)
    {
      items.push_back(item);
    }
    else
    {
      qWarning() << "Error in AbstractJobHandler: can't find domain instruction counterpart";
    }
  }

  emit ActiveInstructionChanged(items);
}

void AbstractJobHandler::OnVariableUpdatedEvent(const VariableUpdatedEvent &event)
{
  (void)event;
  throw RuntimeException("AbstractJobHandler::OnVariableUpdatedEvent is not implemented");
}

void AbstractJobHandler::OnBreakpointHitEvent(const BreakpointHitEvent &event)
{
  if (!IsValidInstructionIndex(event.index))
  {
    m_breakpoint_controller->ResetCurrentActiveBreakpoint();
    return;
  }

  if (auto *item = m_procedure_item_builder->GetInstruction(event.index); item)
  {
    m_breakpoint_controller->SetAsActiveBreakpoint(item);
  }
  else
  {
    qWarning() << "Error in AbstractJobHandler: can't find domain instruction counterpart";
  }
}

void AbstractJobHandler::SetupBreakpointController()
{
  m_breakpoint_controller = std::make_unique<BreakpointController>();

  if (auto expanded_procedure = GetExpandedProcedure(); expanded_procedure)
  {
    m_breakpoint_controller->SaveBreakpoints(*expanded_procedure);
  }
}

void AbstractJobHandler::SetupExpandedProcedureItem()
{
  // remove previous expanded procedure
  if (auto expanded_procedure = GetExpandedProcedure(); expanded_procedure)
  {
    mvvm::utils::RemoveItem(*expanded_procedure);
  }

  auto expanded_procedure =
      m_procedure_item_builder->CreateProcedureItem(m_domain_runner->GetJobInfo());
  auto expanded_procedure_ptr = expanded_procedure.get();

  (void)m_job_item->InsertItem(std::move(expanded_procedure), mvvm::TagIndex::Append());
  m_breakpoint_controller->RestoreBreakpoints(*expanded_procedure_ptr);

  PropagateBreakpointsToDomain();
}

void AbstractJobHandler::SetDomainBreakpoint(std::size_t index, BreakpointStatus breakpoint_status)
{
  if (breakpoint_status == BreakpointStatus::kSet)
  {
    m_domain_runner->SetBreakpoint(index);
  }
  else
  {
    // We do not use "disabled" breakpoints in the domain, InstructionItem's breakpoint marked as
    // disabled, will remove breakpoint from the domain
    m_domain_runner->RemoveBreakpoint(index);
  }
}

void AbstractJobHandler::PropagateBreakpointsToDomain()
{
  // visit all insrtuction items and set breakpoint status to the domain
  auto func = [this](const InstructionItem *item)
  {
    auto index = m_procedure_item_builder->GetIndex(item);
    SetDomainBreakpoint(index, GetBreakpointStatus(*item));
  };
  IterateInstructionContainer<const InstructionItem *>(
      GetExpandedProcedure()->GetInstructionContainer()->GetInstructions(), func);
}

}  // namespace oac_tree_gui
