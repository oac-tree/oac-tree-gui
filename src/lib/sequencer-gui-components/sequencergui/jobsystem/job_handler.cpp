/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "job_handler.h"

#include "domain_event_dispatcher_context.h"
#include "domain_runner_service.h"
#include "job_log.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/operation/breakpoint_controller.h>
#include <sequencergui/operation/breakpoint_helper.h>
#include <sequencergui/pvmonitor/workspace_item_listener.h>
#include <sequencergui/pvmonitor/workspace_monitor_helper.h>
#include <sequencergui/transform/domain_procedure_builder.h>
#include <sequencergui/transform/gui_object_builder.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/iterate_helper.h>

#include <mvvm/signals/item_listener.h>

#include <sup/sequencer/procedure.h>
#include <sup/sequencer/workspace.h>

#include <QDebug>

namespace sequencergui
{

JobHandler::JobHandler(JobItem *job_item, const UserContext &user_context)
    : m_guiobject_builder(std::make_unique<GUIObjectBuilder>())
    , m_job_log(new JobLog)
    , m_job_item(job_item)
{
  if (!job_item)
  {
    throw RuntimeException("JobItem is not initialised");
  }

  if (!m_job_item->GetProcedure())
  {
    throw RuntimeException("Procedure doesn't exist");
  }

  m_property_listener = std::make_unique<mvvm::ItemListener>(job_item);
  auto on_event = [this](const mvvm::PropertyChangedEvent &event)
  {
    if (event.name == itemconstants::kTickTimeout)
    {
      m_domain_runner_service->SetTickTimeout(m_job_item->GetTickTimeout());
    }
  };
  m_property_listener->Connect<mvvm::PropertyChangedEvent>(on_event);

  SetupBreakpointController();

  CreateDomainProcedure();

  SetupDomainRunner(user_context, m_job_item->GetTickTimeout());  // calls also Procedure::Setup

  SetupExpandedProcedureItem();
}

JobHandler::~JobHandler() = default;

void JobHandler::OnStartRequest()
{
  m_domain_runner_service->Start();
}

void JobHandler::OnPauseRequest()
{
  m_domain_runner_service->Pause();
}

void JobHandler::OnMakeStepRequest()
{
  m_domain_runner_service->Step();
}

void JobHandler::OnStopRequest()
{
  m_domain_runner_service->Stop();
}

void JobHandler::Reset()
{
  m_domain_runner_service->Reset();
}

bool JobHandler::IsRunning() const
{
  return m_domain_runner_service->IsBusy();
}

ProcedureItem *JobHandler::GetExpandedProcedure() const
{
  return m_job_item->GetExpandedProcedure();
}

RunnerStatus JobHandler::GetRunnerStatus() const
{
  return m_domain_runner_service ? static_cast<RunnerStatus>(m_domain_runner_service->GetJobState())
                                 : RunnerStatus::kInitial;
}

JobLog *JobHandler::GetJobLog() const
{
  return m_job_log;
}

// FIXME implement unit test
void JobHandler::OnToggleBreakpointRequest(InstructionItem *instruction)
{
  if (IsRunning())
  {
    return;
  }
  ToggleBreakpointStatus(*instruction);

  // update domain breakpoint
  size_t instruction_index = m_guiobject_builder->FindInstructionItemIndex(instruction);
  SetDomainBreakpoint(instruction_index, GetBreakpointStatus(*instruction));
}

void JobHandler::OnInstructionStatusChanged(const InstructionStatusChangedEvent &event)
{
  if (auto *item = m_guiobject_builder->FindInstructionItem(event.instruction); item)
  {
    item->SetStatus(::sup::sequencer::StatusToString(event.status));
    emit InstructionStatusChanged(item);
  }
  else
  {
    qWarning() << "Error in ProcedureReporter: can't find domain instruction counterpart";
  }
}

void JobHandler::OnJobStateChanged(const JobStateChangedEvent &event)
{
  m_job_item->SetStatus(::sup::sequencer::ToString(event.status));
}

void JobHandler::onLogEvent(const sequencergui::LogEvent &event)
{
  m_job_log->Append(event);
}

void JobHandler::OnNextLeavesChangedEvent(const NextLeavesChangedEvent &event)
{
  std::vector<InstructionItem *> items;
  for (const auto *instruction : event.leaves)
  {
    if (auto *item = m_guiobject_builder->FindInstructionItem(instruction); item)
    {
      items.push_back(item);
    }
    else
    {
      qWarning() << "Error in ProcedureReporter: can't find domain instruction counterpart";
    }
  }

  emit NextLeavesChanged(items);
}

void JobHandler::OnWorkspaceEvent(const WorkspaceEvent &event)
{
  m_workspace_item_listener->ProcessEventFromDomain(event);
}

JobModel *JobHandler::GetJobModel()
{
  return dynamic_cast<JobModel *>(m_job_item->GetModel());
}

void JobHandler::SetupBreakpointController()
{
  auto find_instruction = [this](const InstructionItem &item)
  { return m_guiobject_builder->FindInstruction(&item); };
  m_breakpoint_controller = std::make_unique<BreakpointController>(find_instruction);

  if (auto expanded_procedure = GetExpandedProcedure(); expanded_procedure)
  {
    m_breakpoint_controller->SaveBreakpoints(*expanded_procedure);
  }
}

void JobHandler::CreateDomainProcedure()
{
  m_domain_procedure = DomainProcedureBuilder::CreateProcedure(*m_job_item->GetProcedure());
}

void JobHandler::SetupExpandedProcedureItem()
{
  // We expect that Procedure::Setup was already called

  // remove previous expanded procedure
  if (auto expanded_procedure = GetExpandedProcedure(); expanded_procedure)
  {
    GetJobModel()->RemoveItem(expanded_procedure);
  }

  auto expanded_procedure = std::make_unique<ProcedureItem>();
  auto expanded_procedure_ptr = expanded_procedure.get();
  m_guiobject_builder->PopulateProcedureItem(m_domain_procedure.get(), expanded_procedure.get(),
                                             /*root_only*/ true);

  GetJobModel()->InsertItem(std::move(expanded_procedure), m_job_item, mvvm::TagIndex::Append());
  m_breakpoint_controller->RestoreBreakpoints(*expanded_procedure_ptr);

  // propagate all breakpoints to the domain
  // FIXME extract to function
  auto func = [this](const InstructionItem *item)
  {
    auto index = m_guiobject_builder->FindInstructionItemIndex(item);
    SetDomainBreakpoint(index, GetBreakpointStatus(*item));
  };
  IterateInstructionContainer<const InstructionItem *>(
      expanded_procedure_ptr->GetInstructionContainer()->GetInstructions(), func);


  m_workspace_item_listener = std::make_unique<WorkspaceItemListener>(
      expanded_procedure_ptr->GetWorkspace(), &m_domain_procedure->GetWorkspace());
}

void JobHandler::SetupDomainRunner(const UserContext &user_context, int sleep_time_msec)
{
  // this creates beneath DomainRunner(AsyncRunner(Runner())) and then call procedure->Setup
  m_domain_runner_service =
      std::make_unique<DomainRunnerService>(CreateContext(), user_context, *m_domain_procedure);
  m_domain_runner_service->SetTickTimeout(sleep_time_msec);
}

void JobHandler::SetDomainBreakpoint(size_t index, BreakpointStatus breakpoint_status)
{
  if (breakpoint_status == BreakpointStatus::kSet)
  {
    m_domain_runner_service->SetBreakpoint(index);
  }
  else
  {
    // We do not use "disabled" breakpoints in the domain, InstructionItem's breakpoint marked as
    // disabled, will remove breakpoint from the domain
    m_domain_runner_service->RemoveBreakpoint(index);
  }
}

DomainEventDispatcherContext JobHandler::CreateContext()
{
  DomainEventDispatcherContext result;

  result.process_instruction_status_changed = [this](const InstructionStatusChangedEvent &event)
  { OnInstructionStatusChanged(event); };

  result.process_workspace_event = [this](const WorkspaceEvent &event) { OnWorkspaceEvent(event); };

  result.process_job_state_changed = [this](const JobStateChangedEvent &event)
  { OnJobStateChanged(event); };

  result.process_log_event = [this](const LogEvent &event) { onLogEvent(event); };

  result.next_leaves_changed_event = [this](const NextLeavesChangedEvent &event)
  { OnNextLeavesChangedEvent(event); };

  return result;
}

}  // namespace sequencergui
