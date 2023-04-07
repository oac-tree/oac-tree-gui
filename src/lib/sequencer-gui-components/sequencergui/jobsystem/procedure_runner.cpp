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
#include <sequencergui/jobsystem/user_choice_provider.h>
#include <sequencergui/jobsystem/user_input_provider.h>

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
//  auto status_changed = [this](auto) { emit RunnerStatusChanged(); };
//  m_domain_runner_adapter =
//      std::make_unique<DomainRunnerAdapter>(procedure, m_observer.get(), status_changed);
}

ProcedureRunner::~ProcedureRunner() = default;

//! Starts new job, or release paused job.

//bool ProcedureRunner::Start()
//{
//  return false;
////  return m_domain_runner_adapter->Start();
//}

////! Make a step in paused job.

//bool ProcedureRunner::Step()
//{
//  return false;
////  return m_domain_runner_adapter->Step();
//}

////! Stop currently running procedure.

//bool ProcedureRunner::Stop()
//{
//  return false;
////  return m_domain_runner_adapter->Stop();
//}

//bool ProcedureRunner::Pause()
//{
//  return false;
////  return m_domain_runner_adapter->Pause();
//}

//void ProcedureRunner::SetSleepTime(int time_msec)
//{
////  m_domain_runner_adapter->SetTickTimeout(time_msec);
//}

void ProcedureRunner::SetUserContext(const UserContext &user_context)
{
  m_choice_provider = std::make_unique<UserChoiceProvider>(user_context.m_user_choice_callback);
  m_input_provider = std::make_unique<UserInputProvider>(user_context.m_user_input_callback);
}

//bool ProcedureRunner::IsBusy() const
//{
//  return false;
////  return m_domain_runner_adapter ? m_domain_runner_adapter->IsBusy() : false;
//}

//RunnerStatus ProcedureRunner::GetRunnerStatus() const
//{
////  return m_domain_runner_adapter ? m_domain_runner_adapter->GetStatus() : RunnerStatus::kIdle;
//  return RunnerStatus::kIdle;
//}

void ProcedureRunner::OnInstructionStatusChange(const instruction_t *instruction,
                                                const std::string &value)
{
  emit InstructionStatusChanged(instruction, QString::fromStdString(value));
}

//! Propagate log message from observer up in the form of signals.

void ProcedureRunner::OnLogEvent(const LogEvent &event)
{
  emit LogEventReceived(event);
}

UserInputResult ProcedureRunner::OnUserInput(const UserInputArgs& args)
{
  return m_input_provider->GetUserInput(args);
}

UserChoiceResult ProcedureRunner::OnUserChoice(const UserChoiceArgs &args)
{
  return m_choice_provider->GetUserChoice(args);
}

SequencerObserver *ProcedureRunner::GetObserver()
{
  return m_observer.get();
}

}  // namespace sequencergui
