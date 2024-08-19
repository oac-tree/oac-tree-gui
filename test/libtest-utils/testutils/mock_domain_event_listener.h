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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_DOMAIN_EVENT_LISTENER_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_DOMAIN_EVENT_LISTENER_H_

#include <sequencergui/jobsystem/domain_event_dispatcher_context.h>
#include <sequencergui/jobsystem/domain_events.h>

#include <gmock/gmock.h>

namespace sequencergui
{
class InstructionStatusChangedEvent;
class JobStateChangedEvent;
}  // namespace sequencergui

namespace testutils
{

/**
 * @brief The MockDomainEventListener class is a test class to listen for events coming from the
 * DomainRunner.
 *
 * It distributes domain_event_t flow into calls of several mocking methods corresponding to
 * concrete event type.
 */
class MockDomainEventListener
{
public:
  /**
   * @brief Creates a callback for DomainRunner.
   */
  std::function<void(const sequencergui::domain_event_t& event)> CreateCallback()
  {
    return [this](const sequencergui::domain_event_t& arg) { OnEvent(arg); };
  }

  /**
   * @brief Distributes domain events to separate mocking methods.
   */
  void OnEvent(const sequencergui::domain_event_t& event) { std::visit(*this, event); }

  /**
   * @brief Operator to visit monostate.
   */
  void operator()(const std::monostate& event) const {}

  /**
   * @brief Operator to visit InstructionStatusChanged and trigger mock method.
   */
  void operator()(const sequencergui::InstructionStatusChangedEvent& instruction_event) const
  {
    OnInstructionStatusChanged(instruction_event);
  }

  /**
   * @brief Operator to visit JobStateChanged and trigger mock method.
   */
  void operator()(const sequencergui::JobStateChangedEvent& job_event) const
  {
    OnJobStateChanged(job_event);
  }

  /**
   * @brief Operator to visit LogEvent and trigger mock method.
   */
  void operator()(const sequencergui::LogEvent& log_event) const { OnLogEvent(log_event); }

  /**
   * @brief Operator to visit NextLeavesChanged and trigger mock method.
   */
  void operator()(const sequencergui::NextLeavesChangedEvent& leaves_event) const
  {
    OnNextLeavesChanged(leaves_event);
  }

  /**
   * @brief Operator to visit InstructionStateUpdated and trigger mock method.
   */
  void operator()(const sequencergui::InstructionStateUpdatedEvent& instruction_event) const
  {
    OnInstructionStateUpdated(instruction_event);
  }

  MOCK_METHOD(void, OnInstructionStatusChanged,
              (const sequencergui::InstructionStatusChangedEvent&), (const));
  MOCK_METHOD(void, OnJobStateChanged, (const sequencergui::JobStateChangedEvent&), (const));
  MOCK_METHOD(void, OnLogEvent, (const sequencergui::LogEvent&), (const));
  MOCK_METHOD(void, OnNextLeavesChanged, (const sequencergui::NextLeavesChangedEvent&), (const));
  MOCK_METHOD(void, OnInstructionStateUpdated, (const sequencergui::InstructionStateUpdatedEvent&),
              (const));

  /**
   * @brief Creates a structure with callbacks to trigger mock methods.
   */
  sequencergui::DomainEventDispatcherContext CreateDispatcherContext()
  {
    sequencergui::DomainEventDispatcherContext result;

    auto instruction_status_changed =
        [this](const sequencergui::InstructionStatusChangedEvent& event)
    { OnInstructionStatusChanged(event); };

    auto job_state_changed = [this](const sequencergui::JobStateChangedEvent& event)
    { OnJobStateChanged(event); };

    auto log_event = [this](const sequencergui::LogEvent& event) { OnLogEvent(event); };

    auto next_leaves_event = [this](const sequencergui::NextLeavesChangedEvent& event)
    { OnNextLeavesChanged(event); };

    auto instruction_state_updated = [this](const sequencergui::InstructionStateUpdatedEvent& event)
    { OnInstructionStateUpdated(event); };

    return {instruction_status_changed, job_state_changed, log_event, next_leaves_event,
            instruction_state_updated};

    return result;
  }
};

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_DOMAIN_EVENT_LISTENER_H_
