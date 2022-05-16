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

#include "sequencergui/monitor/jobcontext.h"

#include "Instruction.h"
#include "Procedure.h"
#include "sequencergui/model/domainobjectbuilder.h"
#include "sequencergui/model/guiobjectbuilder.h"
#include "sequencergui/model/jobitem.h"
#include "sequencergui/model/jobmodel.h"
#include "sequencergui/model/procedureitem.h"
#include "sequencergui/model/standardinstructionitems.h"
#include "sequencergui/model/standardvariableitems.h"
#include "sequencergui/monitor/joblog.h"
#include "sequencergui/monitor/jobutils.h"
#include "sequencergui/monitor/procedurerunner.h"

#include "mvvm/standarditems/containeritem.h"

#include <QDebug>
#include <iostream>

namespace
{
std::string GetStatus(const instruction_t *instruction)
{
  return ::sup::sequencer::StatusToString(instruction->GetStatus());
}
}  // namespace

namespace sequencergui
{

JobContext::JobContext(JobItem *job_item, QObject *parent)
    : QObject(parent)
    , m_guiobject_builder(std::make_unique<GUIObjectBuilder>())
    , m_procedure_runner(std::make_unique<ProcedureRunner>())
    , m_job_log(new JobLog)
    , m_job_item(job_item)
{
  SetupConnections();
}

void JobContext::onPrepareJobRequest()
{
  // building domain procedure
  DomainObjectBuilder builder;
  m_domain_procedure = builder.CreateProcedure(m_job_item->GetProcedure());
  m_domain_procedure->Setup();  // to perform all necessary internal clones

  // FIXME, refactor, provide unit tests (that JobModel notifies views correctly)
  auto job_model =dynamic_cast<JobModel*>(m_job_item->GetModel());
  auto expanded_procedure = std::make_unique<ProcedureItem>();
  m_guiobject_builder->PopulateProcedureItem(m_domain_procedure.get(), expanded_procedure.get());
  job_model->InsertItem(std::move(expanded_procedure), m_job_item, mvvm::TagIndex::Append());
}

JobContext::~JobContext() = default;

void JobContext::onStartRequest()
{
  if (!IsValid())
  {
    qWarning() << "JobContext is in an invalid state";
    return;
  }

  if (m_procedure_runner->IsBusy())
  {
    return;
  }

  m_job_log->ClearLog();

  m_procedure_runner->ExecuteProcedure(m_domain_procedure.get(), false);  // do not setup
}

void JobContext::onPauseRequest()
{
  m_procedure_runner->SetWaitingMode(WaitingMode::kWaitForRelease);
}

void JobContext::onMakeStepRequest()
{
  m_procedure_runner->onMakeStepRequest();
}

void JobContext::onStopRequest()
{
  if (m_procedure_runner->GetRunnerStatus() == RunnerStatus::kRunning)
  {
    m_procedure_runner->Terminate();
  }
}

bool JobContext::WaitForCompletion(double timeout_sec)
{
  return m_procedure_runner->WaitForCompletion(timeout_sec);
}

bool JobContext::IsRunning() const
{
  return m_procedure_runner->IsBusy();
}

//! Sets message panel to report text information.

void JobContext::SetMessagePanel(MessagePanel *panel)
{
  m_job_log->SetMessagePanel(panel);
}

void JobContext::SetWaitingMode(WaitingMode waiting_mode)
{
  m_procedure_runner->SetWaitingMode(waiting_mode);
}

void JobContext::SetSleepTime(int time_msec)
{
  m_procedure_runner->SetSleepTime(time_msec);
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

void JobContext::onInstructionStatusChange(const instruction_t *instruction)
{
  auto instruction_item = m_guiobject_builder->FindInstructionItem(instruction);
  if (instruction_item)
  {
    instruction_item->SetStatus(GetStatus(instruction));
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
  // FIXME consider setting status of expanded procedure
  auto status = m_procedure_runner->GetRunnerStatus();
  m_job_item->SetStatus(RunnerStatusToString(status));
}

void JobContext::SetupConnections()
{
  connect(m_procedure_runner.get(), &ProcedureRunner::InstructionStatusChanged, this,
          &JobContext::onInstructionStatusChange, Qt::QueuedConnection);

  connect(m_procedure_runner.get(), &ProcedureRunner::LogMessageRequest, this,
          &JobContext::onLogMessage, Qt::QueuedConnection);

  connect(m_procedure_runner.get(), &ProcedureRunner::VariableChanged, this,
          &JobContext::onVariableChange, Qt::QueuedConnection);

  connect(m_procedure_runner.get(), &ProcedureRunner::RunnerStatusChanged, this,
          &JobContext::onRunnerStatusChanged, Qt::QueuedConnection);
}

}  // namespace sequencergui
