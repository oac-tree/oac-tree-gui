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

#include "procedure_runner.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/domain_runner_adapter.h>
#include <sequencergui/jobsystem/job_utils.h>
#include <sequencergui/jobsystem/log_event.h>
#include <sequencergui/jobsystem/sequencer_observer.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/runner.h>

#include <QDebug>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace sequencergui
{
ProcedureRunner::ProcedureRunner(procedure_t *procedure, QObject *parent)
    : QObject(parent), m_observer(std::make_unique<SequencerObserver>(this))
{
  auto status_changed = [this](auto) { emit RunnerStatusChanged(); };
  m_domain_runner_adapter =
      std::make_unique<DomainRunnerAdapter>(procedure, m_observer.get(), status_changed);
}

ProcedureRunner::~ProcedureRunner() = default;

//! Starts new job, or release paused job.

bool ProcedureRunner::Start()
{
  return m_domain_runner_adapter->Start();
}

//! Make a step in paused job.

bool ProcedureRunner::Step()
{
  return m_domain_runner_adapter->Step();
}

//! Stop currently running procedure.

bool ProcedureRunner::Stop()
{
  return m_domain_runner_adapter->Stop();
}

bool ProcedureRunner::Pause()
{
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

void ProcedureRunner::onInstructionStatusChange(const instruction_t *instruction,
                                                const std::string &value)
{
  std::cout << "ProcedureRunner::onInstructionStatusChange" << instruction << " "
            << static_cast<int>(instruction->GetStatus()) << "qqq" << value << std::endl;
  emit InstructionStatusChanged(instruction, QString::fromStdString(value));
}

//! Propagate log message from observer up in the form of signals.

void ProcedureRunner::OnLogEvent(const LogEvent &event)
{
  emit LogEventReceived(event);
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

}  // namespace sequencergui
