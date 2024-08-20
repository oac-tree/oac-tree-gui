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

#include "remote_job_observer.h"

#include <sequencergui/core/exceptions.h>

#include <iostream>

namespace sequencergui
{

RemoteJobObserver::RemoteJobObserver(post_event_callback_t post_event_callback)
    : m_post_event_callback(std::move(post_event_callback))
{
  if (!m_post_event_callback)
  {
    throw RuntimeException("Callback is not initialised");
  }
}

void RemoteJobObserver::InitNumberOfInstructions(sup::dto::uint32 n_instr)
{
  std::cout << "RemoteJobObserver::InitNumberOfInstructions" << "\n";
}

void RemoteJobObserver::InstructionStateUpdated(sup::dto::uint32 instr_idx,
                                                sup::auto_server::InstructionState state)
{
  std::cout << "RemoteJobObserver::InstructionStateUpdated " << instr_idx << " "
            << ::sup::sequencer::StatusToString(state.m_execution_status) << "\n";
  m_post_event_callback(InstructionStateUpdatedEvent{instr_idx, state});
}

void RemoteJobObserver::VariableUpdated(sup::dto::uint32 var_idx, const sup::dto::AnyValue &value,
                                        bool connected)
{
  m_post_event_callback(VariableUpdatedEvent{var_idx, value, connected});
}

void RemoteJobObserver::JobStateUpdated(sup::sequencer::JobState state)
{
  std::cout << "RemoteJobObserver::JobStateUpdated " << ::sup::sequencer::ToString(state) << "\n";
  m_post_event_callback(JobStateChangedEvent{state});
}

bool RemoteJobObserver::PutValue(const sup::dto::AnyValue &value, const std::string &description)
{
  std::cout << "RemoteJobObserver::PutValue" << "\n";
  return false;
}

bool RemoteJobObserver::GetUserValue(sup::dto::AnyValue &value, const std::string &description)
{
  (void)value;
  (void)description;
  std::cout << "RemoteJobObserver::GetUserValue" << "\n";
  return false;
}

int RemoteJobObserver::GetUserChoice(const std::vector<std::string> &options,
                                     const sup::dto::AnyValue &metadata)
{
  (void)options;
  (void)metadata;
  std::cout << "RemoteJobObserver::GetUserChoice" << "\n";
  return 0;
}

void RemoteJobObserver::Message(const std::string &message)
{
  std::cout << "RemoteJobObserver::Message" << "\n";
}

void RemoteJobObserver::Log(int severity, const std::string &message)
{
  (void)severity;
  (void)message;
  std::cout << "RemoteJobObserver::Log" << "\n";
}

}  // namespace sequencergui
