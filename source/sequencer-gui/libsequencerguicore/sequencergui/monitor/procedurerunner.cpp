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

#include "sequencergui/monitor/procedurerunner.h"

#include "Procedure.h"
#include "Runner.h"
#include "sequencergui/core/exceptions.h"
#include "sequencergui/jobsystem/domainrunneradapter.h"
#include "sequencergui/monitor/jobutils.h"
#include "sequencergui/monitor/sequencerobserver.h"

#include <QDebug>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace sequencergui
{
ProcedureRunner::ProcedureRunner(QObject *parent)
    : QObject(parent), m_observer(std::make_unique<SequencerObserver>(this))
{
}

ProcedureRunner::~ProcedureRunner() = default;

void ProcedureRunner::SetProcedure(procedure_t *procedure)
{
  if (IsBusy())
  {
    return;
  }

  auto runner = std::make_unique<runner_t>(m_observer.get());
  runner->SetProcedure(procedure);
  auto status_changed = [this](auto) { emit RunnerStatusChanged(); };
  m_domain_runner_adapter =
      std::make_unique<DomainRunnerAdapter>(std::move(runner), status_changed);
}

bool ProcedureRunner::Start()
{
  CheckConditions();
  return m_domain_runner_adapter->Start();
}

//! Performs request to release possible waiting in ProcedureRunner::onInstructionStatusChange.

bool ProcedureRunner::Step()
{
  CheckConditions();
  return m_domain_runner_adapter->Step();
}

//! Terminate currently running procedure

bool ProcedureRunner::Stop()
{
  CheckConditions();
  auto is_valid_request = m_domain_runner_adapter->Stop();
  if (is_valid_request)
  {
    onLogMessage("ProcedureRunner::Stop()", JobMessageType::kWarning);
  }

  return is_valid_request;
}

bool ProcedureRunner::Pause()
{
  CheckConditions();
  return m_domain_runner_adapter->Pause();
}

void ProcedureRunner::SetSleepTime(int time_msec)
{
  m_domain_runner_adapter->SetTickTimeout(time_msec);
}

void ProcedureRunner::SetUserContext(const UserContext &user_context)
{
  m_user_controller.SetUserContext(user_context);
}

bool ProcedureRunner::IsBusy() const
{
  return m_domain_runner_adapter ? m_domain_runner_adapter->IsBusy() : false;
}

RunnerStatus ProcedureRunner::GetRunnerStatus() const
{
  return m_domain_runner_adapter ? m_domain_runner_adapter->GetStatus() : RunnerStatus::kIdle;
}

void ProcedureRunner::onInstructionStatusChange(const instruction_t *instruction)
{
  emit InstructionStatusChanged(instruction);
}

//! Propagate log message from observer up in the form of signals.

void ProcedureRunner::onLogMessage(const std::string &message, JobMessageType message_type)
{
  emit LogMessageRequest(QString::fromStdString(message), static_cast<int>(message_type));
}

void ProcedureRunner::onVariableChange(const std::string &variable_name, const std::string &value)
{
  emit VariableChanged(QString::fromStdString(variable_name), QString::fromStdString(value));
}

std::string ProcedureRunner::onUserInput(const std::string &current_value,
                                         const std::string &description)
{
  return m_user_controller.GetUserInput(current_value, description);
}

int ProcedureRunner::onUserChoice(const std::vector<std::string> &choices,
                                  const std::string &description)
{
  return m_user_controller.GetUserChoice(choices, description);
}

void ProcedureRunner::CheckConditions() const
{
  if (!m_domain_runner_adapter)
  {
    throw RuntimeException("Domain runner is not initialised");
  }
}
}  // namespace sequencergui
