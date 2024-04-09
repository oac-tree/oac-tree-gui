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
#include <sequencergui/pvmonitor/workspace_synchronizer.h>
#include <sequencergui/transform/domain_procedure_builder.h>
#include <sequencergui/transform/gui_object_builder.h>

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

  m_property_listener = std::make_unique<mvvm::ItemListener<JobItem>>(job_item);
  auto on_event = [this](const mvvm::PropertyChangedEvent &event)
  {
    if (event.m_name == itemconstants::kTickTimeout)
    {
      m_domain_runner_service->SetTickTimeout(m_job_item->GetTickTimeout());
    }
  };
  m_property_listener->Connect<mvvm::PropertyChangedEvent>(on_event);

  SetupBreakpointController();

  SetupDomainProcedure();

  SetupExpandedProcedureItem();

  SetupDomainRunner(user_context, m_job_item->GetTickTimeout());
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

void JobHandler::OnToggleBreakpointRequest(InstructionItem *instruction)
{
  if (IsRunning())
  {
    return;
  }
  ToggleBreakpointStatus(*instruction);
  m_breakpoint_controller->UpdateDomainBreakpoint(*instruction,
                                                  *m_domain_runner_service->GetJobController());
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

void JobHandler::SetupDomainProcedure()
{
  // building domain procedure
  m_domain_procedure = DomainProcedureBuilder::CreateProcedure(*m_job_item->GetProcedure());

  if (!m_domain_procedure->GetWorkspace().GetVariables().empty())
  {
    m_workspace_synchronizer =
        std::make_unique<WorkspaceSynchronizer>(&m_domain_procedure->GetWorkspace());
  }

  m_domain_procedure->Setup();
}

void JobHandler::SetupExpandedProcedureItem()
{
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
  if (m_workspace_synchronizer)
  {
    m_workspace_synchronizer->SetWorkspaceItem(expanded_procedure_ptr->GetWorkspace());
    m_workspace_synchronizer->Start();
  }
}

void JobHandler::SetupDomainRunner(const UserContext &user_context, int sleep_time_msec)
{
  m_domain_runner_service =
      std::make_unique<DomainRunnerService>(CreateContext(), user_context, *m_domain_procedure);
  m_breakpoint_controller->PropagateBreakpointsToDomain(
      *GetExpandedProcedure(), *m_domain_runner_service->GetJobController());
  m_domain_runner_service->SetTickTimeout(sleep_time_msec);
}

DomainEventDispatcherContext JobHandler::CreateContext()
{
  auto on_instruction_status = [this](const InstructionStatusChangedEvent &event)
  { OnInstructionStatusChanged(event); };

  auto on_job_state = [this](const JobStateChangedEvent &event) { OnJobStateChanged(event); };

  auto on_log_event = [this](const LogEvent &event) { onLogEvent(event); };

  auto on_next_leaves = [this](const NextLeavesChangedEvent &event)
  { OnNextLeavesChangedEvent(event); };

  return {on_instruction_status, on_job_state, on_log_event, on_next_leaves};
}

}  // namespace sequencergui
