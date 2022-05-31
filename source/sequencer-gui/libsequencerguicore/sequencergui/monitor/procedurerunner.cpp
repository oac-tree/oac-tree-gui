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
#include "sequencergui/jobsystem/domainrunneradapter.h"
#include "sequencergui/monitor/jobutils.h"
#include "sequencergui/monitor/sequencerobserver.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace sequencergui
{
ProcedureRunner::ProcedureRunner(QObject *parent)
    : QObject(parent)
    , m_observer(std::make_unique<SequencerObserver>(this))
    , m_runner_status(RunnerStatus::kIdle)
{
  m_flow_controller.SetWaitingMode(WaitingMode::kProceed);
}

ProcedureRunner::~ProcedureRunner()
{
  if (m_domain_runner)
  {
    m_domain_runner->Halt();
  }

  m_flow_controller.Interrupt();  // to prevent possible waiting on step request

  if (m_runner_thread.joinable())
  {
    m_runner_thread.join();
  }
}

//! Executes given procedure in a thread.
//! Doesn't have

void ProcedureRunner::ExecuteProcedure(procedure_t *procedure)
{
  if (IsBusy())
  {
    return;
  }

  auto runner = std::make_unique<runner_t>(m_observer.get());
  //  runner->SetProcedure(procedure);
  auto status_changed = [this](auto) { emit RunnerStatusChanged(); };
  m_domain_runner_adapter =
      std::make_unique<DomainRunnerAdapter>(std::move(runner), status_changed);

  if (m_runner_thread.joinable())
  {
    m_runner_thread.join();
  }

  SetRunnerStatus(RunnerStatus::kRunning);
  m_runner_thread = std::thread([this, procedure]() { LaunchDomainRunner(procedure); });
}

//! Performs request to release possible waiting in ProcedureRunner::onInstructionStatusChange.

void ProcedureRunner::Step()
{
  m_flow_controller.StepRequest();
}

//! Terminate currently running procedure

void ProcedureRunner::Stop()
{
  onLogMessage("ProcedureRunner::Terminate()", JobMessageType::kWarning);

  SetRunnerStatus(RunnerStatus::kStopping);

  if (m_domain_runner)
  {
    m_domain_runner->Halt();
  }

  m_flow_controller.Interrupt();  // to prevent possible waiting on step request

  if (m_runner_thread.joinable())
  {
    m_runner_thread.join();
  }
  SetRunnerStatus(RunnerStatus::kStopped);
}

void ProcedureRunner::SetWaitingMode(WaitingMode waiting_mode)
{
  m_flow_controller.SetWaitingMode(waiting_mode);
}

void ProcedureRunner::SetSleepTime(int time_msec)
{
  m_flow_controller.SetSleepTime(time_msec);
}

bool ProcedureRunner::WaitForCompletion(double timeout_sec)
{
  auto timeout =
      std::chrono::system_clock::now() + std::chrono::nanoseconds(std::lround(timeout_sec * 1e9));
  while (std::chrono::system_clock::now() < timeout)
  {
    if (!IsBusy())
    {
      return true;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  return false;
}

void ProcedureRunner::SetUserContext(const UserContext &user_context)
{
  m_user_controller.SetUserContext(user_context);
}

bool ProcedureRunner::IsBusy() const
{
  return m_runner_status == RunnerStatus::kRunning || m_runner_status == RunnerStatus::kStopping
         || m_runner_status == RunnerStatus::kPaused;
}

RunnerStatus ProcedureRunner::GetRunnerStatus() const
{
  std::lock_guard lock(m_mutex);
  return m_runner_status;
}

//! Performs necessary activity on domain instruction status change.
//! This is the place where we pause execution, if necessary.
void ProcedureRunner::onInstructionStatusChange(const instruction_t *instruction)
{
  emit InstructionStatusChanged(instruction);
  m_flow_controller.WaitIfNecessary();
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

void ProcedureRunner::LaunchDomainRunner(procedure_t *procedure)
{
  //  SetRunnerStatus(RunnerStatus::kRunning);

  m_domain_runner = std::make_unique<sup::sequencer::Runner>(m_observer.get());
  m_domain_runner->SetProcedure(procedure);
  m_domain_runner->ExecuteProcedure();
  procedure->Reset();
  if (GetRunnerStatus() != RunnerStatus::kStopping)
  {
    SetRunnerStatus(RunnerStatus::kCompleted);
  }
}

void ProcedureRunner::SetRunnerStatus(RunnerStatus value)
{
  std::lock_guard lock(m_mutex);
  m_runner_status = value;
  emit RunnerStatusChanged();
}

}  // namespace sequencergui
