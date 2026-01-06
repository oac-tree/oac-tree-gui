/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "oac_tree_gui/jobsystem/domain_job_observer.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/domain_event_helper.h>
#include <oac_tree_gui/jobsystem/user_context.h>

#include <sup/oac-tree/execution_status.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for DomainJobObserver class.
 */
class DomainJobObserverTest : public ::testing::Test
{
public:
  testing::MockFunction<void(const domain_event_t&)> m_event_listener;
};

TEST_F(DomainJobObserverTest, InitialState)
{
  EXPECT_THROW(DomainJobObserver({}, {}), RuntimeException);
}

TEST_F(DomainJobObserverTest, OnStateChange)
{
  using ::sup::oac_tree::ExecutionStatus;
  using ::sup::oac_tree::InstructionState;

  DomainJobObserver observer(m_event_listener.AsStdFunction(), {});

  const domain_event_t expected_event(
      InstructionStateUpdatedEvent{0, InstructionState{false, ExecutionStatus::NOT_STARTED}});

  EXPECT_CALL(m_event_listener, Call(expected_event)).Times(1);

  observer.InstructionStateUpdated(0, InstructionState{false, ExecutionStatus::NOT_STARTED});
}

TEST_F(DomainJobObserverTest, ActiveInstructionUpdated)
{
  using ::sup::oac_tree::ExecutionStatus;
  using ::sup::oac_tree::InstructionState;

  DomainJobObserver observer(m_event_listener.AsStdFunction(), {});

  const domain_event_t expected_event1(
      InstructionStateUpdatedEvent{0, InstructionState{false, ExecutionStatus::NOT_STARTED}});
  const domain_event_t expected_event2(
      InstructionStateUpdatedEvent{0, InstructionState{false, ExecutionStatus::NOT_FINISHED}});
  const domain_event_t expected_event3(
      ActiveInstructionChangedEvent{std::vector<sup::dto::uint32>({0})});

  EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
  EXPECT_CALL(m_event_listener, Call(expected_event2)).Times(1);
  EXPECT_CALL(m_event_listener, Call(expected_event3)).Times(1);

  observer.InstructionStateUpdated(0, InstructionState{false, ExecutionStatus::NOT_STARTED});
  observer.InstructionStateUpdated(0, InstructionState{false, ExecutionStatus::NOT_FINISHED});
}

TEST_F(DomainJobObserverTest, ActiveInstructionUpdatedWhenMonitorIsMuted)
{
  using ::sup::oac_tree::ExecutionStatus;
  using ::sup::oac_tree::InstructionState;

  DomainJobObserver observer(m_event_listener.AsStdFunction(), {});

  observer.SetInstructionActiveFilter(CreateInstructionMuteAllFilter());

  const domain_event_t expected_event1(
      InstructionStateUpdatedEvent{0, InstructionState{false, ExecutionStatus::NOT_STARTED}});
  const domain_event_t expected_event2(
      InstructionStateUpdatedEvent{0, InstructionState{false, ExecutionStatus::NOT_FINISHED}});
  // empty list of active instructions will be reported
  const domain_event_t expected_event3(
      ActiveInstructionChangedEvent{std::vector<sup::dto::uint32>({})});

  EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
  EXPECT_CALL(m_event_listener, Call(expected_event2)).Times(1);
  EXPECT_CALL(m_event_listener, Call(expected_event3)).Times(1);

  observer.InstructionStateUpdated(0, InstructionState{false, ExecutionStatus::NOT_STARTED});
  observer.InstructionStateUpdated(0, InstructionState{false, ExecutionStatus::NOT_FINISHED});
}

}  // namespace oac_tree_gui::test
