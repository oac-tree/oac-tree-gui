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

#include "domain_runner_adapter.h"
#include "job_log.h"
#include "job_utils.h"
#include "procedure_reporter.h"
#include "sequencer_observer.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/experimental/domain_event_dispatcher_context.h>
#include <sequencergui/jobsystem/domain_runner_service.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/operation/breakpoint_controller.h>
#include <sequencergui/operation/breakpoint_helper.h>
#include <sequencergui/pvmonitor/workspace_synchronizer.h>
#include <sequencergui/transform/domain_procedure_builder.h>
#include <sequencergui/transform/gui_object_builder.h>

#include <mvvm/standarditems/container_item.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/workspace.h>

#include <QDebug>
#include <iostream>

namespace sequencergui
{

JobHandler::JobHandler(JobItem *job_item)
    : m_guiobject_builder(std::make_unique<GUIObjectBuilder>())
    , m_job_log(new JobLog)
    , m_job_item(job_item)
{
  auto find_instruction = [this](const InstructionItem &item)
  { return m_guiobject_builder->FindInstruction(&item); };
  m_breakpoint_controller = std::make_unique<BreakpointController>(find_instruction);

  auto find_instruction_ietm = [this](const instruction_t &instruction)
  { return m_guiobject_builder->FindInstructionItem(&instruction); };
  m_procedure_reporter = std::make_unique<ProcedureReporter>(find_instruction_ietm);

  connect(m_procedure_reporter.get(), &ProcedureReporter::InstructionStatusChanged, this,
          &JobHandler::InstructionStatusChanged);

  connect(m_procedure_reporter.get(), &ProcedureReporter::NextLeavesChanged, this,
          &JobHandler::NextLeavesChanged);

  connect(m_procedure_reporter.get(), &ProcedureReporter::LogEventReceived, this,
          &JobHandler::onLogEvent, Qt::QueuedConnection);

  auto on_status_changed = [this](auto status)
  { m_job_item->SetStatus(RunnerStatusToString(status)); };
  connect(m_procedure_reporter.get(), &ProcedureReporter::RunnerStatusChanged, this,
          on_status_changed);
}

void JobHandler::onPrepareJobRequest()
{
  if (IsRunning())
  {
    throw RuntimeException("Attempt to reset already running job");
  }

  PrepareForRun();

  SetupDomainProcedure();

  SetupExpandedProcedureItem();

  // SetupDomainRunnerAdapter();

  SetupDomainRunner();
}

JobHandler::~JobHandler() = default;

void JobHandler::onStartRequest()
{
  ValidateJobHandler();
  // const bool was_paused = m_domain_runner_adapter->GetStatus() == RunnerStatus::kPaused;
  // if (m_domain_runner_adapter->Start() && !was_paused)
  // {
  //   m_job_log->ClearLog();
  // }
  m_domain_runner_service->Start();
}

void JobHandler::onPauseRequest()
{
  ValidateJobHandler();
  // m_domain_runner_adapter->Pause();
  m_domain_runner_service->Pause();
}

void JobHandler::onMakeStepRequest()
{
  ValidateJobHandler();
  // m_domain_runner_adapter->Step();
  m_domain_runner_service->Step();
}

void JobHandler::onStopRequest()
{
  ValidateJobHandler();
  // if (m_domain_runner_adapter->Stop())
  // {
  //   m_job_log->Append(CreateLogEvent(Severity::kWarning, "Stop request"));
  // }
  m_domain_runner_service->Stop();
}

bool JobHandler::IsRunning() const
{
  // std::cout << "AAA 1.1" << m_domain_runner_service.get() << std::endl;
  std::cout << "AAA 1.2"
            << (m_domain_runner_service
                    ? sup::sequencer::ToString(m_domain_runner_service->GetCurrentState())
                    : std::string("null"))
            << std::endl;
  // return m_domain_runner_adapter ? m_domain_runner_adapter->IsBusy() : false;
  return m_domain_runner_service ? m_domain_runner_service->IsBusy() : false;
}

void JobHandler::SetSleepTime(int time_msec)
{
  // m_domain_runner_adapter->SetTickTimeout(time_msec);
  m_domain_runner_service->SetTickTimeout(time_msec);
}

void JobHandler::SetUserContext(const UserContext &user_context)
{
  // m_procedure_reporter->SetUserContext(user_context);
  m_domain_runner_service->SetUserContext(user_context);
}

ProcedureItem *JobHandler::GetExpandedProcedure() const
{
  return m_job_item->GetExpandedProcedure();
}

//! Returns true if this context is in valid state
bool JobHandler::IsValid() const
{
  // return m_domain_procedure != nullptr && m_domain_runner_adapter;
  return m_domain_procedure != nullptr && m_domain_runner_service;
}

RunnerStatus JobHandler::GetRunnerStatus() const
{
  // return m_domain_runner_adapter ? m_domain_runner_adapter->GetStatus() : RunnerStatus::kInitial;
  return m_domain_runner_service ? static_cast<RunnerStatus>(m_domain_runner_service->GetCurrentState()) : RunnerStatus::kInitial;
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
                                                  *m_domain_runner_adapter->GetDomainRunner());
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

void JobHandler::ValidateJobHandler()
{
  if (!IsValid())
  {
    throw RuntimeException("Job wasn't properly initialised");
  }
}

JobModel *JobHandler::GetJobModel()
{
  return dynamic_cast<JobModel *>(m_job_item->GetModel());
}

void JobHandler::PrepareForRun()
{
  m_domain_procedure.reset();

  if (auto expanded_procedure = GetExpandedProcedure(); expanded_procedure)
  {
    m_breakpoint_controller->SaveBreakpoints(*expanded_procedure);
    GetJobModel()->RemoveItem(expanded_procedure);
  }

  m_workspace_synchronizer.reset();
  m_domain_runner_adapter.reset();
}

//! Setup domain procedure.

void JobHandler::SetupDomainProcedure()
{
  if (!m_job_item->GetProcedure())
  {
    throw RuntimeException("Procedure doesn't exist");
  }

  // building domain procedure
  m_domain_procedure = DomainProcedureBuilder::CreateProcedure(*m_job_item->GetProcedure());

  if (!m_domain_procedure->GetWorkspace().GetVariables().empty())
  {
    m_workspace_synchronizer =
        std::make_unique<WorkspaceSynchronizer>(&m_domain_procedure->GetWorkspace());
  }

  m_domain_procedure->Setup();
}

//! Setup expanded procedure item. It will reflect the content of domain procedure after its Setup.

void JobHandler::SetupExpandedProcedureItem()
{
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

//! Setup adapter to run procedures.

void JobHandler::SetupDomainRunnerAdapter()
{
  auto status_changed = [this](auto status)
  { m_procedure_reporter->OnDomainRunnerStatusChanged(status); };

  auto on_tick = [this](const auto &procedure)
  { m_procedure_reporter->OnDomainProcedureTick(procedure); };

  DomainRunnerAdapterContext context{m_domain_procedure.get(), m_procedure_reporter->GetObserver(),
                                     status_changed, on_tick};
  m_domain_runner_adapter = std::make_unique<DomainRunnerAdapter>(context);

  // setup breakpoint
  m_breakpoint_controller->PropagateBreakpointsToDomain(
      *GetExpandedProcedure(), *m_domain_runner_adapter->GetDomainRunner());
}

void JobHandler::SetupDomainRunner()
{
  m_domain_runner_service =
      std::make_unique<DomainRunnerService>(CreateContext(), *m_domain_procedure);
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
