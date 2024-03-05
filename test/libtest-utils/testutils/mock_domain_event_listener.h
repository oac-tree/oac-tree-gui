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

#include <sequencergui/experimental/domain_events.h>

#include <gmock/gmock.h>

namespace sequencergui
{
class InstructionStatusChanged;
class JobStateChanged;
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
  void operator()(const sequencergui::InstructionStatusChanged& instruction_event) const
  {
    OnInstructionStatusChanged(instruction_event);
  }

  /**
   * @brief Operator to visit JobStateChanged and trigger mock method.
   */
  void operator()(const sequencergui::JobStateChanged& job_event) const
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
  void operator()(const sequencergui::NextLeavesChanged& leaves_event) const
  {
    OnNextLeavesChangedEvent(leaves_event);
  }

  MOCK_METHOD(void, OnInstructionStatusChanged, (const sequencergui::InstructionStatusChanged&),
              (const));
  MOCK_METHOD(void, OnJobStateChanged, (const sequencergui::JobStateChanged&), (const));
  MOCK_METHOD(void, OnLogEvent, (const sequencergui::LogEvent&), (const));
  MOCK_METHOD(void, OnNextLeavesChangedEvent, (const sequencergui::NextLeavesChanged&), (const));
};

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_DOMAIN_EVENT_LISTENER_H_
