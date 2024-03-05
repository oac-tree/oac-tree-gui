/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <sequencergui/model/instruction_item.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>

#include <QDebug>
#include <iostream>

namespace sequencergui
{

ProcedureReporter::ProcedureReporter(get_instruction_item_cb_t callback, QObject *parent)
    : QObject(parent)
    , m_observer(std::make_unique<SequencerObserver>(this))
    , m_signal_queue(new SignalQueue(this))
    , m_get_instruction_item(callback)
{
  connect(m_signal_queue, &SignalQueue::DomainInstructionStatusChanged, this,
          &ProcedureReporter::ProcessDomainInstructionStatusChange, Qt::QueuedConnection);
  connect(m_signal_queue, &SignalQueue::NextLeavesChanged, this,
          &ProcedureReporter::NextLeavesChanged, Qt::QueuedConnection);
  connect(m_signal_queue, &SignalQueue::RunnerStatusChanged, this,
          &ProcedureReporter::RunnerStatusChanged, Qt::QueuedConnection);
}

ProcedureReporter::~ProcedureReporter() = default;

void ProcedureReporter::SetUserContext(const UserContext &user_context)
{
  m_observer->SetUserContext(user_context);
}

//! Propagate log message from observer up in the form of signals.

void ProcedureReporter::OnLogEvent(const LogEvent &event)
{
  emit LogEventReceived(event);
}

SequencerObserver *ProcedureReporter::GetObserver()
{
  return m_observer.get();
}

void ProcedureReporter::OnDomainInstructionStatusChange(const instruction_t *instruction,
                                                        const std::string &value)
{
  // re-sending via Qt::QueuedConnection
  emit m_signal_queue->DomainInstructionStatusChanged(instruction, QString::fromStdString(value));
}

void ProcedureReporter::OnDomainRunnerStatusChanged(RunnerStatus status)
{
  // re-sending via Qt::QueuedConnection
  emit m_signal_queue->RunnerStatusChanged(status);
}

void ProcedureReporter::OnDomainProcedureTick(const procedure_t &procedure)
{
  std::vector<InstructionItem *> items;
  for (const auto *instruction : sup::sequencer::GetNextLeaves(procedure))
  {
    if (auto *item = m_get_instruction_item(*instruction); item)
    {
      items.push_back(item);
    }
    else
    {
      qWarning() << "Error in ProcedureReporter: can't find domain instruction counterpart";
    }
  }

  emit m_signal_queue->NextLeavesChanged(items);
}

void ProcedureReporter::ProcessDomainInstructionStatusChange(const instruction_t *instruction,
                                                             const QString &value)
{
  if (auto *item = m_get_instruction_item(*instruction); item)
  {
    item->SetStatus(value.toStdString());
    emit InstructionStatusChanged(item);
  }
  else
  {
    qWarning() << "Error in ProcedureReporter: can't find domain instruction counterpart";
  }
}

}  // namespace sequencergui
