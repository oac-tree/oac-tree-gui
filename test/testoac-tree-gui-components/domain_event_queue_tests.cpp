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

#include "oac-tree-gui/jobsystem/domain_event_queue.h"

#include <oac-tree-gui/core/exceptions.h>

#include <gtest/gtest.h>

#include <QSignalSpy>


namespace oac_tree_gui::test
{

//! Tests for DomainEventQueue class.

class DomainEventQueueTest : public ::testing::Test
{
};

TEST_F(DomainEventQueueTest, InitialState)
{
  DomainEventQueue queue;
  EXPECT_EQ(queue.GetEventCount(), 0);

  // auto event = queue.PopEvent();
  EXPECT_THROW(queue.PopEvent(), RuntimeException);
}

TEST_F(DomainEventQueueTest, PushAndPop)
{
  DomainEventQueue queue;

  const QSignalSpy spy_queue(&queue, &DomainEventQueue::NewEvent);

  const domain_event_t event1(JobStateChangedEvent{::sup::oac_tree::JobState::kInitial});
  const domain_event_t event2(JobStateChangedEvent{::sup::oac_tree::JobState::kSucceeded});

  queue.PushEvent(event1);
  queue.PushEvent(event2);

  EXPECT_EQ(spy_queue.count(), 2);

  EXPECT_EQ(queue.GetEventCount(), 2);
  EXPECT_EQ(queue.PopEvent(), event1);
  EXPECT_EQ(queue.GetEventCount(), 1);
  EXPECT_EQ(queue.PopEvent(), event2);
  EXPECT_EQ(queue.GetEventCount(), 0);
}

}
