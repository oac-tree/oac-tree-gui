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

#include "procedure_reporter.h"

#include "log_event.h"
#include "sequencer_observer.h"
#include "user_choice_provider.h"
#include "user_input_provider.h"

#include <sup/sequencer/procedure.h>

#include <iostream>

namespace sequencergui
{

ProcedureReporter::ProcedureReporter(get_instruction_t callback, QObject *parent)
    : QObject(parent)
    , m_observer(std::make_unique<SequencerObserver>(this))
    , m_signal_queue(new SignalQueue(this))
    , m_get_domain_instruction(callback)
{
  //
  connect(m_signal_queue, &SignalQueue::RunnerStatusChanged, this,
          &ProcedureReporter::RunnerStatusChanged, Qt::QueuedConnection);
}

ProcedureReporter::~ProcedureReporter() = default;

void ProcedureReporter::SetUserContext(const UserContext &user_context)
{
  m_choice_provider = std::make_unique<UserChoiceProvider>(user_context.m_user_choice_callback);
  m_input_provider = std::make_unique<UserInputProvider>(user_context.m_user_input_callback);
}

void ProcedureReporter::OnInstructionStatusChange(const instruction_t *instruction,
                                                  const std::string &value)
{
  emit InstructionStatusChanged(instruction, QString::fromStdString(value));
}

//! Propagate log message from observer up in the form of signals.

void ProcedureReporter::OnLogEvent(const LogEvent &event)
{
  emit LogEventReceived(event);
}

UserInputResult ProcedureReporter::OnUserInput(const UserInputArgs &args)
{
  return m_input_provider->GetUserInput(args);
}

UserChoiceResult ProcedureReporter::OnUserChoice(const UserChoiceArgs &args)
{
  return m_choice_provider->GetUserChoice(args);
}

SequencerObserver *ProcedureReporter::GetObserver()
{
  return m_observer.get();
}

void ProcedureReporter::OnDomainRunnerStatusChanged(RunnerStatus status)
{
  // sending via Qt::QueuedConnection since
  emit m_signal_queue->RunnerStatusChanged(status);
}

void ProcedureReporter::OnDomainProcedureTick(const procedure_t &procedure)
{
  std::cout << "tick " << ::sup::sequencer::GetNextLeaves(procedure).size() << std::endl;
}

}  // namespace sequencergui
