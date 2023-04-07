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

#include "job_context.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/domain_runner_adapter.h>
#include <sequencergui/jobsystem/job_utils.h>
#include <sequencergui/jobsystem/procedure_runner.h>
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

#include <QDebug>
#include <iostream>

namespace sequencergui
{

JobContext::JobContext(JobItem *job_item)
    : m_guiobject_builder(std::make_unique<GUIObjectBuilder>())
    , m_job_log(new JobLog)
    , m_job_item(job_item)
{
}

void JobContext::onPrepareJobRequest()
{
  if (IsRunning())
  {
    throw RuntimeException("Attempt to reset already running job");
  }

  // FIXME, refactor, provide unit tests (that JobModel notifies views correctly)
  auto job_model = dynamic_cast<JobModel *>(m_job_item->GetModel());

  if (auto expanded_procedure = m_job_item->GetExpandedProcedure(); expanded_procedure)
  {
    job_model->RemoveItem(expanded_procedure);
  }

  // building domain procedure
  m_domain_procedure = DomainProcedureBuilder::CreateProcedure(m_job_item->GetProcedure());

  // to perform all necessary internal clones
  if (!m_domain_procedure->Setup())
  {
    throw InvalidOperationException("Can't setup procedure");
  }

  auto expanded_procedure = std::make_unique<ProcedureItem>();
  m_guiobject_builder->PopulateProcedureItem(m_domain_procedure.get(), expanded_procedure.get(),
                                             /*root_only*/ true);
  job_model->InsertItem(std::move(expanded_procedure), m_job_item, mvvm::TagIndex::Append());

  auto workspace_item = m_job_item->GetExpandedProcedure()->GetWorkspace();
  if (workspace_item->GetVariableCount() > 0)
  {
    auto workspace = const_cast<sup::sequencer::Workspace *>(m_domain_procedure->GetWorkspace());
    m_workspace_syncronizer = std::make_unique<WorkspaceSynchronizer>(workspace_item, workspace);
    m_workspace_syncronizer->Start();
  }

  m_procedure_runner = CreateProcedureRunner(m_domain_procedure.get());

  auto status_changed = [this](auto) { emit m_procedure_runner->RunnerStatusChanged(); };

  m_domain_runner_adapter = std::make_unique<DomainRunnerAdapter>(
      m_domain_procedure.get(), m_procedure_runner->GetObserver(), status_changed);
}

JobContext::~JobContext() = default;

void JobContext::onStartRequest()
{
  if (m_domain_runner_adapter->Start())
  {
    m_job_log->ClearLog();
  }
}

void JobContext::onPauseRequest()
{
  m_domain_runner_adapter->Pause();
}

void JobContext::onMakeStepRequest()
{
  m_domain_runner_adapter->Step();
}

void JobContext::onStopRequest()
{
  if (m_domain_runner_adapter->Stop())
  {
    m_job_log->Append(CreateLogEvent(Severity::kWarning, "Stop request"));
  }
}

bool JobContext::IsRunning() const
{
  return m_domain_runner_adapter ? m_domain_runner_adapter->IsBusy() : false;
}

void JobContext::SetSleepTime(int time_msec)
{
  m_domain_runner_adapter->SetTickTimeout(time_msec);
}

void JobContext::SetUserContext(const UserContext &user_context)
{
  m_procedure_runner->SetUserContext(user_context);
}

ProcedureItem *JobContext::GetExpandedProcedure() const
{
  return m_job_item->GetExpandedProcedure();
}

//! Returns true if this context is in valid state
bool JobContext::IsValid() const
{
  return m_domain_procedure != nullptr;
}

RunnerStatus JobContext::GetRunnerStatus() const
{
  return m_domain_runner_adapter ? m_domain_runner_adapter->GetStatus() : RunnerStatus::kIdle;
}

JobLog *JobContext::GetJobLog() const
{
  return m_job_log;
}

void JobContext::onInstructionStatusChange(const instruction_t *instruction, const QString &status)
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

void JobContext::onLogEvent(const sequencergui::LogEvent &event)
{
  m_job_log->Append(event);
}

void JobContext::onRunnerStatusChanged()
{
  auto status = m_domain_runner_adapter->GetStatus();
  m_job_item->SetStatus(RunnerStatusToString(status));
}

std::unique_ptr<ProcedureRunner> JobContext::CreateProcedureRunner(procedure_t *procedure)
{
  auto result = std::make_unique<ProcedureRunner>(procedure);

  connect(result.get(), &ProcedureRunner::InstructionStatusChanged, this,
          &JobContext::onInstructionStatusChange, Qt::QueuedConnection);

  connect(result.get(), &ProcedureRunner::LogEventReceived, this, &JobContext::onLogEvent,
          Qt::QueuedConnection);

  connect(result.get(), &ProcedureRunner::RunnerStatusChanged, this,
          &JobContext::onRunnerStatusChanged, Qt::QueuedConnection);

  return result;
}

}  // namespace sequencergui
