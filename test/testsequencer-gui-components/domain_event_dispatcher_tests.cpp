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

using namespace sequencergui::experimental;
using ::testing::_;

//! Tests for DomainEventDispatcher class.

class DomainEventDispatcherTest : public ::testing::Test
{
public:
  /**
   * @brief Test helper method to create a context necessary for DomainEventDispatcher to function.
   */
  DomainEventDispatcherContext CreateContext(const domain_event_t& event)
  {
    auto get_event_callback = [this, event]() { return event; };
    return {get_event_callback, m_instruction_status_listener.CreateCallback(),
            m_job_status_listener.CreateCallback()};
  }

  mvvm::test::MockCallbackListener<InstructionStatusChanged> m_instruction_status_listener;
  mvvm::test::MockCallbackListener<JobStatusChanged> m_job_status_listener;
};

TEST_F(DomainEventDispatcherTest, EmptyEvent)
{
  domain_event_t event;
  DomainEventDispatcher dispatcher(CreateContext(event));

  EXPECT_CALL(m_instruction_status_listener, OnCallback(_)).Times(0);
  EXPECT_CALL(m_job_status_listener, OnCallback(_)).Times(0);

  dispatcher.OnNewEvent();
}

TEST_F(DomainEventDispatcherTest, InstructionStatusChanged)
{
  InstructionStatusChanged expected_event{nullptr, ::sup::sequencer::ExecutionStatus::NOT_STARTED};
  DomainEventDispatcher dispatcher(CreateContext(expected_event));

  EXPECT_CALL(m_instruction_status_listener, OnCallback(expected_event)).Times(1);
  EXPECT_CALL(m_job_status_listener, OnCallback(_)).Times(0);

  dispatcher.OnNewEvent();
}

TEST_F(DomainEventDispatcherTest, JobStatusChanged)
{
  JobStatusChanged expected_event{"abc"};
  DomainEventDispatcher dispatcher(CreateContext(expected_event));

  EXPECT_CALL(m_instruction_status_listener, OnCallback(_)).Times(0);
  EXPECT_CALL(m_job_status_listener, OnCallback(expected_event)).Times(1);

  dispatcher.OnNewEvent();
}
