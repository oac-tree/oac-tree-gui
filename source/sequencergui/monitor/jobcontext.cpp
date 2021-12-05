/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
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
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/monitor/joblog.h"
#include "sequencergui/monitor/procedurerunner.h"

#include <iostream>

namespace
{
std::string GetStatus(const instruction_t *instruction)
{
  return ::sup::sequencer::StatusToString(instruction->GetStatus());
}
}  // namespace

namespace sequi
{

JobContext::JobContext(ProcedureItem *procedure_item, QObject *parent)
    : QObject(parent)
    , m_guiobject_builder(std::make_unique<GUIObjectBuilder>())
    , m_procedure_runner(std::make_unique<ProcedureRunner>())
    , m_job_log(new JobLog)
    , m_procedure_item{procedure_item}
    , m_job_model(std::make_unique<SequencerModel>())
{
  SetupConnections();
}

void JobContext::onPrepareJobRequest()
{
  // building domain procedure
  DomainObjectBuilder builder;
  m_domain_procedure = builder.CreateProcedure(m_procedure_item);
  m_domain_procedure->Setup();  // to perform all necessary internal clones

  // creating ProcedureItem corresponding to the domain procedure after the setup
  if (!m_job_model->GetProcedureContainer()->IsEmpty())
  {
    throw std::runtime_error("Error non empty container");
  }
  m_expanded_procedure_item =
      m_job_model->InsertItem<ProcedureItem>(m_job_model->GetProcedureContainer());
  m_guiobject_builder->PopulateProcedureItem(m_domain_procedure.get(), m_expanded_procedure_item);
}

JobContext::~JobContext() = default;

void JobContext::onStartRequest()
{
  if (m_procedure_runner->IsBusy())
  {
    return;
  }

  if (!m_domain_procedure)
  {
    throw std::runtime_error("Error in JobContext: procedure is not ready");
  }

  m_job_log->ClearLog();

  m_procedure_runner->ExecuteProcedure(m_domain_procedure.get(), false); // do not setup
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

void JobContext::SetWaitingMode(WaitingMode waiting_mode)
{
  m_procedure_runner->SetWaitingMode(waiting_mode);
}

void JobContext::SetSleepTime(int time_msec)
{
  m_procedure_runner->SetSleepTime(time_msec);
}

ProcedureItem *JobContext::GetExpandedProcedure() const
{
  return m_expanded_procedure_item;
}

SequencerModel *JobContext::GetExpandedModel()
{
  return m_job_model.get();
}

bool JobContext::IsRunning() const
{
  return m_procedure_runner->IsBusy();
}

void JobContext::SetMessagePanel(MessagePanel *panel)
{
  m_job_log->SetMessagePanel(panel);
}

bool JobContext::WaitForCompletion(double timeout_sec)
{
  return m_procedure_runner->WaitForCompletion(timeout_sec);
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
  m_job_log->Append(message.toStdString(), static_cast<MessageType>(message_type));
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
  }
  else
  {
    std::cout << "Error in JobManager: can't find VariableItem" << std::endl;
  }
}

void JobContext::SetupConnections()
{
  connect(m_procedure_runner.get(), &ProcedureRunner::InstructionStatusChanged, this,
          &JobContext::onInstructionStatusChange, Qt::QueuedConnection);

  connect(m_procedure_runner.get(), &ProcedureRunner::LogMessageRequest, this,
          &JobContext::onLogMessage, Qt::QueuedConnection);

  connect(m_procedure_runner.get(), &ProcedureRunner::VariableChanged, this,
          &JobContext::onVariableChange, Qt::QueuedConnection);
}

}  // namespace sequi
