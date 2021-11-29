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
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/monitor/joblog.h"
#include "sequencergui/monitor/procedurerunner.h"

namespace
{
std::string GetStatus(const instruction_t *instruction)
{
  return ::sup::sequencer::StatusToString(instruction->GetStatus());
}
}  // namespace

namespace sequi
{

JobContext::JobContext(SequencerModel *model, ProcedureItem *procedure_item, QObject *parent)
    : QObject(parent)
    , m_domain_builder(std::make_unique<DomainObjectBuilder>())
    , m_procedure_runner(std::make_unique<ProcedureRunner>())
    , m_job_log(new JobLog)
    , m_procedure_item{procedure_item}
    , m_model(model)
{
  SetupConnections();
}

JobContext::~JobContext() = default;

void JobContext::onStartRequest()
{
  if (m_procedure_runner->IsBusy())
  {
    return;
  }

  m_job_log->clearLog();

  m_domain_builder->BuildProcedure(m_procedure_item);
  m_procedure_runner->ExecuteProcedure(m_domain_builder->GetProcedure());
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

void JobContext::onInstructionStatusChange(const instruction_t *instruction)
{
  if (!m_model)
  {
    throw std::runtime_error("Error in JobModel: uninitialised model.");
  }
  auto instruction_item =
      m_model->GetInstruction(m_domain_builder->FindInstructionIdentifier(instruction));
  if (!instruction_item)
  {
    throw std::runtime_error("Error in JobManager: can't find InstructionItem");
  }
  instruction_item->SetStatus(GetStatus(instruction));

  emit InstructionStatusChanged(instruction_item);
}

void JobContext::onLogMessage(const QString &message)
{
  m_job_log->append(message.toStdString(), JobLog::kDefault);
}

void JobContext::onVariableChange(const QString &variable_name, const QString &value)
{
  auto id = m_domain_builder->FindVariableItemIdentifier(variable_name.toStdString());
  if (auto item = m_model->FindItem(id); item)
  {
    if (auto local_var = dynamic_cast<LocalVariableItem *>(item); local_var)
    {
      local_var->SetJsonValue(value.toStdString());
    }
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

bool JobContext::IsRunning() const
{
  return m_procedure_runner->IsBusy();
}

void JobContext::SetMessagePanel(MessagePanel *panel)
{
  m_job_log->setMessagePanel(panel);
}

bool JobContext::WaitForCompletion(double timeout_sec)
{
  return m_procedure_runner->WaitForCompletion(timeout_sec);
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
