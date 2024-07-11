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

#include "sequencergui/jobsystem/domain_event_queue.h"

#include <gtest/gtest.h>

#include <QSignalSpy>

using namespace sequencergui;

//! Tests for DomainEventQueue class.

class DomainEventQueueTest : public ::testing::Test
{
};

TEST_F(DomainEventQueueTest, InitialState)
{
  DomainEventQueue queue;
  EXPECT_EQ(queue.GetEventCount(), 0);

  auto event = queue.PopEvent();
  EXPECT_FALSE(IsValid(event));
}

TEST_F(DomainEventQueueTest, PushAndPop)
{
  DomainEventQueue queue;

  QSignalSpy spy_queue(&queue, &DomainEventQueue::NewEvent);

  domain_event_t event1(JobStateChangedEvent{::sup::sequencer::JobState::kInitial});
  domain_event_t event2(JobStateChangedEvent{::sup::sequencer::JobState::kSucceeded});

  queue.PushEvent(event1);
  queue.PushEvent(event2);

  EXPECT_EQ(spy_queue.count(), 2);

  EXPECT_EQ(queue.GetEventCount(), 2);
  EXPECT_EQ(queue.PopEvent(), event1);
  EXPECT_EQ(queue.GetEventCount(), 1);
  EXPECT_EQ(queue.PopEvent(), event2);
  EXPECT_EQ(queue.GetEventCount(), 0);
}
