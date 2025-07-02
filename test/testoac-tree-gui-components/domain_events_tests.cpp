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

#include "oac_tree_gui/jobsystem/domain_events.h"

#include <oac_tree_gui/domain/domain_constants.h>

#include <sup/oac-tree/instruction.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Tests events defined in domain_events.h

class DomainEventTest : public ::testing::Test
{
};

TEST_F(DomainEventTest, Monostate)
{
  const domain_event_t event;
  EXPECT_FALSE(IsValid(event));
}

TEST_F(DomainEventTest, InstructionStateUpdatedEvent)
{
  using ::sup::oac_tree::ExecutionStatus;

  EXPECT_TRUE(IsValid(domain_event_t{InstructionStateUpdatedEvent{}}));

  {  // default constructed
    const InstructionStateUpdatedEvent event1{};
    const InstructionStateUpdatedEvent event2{};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
  }

  {  // state is different
    const sup::oac_tree::InstructionState state1{false, ExecutionStatus::RUNNING};
    const sup::oac_tree::InstructionState state2{false, ExecutionStatus::SUCCESS};
    const InstructionStateUpdatedEvent event1{0, state1};
    const InstructionStateUpdatedEvent event2{0, state1};
    const InstructionStateUpdatedEvent event3{0, state2};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
    EXPECT_FALSE(event1 == event3);
    EXPECT_TRUE(event1 != event3);
  }

  {  // index is different
    const sup::oac_tree::InstructionState state{false, ExecutionStatus::RUNNING};
    const InstructionStateUpdatedEvent event1{0, state};
    const InstructionStateUpdatedEvent event2{1, state};
    EXPECT_FALSE(event1 == event2);
    EXPECT_TRUE(event1 != event2);
  }
}

TEST_F(DomainEventTest, VariableUpdatedEvent)
{
  {  // default constructed
    const VariableUpdatedEvent event1;
    const VariableUpdatedEvent event2;
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
  }

  {  // same id's
    const VariableUpdatedEvent event1{42};
    const VariableUpdatedEvent event2{42};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
  }

  {  // same id's and values
    const VariableUpdatedEvent event1{42, sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    const VariableUpdatedEvent event2{42, sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
  }

  {  // different id's and same values
    const VariableUpdatedEvent event1{42, sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    const VariableUpdatedEvent event2{43, sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    EXPECT_FALSE(event1 == event2);
    EXPECT_TRUE(event1 != event2);
  }

  {  // same id's and different values
    const VariableUpdatedEvent event1{42, sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}};
    const VariableUpdatedEvent event2{42, sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43}};
    EXPECT_FALSE(event1 == event2);
    EXPECT_TRUE(event1 != event2);
  }

  {  // same id's and values, different is_available
    const VariableUpdatedEvent event1{42, sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42},
                                      false};
    const VariableUpdatedEvent event2{42, sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42},
                                      true};
    EXPECT_FALSE(event1 == event2);
    EXPECT_TRUE(event1 != event2);
  }
}

TEST_F(DomainEventTest, JobStateChangedEvent)
{
  EXPECT_TRUE(IsValid(domain_event_t{JobStateChangedEvent{}}));

  {  // default constructed
    const JobStateChangedEvent event1{};
    const JobStateChangedEvent event2{};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
  }

  {  // names
    const JobStateChangedEvent event1{::sup::oac_tree::JobState::kInitial};
    const JobStateChangedEvent event2{::sup::oac_tree::JobState::kInitial};
    const JobStateChangedEvent event3{::sup::oac_tree::JobState::kRunning};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
    EXPECT_FALSE(event1 == event3);
    EXPECT_TRUE(event1 != event3);
  }
}

TEST_F(DomainEventTest, ActiveInstructionChangedEvent)
{
  using ::sup::oac_tree::ExecutionStatus;

  {  // default constructed
    const ActiveInstructionChangedEvent event1{};
    const ActiveInstructionChangedEvent event2{};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
  }

  {  // instruction
    const ActiveInstructionChangedEvent event1{{1}};
    const ActiveInstructionChangedEvent event2{{1}};
    const ActiveInstructionChangedEvent event3{{2}};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
    EXPECT_FALSE(event1 == event3);
    EXPECT_TRUE(event1 != event3);
  }
}

TEST_F(DomainEventTest, BreakpointHitEvent)
{
  using ::sup::oac_tree::ExecutionStatus;

  {  // default constructed
    const BreakpointHitEvent event1{};
    const BreakpointHitEvent event2{};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
  }

  {  // instruction
    const BreakpointHitEvent event1{1U};
    const BreakpointHitEvent event2{1U};
    const BreakpointHitEvent event3{2U};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
    EXPECT_FALSE(event1 == event3);
    EXPECT_TRUE(event1 != event3);
  }
}

}  // namespace oac_tree_gui::test
