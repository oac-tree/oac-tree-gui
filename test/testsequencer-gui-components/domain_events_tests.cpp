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

#include "sequencergui/experimental/domain_events.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>

#include <sup/sequencer/instruction.h>

#include <gtest/gtest.h>

using namespace sequencergui::experimental;

//! Tests events defined in domain_events.h

class DomainEventTest : public ::testing::Test
{
};

TEST_F(DomainEventTest, Monostate)
{
  domain_event_t event;
  EXPECT_FALSE(IsValid(event));
}

TEST_F(DomainEventTest, InstructionStatusChanged)
{
  using ::sup::sequencer::ExecutionStatus;

  EXPECT_TRUE(IsValid(domain_event_t{InstructionStatusChanged{}}));

  {  // default constructed
    const InstructionStatusChanged event1{};
    const InstructionStatusChanged event2{};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
  }

  {  // names
    const InstructionStatusChanged event1{nullptr, ExecutionStatus::RUNNING};
    const InstructionStatusChanged event2{nullptr, ExecutionStatus::RUNNING};
    const InstructionStatusChanged event3{nullptr, ExecutionStatus::SUCCESS};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
    EXPECT_FALSE(event1 == event3);
    EXPECT_TRUE(event1 != event3);
  }

  {  // instruction
    auto instr1 = ::sequencergui::CreateDomainInstruction(
        ::sequencergui::domainconstants::kWaitInstructionType);
    auto instr2 = ::sequencergui::CreateDomainInstruction(
        ::sequencergui::domainconstants::kWaitInstructionType);
    const InstructionStatusChanged event1{instr1.get(), ExecutionStatus::RUNNING};
    const InstructionStatusChanged event2{instr1.get(), ExecutionStatus::RUNNING};
    const InstructionStatusChanged event3{instr2.get(), ExecutionStatus::RUNNING};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
    EXPECT_FALSE(event1 == event3);
    EXPECT_TRUE(event1 != event3);
  }
}

TEST_F(DomainEventTest, JobStatusChanged)
{
  EXPECT_TRUE(IsValid(domain_event_t{JobStateChanged{}}));

  {  // default constructed
    const JobStateChanged event1{};
    const JobStateChanged event2{};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
  }

  {  // names
    const JobStateChanged event1{::sup::sequencer::JobState::kInitial};
    const JobStateChanged event2{::sup::sequencer::JobState::kInitial};
    const JobStateChanged event3{::sup::sequencer::JobState::kRunning};
    EXPECT_TRUE(event1 == event2);
    EXPECT_FALSE(event1 != event2);
    EXPECT_FALSE(event1 == event3);
    EXPECT_TRUE(event1 != event3);
  }
}
