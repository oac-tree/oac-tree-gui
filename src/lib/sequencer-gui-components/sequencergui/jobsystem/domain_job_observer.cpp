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

#include "domain_job_observer.h"

#include "user_choice_provider.h"
#include "user_context.h"
#include "user_input_provider.h"

#include <sequencergui/core/exceptions.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/utils/string_utils.h>

#include <sup/sequencer/instruction.h>

#include <cmath>
#include <sstream>
#include <thread>

namespace sequencergui
{

DomainJobObserver::DomainJobObserver(post_event_callback_t post_event_callback,
                                                     const UserContext &user_context)
    : m_post_event_callback(std::move(post_event_callback))
{
  if (!m_post_event_callback)
  {
    throw RuntimeException("Callback is not initialised");
  }

  if (user_context.user_choice_callback)
  {
    m_choice_provider = std::make_unique<UserChoiceProvider>(user_context.user_choice_callback);
  }

  if (user_context.user_input_callback)
  {
    m_input_provider = std::make_unique<UserInputProvider>(user_context.user_input_callback);
  }
}

DomainJobObserver::~DomainJobObserver() = default;

void DomainJobObserver::InitNumberOfInstructions(sup::dto::uint32 n_instr)
{
  (void)n_instr;
}

void DomainJobObserver::InstructionStateUpdated(sup::dto::uint32 instr_idx,
                                                        sup::sequencer::InstructionState state)
{
  m_post_event_callback(InstructionStateUpdatedEvent{instr_idx, state});
}

void DomainJobObserver::VariableUpdated(sup::dto::uint32 var_idx,
                                                const sup::dto::AnyValue &value, bool connected)
{
  m_post_event_callback(VariableUpdatedEvent{var_idx, value, connected});
}

void DomainJobObserver::JobStateUpdated(sup::sequencer::JobState state)
{
  {
    const std::lock_guard<std::mutex> lock{m_mutex};
    m_state = state;
    m_post_event_callback(JobStateChangedEvent{state});
  }
  m_cv.notify_one();
}

void DomainJobObserver::PutValue(const sup::dto::AnyValue &value,
                                         const std::string &description)
{
  auto value_string = sup::gui::ValuesToJSONString(value);
  std::ostringstream ostr;
  ostr << description << " " << value_string;
  m_post_event_callback(CreateLogEvent(Severity::kInfo, ostr.str()));
}

bool DomainJobObserver::GetUserValue(sup::dto::uint64 id, sup::dto::AnyValue &value,
                                             const std::string &description)
{
  (void)id; // TODO send this id to the GUI

  if (m_input_provider)
  {
    auto result = m_input_provider->GetUserInput({value, description});
    value = result.value;
    return result.processed;
  }

  m_post_event_callback(CreateLogEvent(Severity::kWarning, "User input callback is not set"));
  return false;
}

int DomainJobObserver::GetUserChoice(sup::dto::uint64 id, const std::vector<std::string> &options,
                                     const sup::dto::AnyValue &metadata)
{
  (void)id; // TODO send this id to the GUI

  if (m_choice_provider)
  {
    auto result = m_choice_provider->GetUserChoice({options, metadata});
    return result.processed ? result.index : -1;
  }

  m_post_event_callback(CreateLogEvent(Severity::kWarning, "User choice callback is not set"));
  return -1;
}

void DomainJobObserver::Interrupt(sup::dto::uint64 id)
{
  // TODO What to do here?
  (void)id;
}

void DomainJobObserver::Message(const std::string &message)
{
  m_post_event_callback(CreateLogEvent(Severity::kInfo, message));
}

void DomainJobObserver::Log(int severity, const std::string &message)
{
  // assuming sequencer severity is the same as GUI severity
  m_post_event_callback(CreateLogEvent(static_cast<Severity>(severity), message));
}

void DomainJobObserver::NextInstructionsUpdated(
    const std::vector<sup::dto::uint32> &instr_indices)
{
  std::unique_lock<std::mutex> lock{m_mutex};

  m_post_event_callback(NextLeavesChangedEvent{instr_indices});

  if (m_tick_timeout_msec > 0 && !IsLastTick())
  {
    lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(m_tick_timeout_msec));
  }
}

sup::sequencer::JobState DomainJobObserver::GetCurrentState() const
{
  const std::lock_guard<std::mutex> lock{m_mutex};
  return m_state;
}

bool DomainJobObserver::WaitForState(sup::sequencer::JobState state, double msec) const
{
  const double nanosec_per_msec{1e6};
  auto duration = std::chrono::nanoseconds(std::lround(msec * nanosec_per_msec));
  std::unique_lock<std::mutex> lock{m_mutex};
  return m_cv.wait_for(lock, duration, [this, state]() { return m_state == state; });
}

sup::sequencer::JobState DomainJobObserver::WaitForFinished() const
{
  auto pred = [this]() { return sup::sequencer::IsFinishedJobState(m_state); };
  std::unique_lock<std::mutex> lock{m_mutex};
  m_cv.wait(lock, pred);
  return m_state;
}

void DomainJobObserver::SetTickTimeout(int msec)
{
  const std::unique_lock<std::mutex> lock{m_mutex};
  m_tick_timeout_msec = msec;
}

bool DomainJobObserver::IsLastTick()
{
  // return status == ExecutionStatus::SUCCESS || status == ExecutionStatus::FAILURE;
  // How to check last tick?
  return false;
}

}  // namespace sequencergui
