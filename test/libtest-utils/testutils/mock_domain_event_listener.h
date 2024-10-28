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
   * @brief Operator to visit JobStateChanged and trigger mock method.
   */
  void operator()(const sequencergui::JobStateChangedEvent& event) const
  {
    OnJobStateChanged(event);
  }

  /**
   * @brief Operator to visit LogEvent and trigger mock method.
   */
  void operator()(const sequencergui::LogEvent& event) const { OnLogEvent(event); }

  /**
   * @brief Operator to visit NextLeavesChanged and trigger mock method.
   */
  void operator()(const sequencergui::NextLeavesChangedEvent& event) const
  {
    OnNextLeavesChangedV2(event);
  }

  /**
   * @brief Operator to visit VariableStateUpdated and trigger mock method.
   */
  void operator()(const sequencergui::VariableUpdatedEvent& event) const
  {
    OnVariableUpdated(event);
  }

  /**
   * @brief Operator to visit InstructionStateUpdated and trigger mock method.
   */
  void operator()(const sequencergui::InstructionStateUpdatedEvent& event) const
  {
    OnInstructionStateUpdated(event);
  }

  MOCK_METHOD(void, OnJobStateChanged, (const sequencergui::JobStateChangedEvent&), (const));
  MOCK_METHOD(void, OnLogEvent, (const sequencergui::LogEvent&), (const));
  MOCK_METHOD(void, OnNextLeavesChangedV2, (const sequencergui::NextLeavesChangedEvent&),
              (const));
  MOCK_METHOD(void, OnInstructionStateUpdated, (const sequencergui::InstructionStateUpdatedEvent&),
              (const));
  MOCK_METHOD(void, OnVariableUpdated, (const sequencergui::VariableUpdatedEvent&), (const));

  /**
   * @brief Creates a structure with callbacks to trigger mock methods.
   */
  sequencergui::DomainEventDispatcherContext CreateDispatcherContext()
  {
    sequencergui::DomainEventDispatcherContext result;

    result.process_job_state_changed = [this](const sequencergui::JobStateChangedEvent& event)
    { OnJobStateChanged(event); };

    result.process_log_event = [this](const sequencergui::LogEvent& event) { OnLogEvent(event); };

    result.next_leaves_changed_event =
        [this](const sequencergui::NextLeavesChangedEvent& event)
    { OnNextLeavesChangedV2(event); };

    result.process_instruction_state_updated =
        [this](const sequencergui::InstructionStateUpdatedEvent& event)
    { OnInstructionStateUpdated(event); };

    result.process_variable_updated = [this](const sequencergui::VariableUpdatedEvent& event)
    { OnVariableUpdated(event); };

    return result;
  }
};

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_DOMAIN_EVENT_LISTENER_H_
