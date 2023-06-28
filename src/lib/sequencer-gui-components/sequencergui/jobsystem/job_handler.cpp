/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/domain_runner_adapter_new.h>
#include <sequencergui/jobsystem/job_utils.h>
#include <sequencergui/jobsystem/procedure_reporter.h>
#include <sequencergui/jobsystem/sequencer_observer.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/monitor/job_log.h>
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

  SetupWorkspaceSynchronizer();

  SetupProcedureReporter();

  SetupDomainRunnerAdapter();
}

JobHandler::~JobHandler() = default;

void JobHandler::onStartRequest()
{
  if (m_domain_runner_adapter->Start())
  {
    m_job_log->ClearLog();
  }
}

void JobHandler::onPauseRequest()
{
  m_domain_runner_adapter->Pause();
}

void JobHandler::onMakeStepRequest()
{
  m_domain_runner_adapter->Step();
}

void JobHandler::onStopRequest()
{
  if (m_domain_runner_adapter->Stop())
  {
    m_job_log->Append(CreateLogEvent(Severity::kWarning, "Stop request"));
  }
}

bool JobHandler::IsRunning() const
{
  return m_domain_runner_adapter ? m_domain_runner_adapter->IsBusy() : false;
}

void JobHandler::SetSleepTime(int time_msec)
{
  m_domain_runner_adapter->SetTickTimeout(time_msec);
}

void JobHandler::SetUserContext(const UserContext &user_context)
{
  m_procedure_reporter->SetUserContext(user_context);
}

ProcedureItem *JobHandler::GetExpandedProcedure() const
{
  return m_job_item->GetExpandedProcedure();
}

//! Returns true if this context is in valid state
bool JobHandler::IsValid() const
{
  return m_domain_procedure != nullptr;
}

RunnerStatus JobHandler::GetRunnerStatus() const
{
  return m_domain_runner_adapter ? m_domain_runner_adapter->GetStatus() : RunnerStatus::kIdle;
}

JobLog *JobHandler::GetJobLog() const
{
  return m_job_log;
}

void JobHandler::onInstructionStatusChange(const instruction_t *instruction, const QString &status)
{
  auto instruction_item = m_guiobject_builder->FindInstructionItem(instruction);
  if (instruction_item)
  {
    instruction_item->SetStatus(status.toStdString());
    emit InstructionStatusChanged(instruction_item);
  }
  else
  {
    std::cout << "Error in JobManager: can't find InstructionItem" << std::endl;
  }
}

void JobHandler::onLogEvent(const sequencergui::LogEvent &event)
{
  m_job_log->Append(event);
}

void JobHandler::onRunnerStatusChanged()
{
  auto status = m_domain_runner_adapter->GetStatus();
  m_job_item->SetStatus(RunnerStatusToString(status));
}

JobModel *JobHandler::GetJobModel()
{
  return dynamic_cast<JobModel *>(m_job_item->GetModel());
}

void JobHandler::PrepareForRun()
{
  m_domain_procedure.reset();

  if (auto expanded_procedure = m_job_item->GetExpandedProcedure(); expanded_procedure)
  {
    GetJobModel()->RemoveItem(expanded_procedure);
  }

  m_workspace_synchronizer.reset();
  m_procedure_reporter.reset();
  m_domain_runner_adapter.reset();
}

//! Setup domain procedure.

void JobHandler::SetupDomainProcedure()
{
  // building domain procedure
  m_domain_procedure = DomainProcedureBuilder::CreateProcedure(m_job_item->GetProcedure());

  // we are setting up root instruction, but deliberately not setting up the Workspace
  if (m_domain_procedure->RootInstruction())
  {
    m_domain_procedure->RootInstruction()->Setup(*m_domain_procedure);
  }
  else
  {
    if (m_domain_procedure->GetInstructionCount() > 1)
    {
      throw RuntimeException(
          "None of existing top-level instructions is marked as root instruction");
    }
    // we actually allow it to pass the setup, if procedure has only Workspace
  }
}

//! Setup expanded procedure item. It will reflect the content of domain procedure after its Setup.

void JobHandler::SetupExpandedProcedureItem()
{
  auto expanded_procedure = std::make_unique<ProcedureItem>();
  m_guiobject_builder->PopulateProcedureItem(m_domain_procedure.get(), expanded_procedure.get(),
                                             /*root_only*/ true);
  GetJobModel()->InsertItem(std::move(expanded_procedure), m_job_item, mvvm::TagIndex::Append());
}

//! Setup synchronization of workspace variables.

void JobHandler::SetupWorkspaceSynchronizer()
{
  auto workspace_item = m_job_item->GetExpandedProcedure()->GetWorkspace();
  if (workspace_item->GetVariableCount() > 0)
  {
    auto workspace = const_cast<sup::sequencer::Workspace *>(m_domain_procedure->GetWorkspace());
    m_workspace_synchronizer = std::make_unique<WorkspaceSynchronizer>(workspace_item, workspace);
    m_workspace_synchronizer->Start();  // will setup domain Workspace too
  }
}

//! Setup procedure reporter.

void JobHandler::SetupProcedureReporter()
{
  m_procedure_reporter = std::make_unique<ProcedureReporter>();

  connect(m_procedure_reporter.get(), &ProcedureReporter::InstructionStatusChanged, this,
          &JobHandler::onInstructionStatusChange, Qt::QueuedConnection);

  connect(m_procedure_reporter.get(), &ProcedureReporter::LogEventReceived, this,
          &JobHandler::onLogEvent, Qt::QueuedConnection);

  connect(m_procedure_reporter.get(), &ProcedureReporter::RunnerStatusChanged, this,
          &JobHandler::onRunnerStatusChanged, Qt::QueuedConnection);
}

//! Setup adapter to run procedures.

void JobHandler::SetupDomainRunnerAdapter()
{
  auto status_changed = [this](auto) { emit m_procedure_reporter->RunnerStatusChanged(); };

  m_domain_runner_adapter = std::make_unique<DomainRunnerAdapterNew>(
      m_domain_procedure.get(), m_procedure_reporter->GetObserver(), status_changed);
}

}  // namespace sequencergui
