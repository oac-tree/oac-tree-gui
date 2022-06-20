/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/jobsystem/job_context.h"

#include "Instruction.h"
#include "Procedure.h"
#include "sequencergui/core/exceptions.h"
#include "sequencergui/jobsystem/job_utils.h"
#include "sequencergui/jobsystem/procedure_runner.h"
#include "sequencergui/model/domain_object_builder.h"
#include "sequencergui/model/gui_object_builder.h"
#include "sequencergui/model/job_item.h"
#include "sequencergui/model/job_model.h"
#include "sequencergui/model/procedure_item.h"
#include "sequencergui/model/standard_instruction_items.h"
#include "sequencergui/model/standard_variable_items.h"
#include "sequencergui/monitor/job_log.h"

#include "mvvm/standarditems/container_item.h"

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
  DomainObjectBuilder builder;
  m_domain_procedure = builder.CreateProcedure(m_job_item->GetProcedure());

  // to perform all necessary internal clones
  if (!m_domain_procedure->Setup())
  {
    throw InvalidOperationException("Can't setup procedure");
  }

  auto expanded_procedure = std::make_unique<ProcedureItem>();
  m_guiobject_builder->PopulateProcedureItem(m_domain_procedure.get(), expanded_procedure.get());
  job_model->InsertItem(std::move(expanded_procedure), m_job_item, mvvm::TagIndex::Append());

  m_procedure_runner = CreateProcedureRunner(m_domain_procedure.get());
}

JobContext::~JobContext() = default;

void JobContext::onStartRequest()
{
  CheckRunner();
  if (m_procedure_runner->Start())
  {
    m_job_log->ClearLog();
  }
}

void JobContext::onPauseRequest()
{
  CheckRunner();
  m_procedure_runner->Pause();
}

void JobContext::onMakeStepRequest()
{
  CheckRunner();
  m_procedure_runner->Step();
}

void JobContext::onStopRequest()
{
  CheckRunner();
  m_procedure_runner->Stop();
}

bool JobContext::IsRunning() const
{
  return m_procedure_runner ? m_procedure_runner->IsBusy() : false;
}

//! Sets message panel to report text information.

void JobContext::SetMessagePanel(MessagePanel *panel)
{
  m_job_log->SetMessagePanel(panel);
}

void JobContext::SetSleepTime(int time_msec)
{
  CheckRunner();
  m_procedure_runner->SetSleepTime(time_msec);
}

void JobContext::SetUserContext(const UserContext &user_context)
{
  CheckRunner();
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
  CheckRunner();
  return m_procedure_runner->GetRunnerStatus();
}

void JobContext::onInstructionStatusChange(const instruction_t *instruction, const QString &status)
{
  auto instruction_item = m_guiobject_builder->FindInstructionItem(instruction);
  std::cout << "JobContext::onInstructionStatusChange() " << instruction << " "
            << status.toStdString() << std::endl;
  ;
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

void JobContext::onLogMessage(const QString &message, int message_type)
{
  m_job_log->Append(message.toStdString(), static_cast<JobMessageType>(message_type));
}

void JobContext::onVariableChange(const QString &variable_name, const QString &value)
{
  auto variable_item = m_guiobject_builder->FindVariableItem(variable_name.toStdString());
  if (variable_item)
  {
    if (auto local_var = dynamic_cast<LocalVariableItem *>(variable_item); local_var)
    {
      local_var->SetJsonValue(value.toStdString());
    }
    else if (auto local_var = dynamic_cast<ChannelAccessVariableItem *>(variable_item))
    {
      local_var->SetJsonValue(value.toStdString());
    }
    else if (auto local_var = dynamic_cast<PVClientVariableItem *>(variable_item))
    {
      local_var->SetJsonValue(value.toStdString());
    }
  }
  else
  {
    std::cout << "Error in JobManager: can't find VariableItem" << std::endl;
  }
}

void JobContext::onRunnerStatusChanged()
{
  auto status = m_procedure_runner->GetRunnerStatus();
  m_job_item->SetStatus(RunnerStatusToString(status));
}

void JobContext::CheckRunner() const
{
  if (!m_procedure_runner)
  {
    throw RuntimeException("No runner defined");
  }
}

std::unique_ptr<ProcedureRunner> JobContext::CreateProcedureRunner(procedure_t *procedure)
{
  auto result = std::make_unique<ProcedureRunner>(procedure);

  connect(result.get(), &ProcedureRunner::InstructionStatusChanged, this,
          &JobContext::onInstructionStatusChange, Qt::QueuedConnection);

  connect(result.get(), &ProcedureRunner::LogMessageRequest, this, &JobContext::onLogMessage,
          Qt::QueuedConnection);

  connect(result.get(), &ProcedureRunner::VariableChanged, this, &JobContext::onVariableChange,
          Qt::QueuedConnection);

  connect(result.get(), &ProcedureRunner::RunnerStatusChanged, this,
          &JobContext::onRunnerStatusChanged, Qt::QueuedConnection);

  return result;
}

}  // namespace sequencergui
