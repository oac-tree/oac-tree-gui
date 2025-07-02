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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_DOMAIN_EVENT_LISTENER_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_DOMAIN_EVENT_LISTENER_H_

#include <oac_tree_gui/jobsystem/domain_event_dispatcher_context.h>
#include <oac_tree_gui/jobsystem/domain_events.h>

#include <gmock/gmock.h>

namespace oac_tree_gui
{

class InstructionStatusChangedEvent;
class JobStateChangedEvent;

namespace test
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
  std::function<void(const domain_event_t& event)> CreateCallback()
  {
    return [this](const domain_event_t& arg) { OnEvent(arg); };
  }

  /**
   * @brief Distributes domain events to separate mocking methods.
   */
  void OnEvent(const domain_event_t& event) { std::visit(*this, event); }

  /**
   * @brief Operator to visit monostate.
   */
  void operator()(const std::monostate& event) const {}

  /**
   * @brief Operator to visit InstructionStateUpdated and trigger mock method.
   */
  void operator()(const InstructionStateUpdatedEvent& event) const
  {
    OnInstructionStateUpdated(event);
  }

  /**
   * @brief Operator to visit VariableStateUpdated and trigger mock method.
   */
  void operator()(const VariableUpdatedEvent& event) const { OnVariableUpdated(event); }

  /**
   * @brief Operator to visit JobStateChanged and trigger mock method.
   */
  void operator()(const JobStateChangedEvent& event) const { OnJobStateChanged(event); }

  /**
   * @brief Operator to visit LogEvent and trigger mock method.
   */
  void operator()(const LogEvent& event) const { OnLogEvent(event); }

  /**
   * @brief Operator to visit ActiveInstructionChangedEvent and trigger mock method.
   */
  void operator()(const ActiveInstructionChangedEvent& event) const
  {
    OnActiveInstructionChanged(event);
  }

  /**
   * @brief Operator to visit ActiveInstructionChangedEvent and trigger mock method.
   */
  void operator()(const BreakpointHitEvent& event) const { OnBreakpointHitEvent(event); }

  MOCK_METHOD(void, OnInstructionStateUpdated, (const InstructionStateUpdatedEvent&), (const));
  MOCK_METHOD(void, OnVariableUpdated, (const VariableUpdatedEvent&), (const));
  MOCK_METHOD(void, OnJobStateChanged, (const JobStateChangedEvent&), (const));
  MOCK_METHOD(void, OnLogEvent, (const LogEvent&), (const));
  MOCK_METHOD(void, OnActiveInstructionChanged, (const ActiveInstructionChangedEvent&), (const));
  MOCK_METHOD(void, OnBreakpointHitEvent, (const BreakpointHitEvent&), (const));

  /**
   * @brief Creates a structure with callbacks to trigger mock methods.
   */
  DomainEventDispatcherContext CreateDispatcherContext()
  {
    DomainEventDispatcherContext result;

    result.process_instruction_state_updated = [this](const InstructionStateUpdatedEvent& event)
    { OnInstructionStateUpdated(event); };

    result.process_variable_updated = [this](const VariableUpdatedEvent& event)
    { OnVariableUpdated(event); };

    result.process_job_state_changed = [this](const JobStateChangedEvent& event)
    { OnJobStateChanged(event); };

    result.process_log_event = [this](const LogEvent& event) { OnLogEvent(event); };

    result.active_instruction_changed_event = [this](const ActiveInstructionChangedEvent& event)
    { OnActiveInstructionChanged(event); };

    return result;
  }
};

}  // namespace test

}  // namespace oac_tree_gui

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_DOMAIN_EVENT_LISTENER_H_
