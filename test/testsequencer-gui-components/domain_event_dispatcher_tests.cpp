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

#include "sequencergui/experimental/domain_event_dispatcher.h"

#include <sequencergui/experimental/domain_events.h>

#include <mvvm/test/mock_callback_listener.h>

#include <gtest/gtest.h>

using namespace sequencergui;
using ::testing::_;

//! Tests for DomainEventDispatcher class.

class DomainEventDispatcherTest : public ::testing::Test
{
public:
  /**
   * @brief Test helper method to create a context necessary for DomainEventDispatcher to function.
   */
  DomainEventDispatcherContext CreateContext()
  {
    return {m_instruction_status_listener.CreateCallback(), m_job_status_listener.CreateCallback()};
  }

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
    return std::make_unique<DomainEventDispatcher>(CreateGetEventCallback(event), CreateContext());
  }

  mvvm::test::MockCallbackListener<InstructionStatusChanged> m_instruction_status_listener;
  mvvm::test::MockCallbackListener<JobStateChanged> m_job_status_listener;
};

TEST_F(DomainEventDispatcherTest, EmptyEvent)
{
  const domain_event_t event;
  auto dispatcher = CreateDispatcher(event);

  EXPECT_CALL(m_instruction_status_listener, OnCallback(_)).Times(0);
  EXPECT_CALL(m_job_status_listener, OnCallback(_)).Times(0);

  dispatcher->OnNewEvent();
}

TEST_F(DomainEventDispatcherTest, InstructionStatusChanged)
{
  InstructionStatusChanged expected_event{nullptr, ::sup::sequencer::ExecutionStatus::NOT_STARTED};
  auto dispatcher = CreateDispatcher(expected_event);

  EXPECT_CALL(m_instruction_status_listener, OnCallback(expected_event)).Times(1);
  EXPECT_CALL(m_job_status_listener, OnCallback(_)).Times(0);

  dispatcher->OnNewEvent();
}

TEST_F(DomainEventDispatcherTest, JobStatusChanged)
{
  JobStateChanged expected_event{::sup::sequencer::JobState::kInitial};
  auto dispatcher = CreateDispatcher(expected_event);

  EXPECT_CALL(m_instruction_status_listener, OnCallback(_)).Times(0);
  EXPECT_CALL(m_job_status_listener, OnCallback(expected_event)).Times(1);

  dispatcher->OnNewEvent();
}
