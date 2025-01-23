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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac-tree-gui/jobsystem/domain_event_dispatcher.h"

#include <oac-tree-gui/jobsystem/domain_events.h>

#include <gtest/gtest.h>
#include <testutils/mock_domain_event_listener.h>

namespace sequencergui::test
{

//! Tests for DomainEventDispatcher class.

class DomainEventDispatcherTest : public ::testing::Test
{
public:
  /**
   * @brief Test helper method to create a callback reporting given event.
   */
  static DomainEventDispatcher::get_event_callback_t CreateGetEventCallback(
      const domain_event_t& event)
  {
    return [event]() { return event; };
  }

  /**
   * @brief Test helper method to create a dispatcher acting on given event.
   */
  std::unique_ptr<DomainEventDispatcher> CreateDispatcher(const domain_event_t& event)
  {
    return std::make_unique<DomainEventDispatcher>(CreateGetEventCallback(event),
                                                   m_listener.CreateDispatcherContext());
  }

  test::MockDomainEventListener m_listener;
};

TEST_F(DomainEventDispatcherTest, EmptyEvent)
{
  using ::testing::_;

  const domain_event_t event;
  auto dispatcher = CreateDispatcher(event);

  EXPECT_CALL(m_listener, OnInstructionStateUpdated(_)).Times(0);
  EXPECT_CALL(m_listener, OnJobStateChanged(_)).Times(0);

  dispatcher->OnNewEvent();
}

TEST_F(DomainEventDispatcherTest, InstructionStateUpdatedChanged)
{
  using ::testing::_;

  InstructionStateUpdatedEvent expected_event{
      0, sup::oac_tree::InstructionState{false, ::sup::oac_tree::ExecutionStatus::NOT_STARTED}};
  auto dispatcher = CreateDispatcher(expected_event);

  EXPECT_CALL(m_listener, OnInstructionStateUpdated(expected_event)).Times(1);
  EXPECT_CALL(m_listener, OnJobStateChanged(_)).Times(0);

  dispatcher->OnNewEvent();
}

TEST_F(DomainEventDispatcherTest, JobStatusChanged)
{
  using ::testing::_;

  JobStateChangedEvent expected_event{::sup::oac_tree::JobState::kInitial};
  auto dispatcher = CreateDispatcher(expected_event);

  EXPECT_CALL(m_listener, OnInstructionStateUpdated(_)).Times(0);
  EXPECT_CALL(m_listener, OnJobStateChanged(expected_event)).Times(1);

  dispatcher->OnNewEvent();
}

}
