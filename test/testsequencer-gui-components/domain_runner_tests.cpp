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

#include "sequencergui/experimental/domain_runner.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/experimental/domain_events.h>

#include <mvvm/test/mock_callback_listener.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>

#include <gtest/gtest.h>
#include <testutils/standard_procedures.h>
#include <testutils/test_utils.h>

#include <iostream>

using namespace sequencergui;
using ::testing::_;

//! Tests for DomainRunner class.

class DomainRunnerTest : public ::testing::Test
{
public:
  using msec = std::chrono::milliseconds;

  /**
   * @brief Test helper function to print event.
   */
  static std::function<void(const domain_event_t& event)> CreatePrintCallback()
  {
    return [](const domain_event_t& event) { std::cout << ToString(event) << std::endl; };
  }

  mvvm::test::MockCallbackListener<domain_event_t> m_event_listener;
};

TEST_F(DomainRunnerTest, InitialState)
{
  auto procedure = testutils::CreateMessageProcedure("text");
  DomainRunner runner(CreatePrintCallback(), *procedure);
  runner.Start();

  EXPECT_EQ(runner.GetCurrentState(), sup::sequencer::JobState::kInitial);
  EXPECT_FALSE(runner.IsFinished());
}

//! Running short domain procedure that executes normally.

TEST_F(DomainRunnerTest, ShortProcedureThatExecutesNormally)
{
  using ::sup::sequencer::ExecutionStatus;
  using ::sup::sequencer::JobState;

  auto procedure = testutils::CreateMessageProcedure("text");
  auto instruction_ptr = procedure->RootInstruction();

  {
    const ::testing::InSequence seq;

    // triggered by JobController c-tor
    const domain_event_t event1(JobStateChanged{JobState::kInitial});
    EXPECT_CALL(m_event_listener, OnCallback(event1)).Times(1);

    const domain_event_t event2(JobStateChanged{JobState::kRunning});
    EXPECT_CALL(m_event_listener, OnCallback(event2)).Times(1);

    const domain_event_t event3(
        InstructionStatusChanged{instruction_ptr, ExecutionStatus::NOT_FINISHED});
    EXPECT_CALL(m_event_listener, OnCallback(event3)).Times(1);

    const domain_event_t event4(
        InstructionStatusChanged{instruction_ptr, ExecutionStatus::SUCCESS});
    EXPECT_CALL(m_event_listener, OnCallback(event4)).Times(1);

    const domain_event_t event5(JobStateChanged{JobState::kSucceeded});
    EXPECT_CALL(m_event_listener, OnCallback(event5)).Times(1);

    // triggered by JobController d-tor
    const domain_event_t event6(
        InstructionStatusChanged{instruction_ptr, ExecutionStatus::NOT_STARTED});
    EXPECT_CALL(m_event_listener, OnCallback(event6)).Times(1);
  }

  DomainRunner runner(m_event_listener.CreateCallback(), *procedure);
  runner.Start();

  auto final_state = runner.WaitForFinished();
  EXPECT_EQ(final_state, sup::sequencer::JobState::kSucceeded);
}

//! Terminates procedure that runs too long.

TEST_F(DomainRunnerTest, StartAndTerminate)
{
  using ::sup::sequencer::ExecutionStatus;
  using ::sup::sequencer::JobState;
  const std::chrono::milliseconds wait_timeout(10000);

  auto procedure = testutils::CreateSingleWaitProcedure(wait_timeout);
  auto instruction_ptr = procedure->RootInstruction();

  DomainRunner runner(CreatePrintCallback(), *procedure);
  runner.Start();

  EXPECT_TRUE(runner.Start());  // trigger action

  std::this_thread::sleep_for(msec(50));

  auto has_started = [instruction_ptr, &runner]()
  {
    return instruction_ptr->GetStatus() == ExecutionStatus::NOT_FINISHED
           && runner.GetCurrentState() == JobState::kRunning;
  };
  EXPECT_TRUE(testutils::WaitFor(has_started, msec(50)));

  runner.Stop();

  auto has_finished = [instruction_ptr, &runner]() { return runner.IsFinished(); };
  EXPECT_TRUE(testutils::WaitFor(has_finished, msec(50)));

  EXPECT_TRUE(runner.IsFinished());

  EXPECT_EQ(runner.GetCurrentState(), JobState::kFailed);
  // it is FAILURE here (and not NOT_FINISHED) because we have interrupted Wait with the Halt
  std::cout << static_cast<int>(runner.GetCurrentState()) << std::endl;
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);
  EXPECT_EQ(instruction_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);
}
