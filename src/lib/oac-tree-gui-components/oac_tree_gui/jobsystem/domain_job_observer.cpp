/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "domain_job_observer.h"

#include "user_choice_provider.h"
#include "user_context.h"
#include "user_input_provider.h"

#include <oac_tree_gui/core/exceptions.h>

#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/utils/string_utils.h>

#include <sup/oac-tree/active_instruction_monitor.h>
#include <sup/oac-tree/instruction.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <thread>

namespace oac_tree_gui
{

DomainJobObserver::DomainJobObserver(post_event_callback_t post_event_callback,
                                     const UserContext &user_context)
    : m_post_event_callback(std::move(post_event_callback))
    , m_active_instruction_monitor(CreateActiveInstructionMonitor({}))
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
                                                sup::oac_tree::InstructionState state)
{
  m_post_event_callback(InstructionStateUpdatedEvent{instr_idx, state});

  std::cout << " InstructionStateUpdated " << instr_idx << " "
            << sup::oac_tree::StatusToString(state.m_execution_status) << " breakpoint set"
            << state.m_breakpoint_set << std::endl;

  {
    const std::lock_guard<std::mutex> lock{m_mutex};
    m_active_instruction_monitor->InstructionStatusUpdated(instr_idx, state.m_execution_status);
  }
}

void DomainJobObserver::BreakpointInstructionUpdated(sup::dto::uint32 instr_idx)
{
  std::cout << " BreakpointInstructionUpdated " << instr_idx << "\n";
  (void)instr_idx;
}

void DomainJobObserver::VariableUpdated(sup::dto::uint32 var_idx, const sup::dto::AnyValue &value,
                                        bool connected)
{
  m_post_event_callback(VariableUpdatedEvent{var_idx, value, connected});
}

void DomainJobObserver::JobStateUpdated(sup::oac_tree::JobState state)
{
  {
    const std::lock_guard<std::mutex> lock{m_mutex};
    m_state = state;
    m_post_event_callback(JobStateChangedEvent{state});
  }
  m_cv.notify_one();
}

void DomainJobObserver::PutValue(const sup::dto::AnyValue &value, const std::string &description)
{
  auto value_string = sup::gui::ValuesToJSONString(value);
  std::ostringstream ostr;
  ostr << "Put value request > " << description << " " << value_string;
  m_post_event_callback(CreateLogEvent(Severity::kInfo, ostr.str()));
}

bool DomainJobObserver::GetUserValue(sup::dto::uint64 id, sup::dto::AnyValue &value,
                                     const std::string &description)
{
  (void)id;  // TODO send this id to the GUI

  if (m_input_provider)
  {
    auto value_string = sup::gui::ValuesToJSONString(value);
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
  (void)id;  // TODO send this id to the GUI

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

void DomainJobObserver::NextInstructionsUpdated(const std::vector<sup::dto::uint32> &instr_indices)
{
  m_post_event_callback(ActiveInstructionChangedEvent{instr_indices});
}

void DomainJobObserver::ProcedureTicked()
{
  std::unique_lock<std::mutex> lock{m_mutex};
  if (m_tick_timeout_msec > 0)
  {
    lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(m_tick_timeout_msec));
  }
}

sup::oac_tree::JobState DomainJobObserver::GetCurrentState() const
{
  const std::lock_guard<std::mutex> lock{m_mutex};
  return m_state;
}

bool DomainJobObserver::WaitForState(sup::oac_tree::JobState state, double msec) const
{
  const double nanosec_per_msec{1e6};
  auto duration = std::chrono::nanoseconds(std::lround(msec * nanosec_per_msec));
  std::unique_lock<std::mutex> lock{m_mutex};
  return m_cv.wait_for(lock, duration, [this, state]() { return m_state == state; });
}

sup::oac_tree::JobState DomainJobObserver::WaitForFinished() const
{
  auto pred = [this]() { return sup::oac_tree::IsFinishedJobState(m_state); };
  std::unique_lock<std::mutex> lock{m_mutex};
  m_cv.wait(lock, pred);
  return m_state;
}

void DomainJobObserver::SetTickTimeout(int msec)
{
  const std::unique_lock<std::mutex> lock{m_mutex};
  m_tick_timeout_msec = msec;
}

void DomainJobObserver::SetInstructionActiveFilter(const active_filter_t &filter)
{
  const std::unique_lock<std::mutex> lock{m_mutex};
  m_active_instruction_monitor = CreateActiveInstructionMonitor(filter);
}

std::unique_ptr<DomainJobObserver::active_monitor_t>
DomainJobObserver::CreateActiveInstructionMonitor(const active_filter_t &filter)
{
  auto callback = [this](const auto &instr_idx)
  {
    const std::vector<sup::dto::uint32> leaves(instr_idx.begin(), instr_idx.end());
    NextInstructionsUpdated(leaves);
  };

  if (filter)
  {
    return std::make_unique<sup::oac_tree::ActiveInstructionMonitor>(callback, filter);
  }

  return std::make_unique<sup::oac_tree::ActiveInstructionMonitor>(callback);
}

}  // namespace oac_tree_gui
