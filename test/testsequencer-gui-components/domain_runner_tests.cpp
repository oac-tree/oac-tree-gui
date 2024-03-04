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
#include <sup/sequencer/variable.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <testutils/mock_domain_event_listener.h>
#include <testutils/standard_procedures.h>
#include <testutils/test_utils.h>

using namespace sequencergui;
using ::testing::_;
using ::testing::AtLeast;

//! Tests for DomainRunner class.

class DomainRunnerTest : public ::testing::Test
{
public:
  using clock_used = std::chrono::high_resolution_clock;
  using time_t = std::chrono::time_point<clock_used>;
  using duration_unit = std::chrono::milliseconds;
  using msec = std::chrono::milliseconds;

  /**
   * @brief Test helper function to create callbacks to print events.
   */
  static std::function<void(const domain_event_t& event)> CreatePrintCallback()
  {
    return [](const domain_event_t& event) { std::cout << ToString(event) << std::endl; };
  }

  /**
   * @brief Test helper function to create callbacks that do nothing.
   */
  static std::function<void(const domain_event_t& event)> CreateNoopCallback()
  {
    return [](const domain_event_t& event) { (void)event; };
  }

  mvvm::test::MockCallbackListener<domain_event_t> m_event_listener;
};

TEST_F(DomainRunnerTest, InitialState)
{
  auto procedure = testutils::CreateMessageProcedure("text");
  DomainRunner runner(CreateNoopCallback(), *procedure);
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
  EXPECT_EQ(runner.GetCurrentState(), sup::sequencer::JobState::kInitial);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  runner.Start();

  auto final_state = runner.WaitForFinished();
  EXPECT_EQ(final_state, sup::sequencer::JobState::kSucceeded);
  EXPECT_EQ(runner.GetCurrentState(), sup::sequencer::JobState::kSucceeded);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);
}

//! Terminates procedure that runs too long.

TEST_F(DomainRunnerTest, StartAndTerminate)
{
  using ::sup::sequencer::ExecutionStatus;
  using ::sup::sequencer::JobState;
  const std::chrono::milliseconds wait_timeout(10000);

  auto procedure = testutils::CreateSingleWaitProcedure(wait_timeout);
  auto instruction_ptr = procedure->RootInstruction();

  DomainRunner runner(CreateNoopCallback(), *procedure);

  EXPECT_TRUE(runner.Start());  // trigger action

  std::this_thread::sleep_for(msec(50));

  auto has_started = [instruction_ptr, &runner]()
  {
    return instruction_ptr->GetStatus() == ExecutionStatus::NOT_FINISHED
           && runner.GetCurrentState() == JobState::kRunning;
  };
  EXPECT_TRUE(testutils::WaitFor(has_started, msec(50)));

  runner.Stop();

  auto has_finished = [&runner]() { return runner.IsFinished(); };
  EXPECT_TRUE(testutils::WaitFor(has_finished, msec(50)));

  EXPECT_TRUE(runner.IsFinished());

  EXPECT_EQ(runner.GetCurrentState(), JobState::kFailed);
  // it is FAILURE here (and not NOT_FINISHED) because we have interrupted Wait with the Halt
  std::cout << static_cast<int>(runner.GetCurrentState()) << std::endl;
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);
  EXPECT_EQ(instruction_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);
}

//! Sequence with single message in normal start mode. Validating that tick timeout is ignored for
//! a single instructions.

TEST_F(DomainRunnerTest, SequenceWithSingleMessage)
{
  const int tick_timeout_msec(1000);  // intentionally long timeout

  auto procedure = testutils::CreateSequenceWithSingleMessageProcedure();
  DomainRunner runner(CreateNoopCallback(), *procedure);

  runner.SetTickTimeout(tick_timeout_msec);

  const time_t start_time = clock_used::now();

  // triggering action
  EXPECT_TRUE(runner.Start());

  auto has_finished = [&runner]() { return runner.IsFinished(); };
  EXPECT_TRUE(testutils::WaitFor(has_finished, msec(50)));

  EXPECT_EQ(runner.GetCurrentState(), sup::sequencer::JobState::kSucceeded);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

  const time_t end_time = clock_used::now();

  // Here we test that adapter.SetTickTimeout(1000) doesn't influence execution time,
  // since we have only one child that gets executed during single step.
  EXPECT_TRUE(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
              < msec(tick_timeout_msec));
}

//! Sequence with two messages in normal start mode. Additional tick timeout slows down the
//! execution.
TEST_F(DomainRunnerTest, SequenceWithTwoMessages)
{
  const int tick_timeout_msec(50);

  auto procedure = testutils::CreateSequenceWithTwoMessagesProcedure();
  DomainRunner runner(CreateNoopCallback(), *procedure);

  runner.SetTickTimeout(tick_timeout_msec);

  const time_t start_time = clock_used::now();

  // triggering action
  EXPECT_TRUE(runner.Start());

  auto has_finished = [&runner]() { return runner.IsFinished(); };
  EXPECT_TRUE(testutils::WaitFor(has_finished, msec(200)));

  EXPECT_EQ(runner.GetCurrentState(), sup::sequencer::JobState::kSucceeded);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

  const time_t end_time = clock_used::now();

  // Here we test that adapter.SetTickTimeout(100) was invoked once
  EXPECT_TRUE(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
              > msec(tick_timeout_msec));
}

//! Sequence with two waits in step mode. Making steps until complete.
TEST_F(DomainRunnerTest, SequenceWithTwoWaitsInStepMode)
{
  using ::sup::sequencer::ExecutionStatus;
  using ::sup::sequencer::JobState;

  const msec wait_time(10);   // parameter for Wait instruction
  const msec safety_gap(50);  // some additional waiting time safety gap

  // wait time after each step for procedure containing two Wait instructions
  const msec max_after_step_wait_time(2 * (testutils::kDefaultWaitPrecision + wait_time)
                                      + safety_gap);

  auto procedure = testutils::CreateSequenceWithTwoWaitsProcedure(wait_time, wait_time);

  auto sequence_ptr = procedure->RootInstruction();
  auto wait0_ptr = sequence_ptr->ChildInstructions().at(0);
  auto wait1_ptr = sequence_ptr->ChildInstructions().at(1);

  {
    const ::testing::InSequence seq;

    const domain_event_t event0(JobStateChanged{JobState::kInitial});
    EXPECT_CALL(m_event_listener, OnCallback(event0)).Times(1);

    const domain_event_t event1(JobStateChanged{JobState::kStepping});
    EXPECT_CALL(m_event_listener, OnCallback(event1)).Times(1);

    const domain_event_t event2(
        InstructionStatusChanged{sequence_ptr, ExecutionStatus::NOT_FINISHED});
    EXPECT_CALL(m_event_listener, OnCallback(event2)).Times(1);

    const domain_event_t event3(InstructionStatusChanged{wait0_ptr, ExecutionStatus::NOT_FINISHED});
    EXPECT_CALL(m_event_listener, OnCallback(event3)).Times(1);

    const domain_event_t event4(InstructionStatusChanged{wait0_ptr, ExecutionStatus::SUCCESS});
    EXPECT_CALL(m_event_listener, OnCallback(event4)).Times(1);

    const domain_event_t event5(JobStateChanged{JobState::kPaused});
    EXPECT_CALL(m_event_listener, OnCallback(event5)).Times(1);
  }

  DomainRunner runner(m_event_listener.CreateCallback(), *procedure);

  EXPECT_EQ(runner.GetCurrentState(), sup::sequencer::JobState::kInitial);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  // Step #1 right from initial state
  EXPECT_TRUE(runner.Step());

  auto is_paused = [&runner]() { return runner.GetCurrentState() == JobState::kPaused; };
  EXPECT_TRUE(testutils::WaitFor(is_paused, msec(max_after_step_wait_time)));

  {
    const ::testing::InSequence seq;

    const domain_event_t event1(JobStateChanged{JobState::kStepping});
    EXPECT_CALL(m_event_listener, OnCallback(event1)).Times(1);

    const domain_event_t event2(InstructionStatusChanged{wait1_ptr, ExecutionStatus::NOT_FINISHED});
    EXPECT_CALL(m_event_listener, OnCallback(event2)).Times(1);

    const domain_event_t event3(InstructionStatusChanged{wait1_ptr, ExecutionStatus::SUCCESS});
    EXPECT_CALL(m_event_listener, OnCallback(event3)).Times(1);

    const domain_event_t event4(InstructionStatusChanged{sequence_ptr, ExecutionStatus::SUCCESS});
    EXPECT_CALL(m_event_listener, OnCallback(event4)).Times(1);

    const domain_event_t event5(JobStateChanged{JobState::kSucceeded});
    EXPECT_CALL(m_event_listener, OnCallback(event5)).Times(1);
  }

  // Step #2 and waiting till the end
  EXPECT_TRUE(runner.Step());

  auto has_succeeded = [&runner]() { return runner.GetCurrentState() == JobState::kSucceeded; };
  EXPECT_TRUE(testutils::WaitFor(has_succeeded, max_after_step_wait_time));

  // the rest will listen for activity during JobController destruction
  {
    const ::testing::InSequence seq;

    const domain_event_t event1(InstructionStatusChanged{wait0_ptr, ExecutionStatus::NOT_STARTED});
    EXPECT_CALL(m_event_listener, OnCallback(event1)).Times(1);

    const domain_event_t event2(InstructionStatusChanged{wait1_ptr, ExecutionStatus::NOT_STARTED});
    EXPECT_CALL(m_event_listener, OnCallback(event2)).Times(1);

    const domain_event_t event3(
        InstructionStatusChanged{sequence_ptr, ExecutionStatus::NOT_STARTED});
    EXPECT_CALL(m_event_listener, OnCallback(event3)).Times(1);
  }
}

//! Sequence with two messages in a step mode. After first step it is interrupted.

TEST_F(DomainRunnerTest, SequenceWithTwoWaitsInStepModeInterrupted)
{
  using ::sup::sequencer::ExecutionStatus;
  using ::sup::sequencer::JobState;

  auto procedure = testutils::CreateSequenceWithTwoMessagesProcedure();

  DomainRunner runner(CreateNoopCallback(), *procedure);
  EXPECT_EQ(runner.GetCurrentState(), sup::sequencer::JobState::kInitial);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  runner.Step();

  auto has_paused = [&runner]() { return runner.GetCurrentState() == JobState::kPaused; };
  EXPECT_TRUE(testutils::WaitFor(has_paused, msec(50)));

  EXPECT_TRUE(runner.Stop());

  runner.WaitForFinished();

  EXPECT_EQ(runner.GetCurrentState(), sup::sequencer::JobState::kHalted);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_FINISHED);
}

//! Repeat procedure with increment instruction inside. We start in step mode, and after the first
//! step continue till the end without interruptions.

TEST_F(DomainRunnerTest, StepAndRunTillTheEnd)
{
  testutils::MockDomainEventListener listener;

  auto procedure = testutils::CreateCounterProcedure(3);
  auto variable = procedure->GetWorkspace().GetVariable("counter");
  procedure->Setup();

  sup::dto::AnyValue counter_value;
  variable->GetValue(counter_value);
  EXPECT_EQ(counter_value, 0U);

  // JobState: initial, stepping, paused
  EXPECT_CALL(listener, OnJobStateChanged(_)).Times(3);
  // Instruction: repeat (not finished) + one increment (not finished, success)
  EXPECT_CALL(listener, OnInstructionStatusChanged(_)).Times(3);

  DomainRunner runner(listener.CreateCallback(), *procedure);

  // making step
  EXPECT_TRUE(runner.Step());

  auto is_incremented = [variable]()
  {
    sup::dto::AnyValue counter_value;
    variable->GetValue(counter_value);
    return counter_value == 1U;
  };

  EXPECT_TRUE(testutils::WaitFor(is_incremented, msec(50)));

  // JobState: running, success
  EXPECT_CALL(listener, OnJobStateChanged(_)).Times(2);
  // Instruction: two increments 2*(not started, not finished, success) + repeat (success)
  EXPECT_CALL(listener, OnInstructionStatusChanged(_)).Times(7);

  // continuing till the end
  EXPECT_TRUE(runner.Start());

  runner.WaitForFinished();

  EXPECT_EQ(runner.GetCurrentState(), sup::sequencer::JobState::kSucceeded);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

  variable->GetValue(counter_value);
  EXPECT_EQ(counter_value, 3U);

  // destruction of jobController, two instructions (not_started)
  EXPECT_CALL(listener, OnInstructionStatusChanged(_)).Times(2);
}

//! Repeat procedure with increment instruction for infinite repeat for run-pause-run scenario.
//!
//! - Repeat
//!   - Sequence
//!     - Compare
//!     - Increment
//!
//! We start in run mode, then pause, then push run again to continue till the end. The procedure
//! contains two variables: a counter for increment, and interruption variable to quit from the
//! repeat. Test scenario:
//! - Run
//! - Pause
//! - Run again, wait a bit, and stop via interrupt variable.

TEST_F(DomainRunnerTest, RunPauseRun)
{
  using ::sup::sequencer::ExecutionStatus;
  using ::sup::sequencer::JobState;

  // The procedure contains two variables: a counter, and variable for interruption. By magic we
  // know variable names.
  auto procedure = testutils::CreateRepeatIncrementAndCompare();
  const std::string kCounterVarName("counter");
  const std::string kContinueVarName("to_continue");
  auto& workspace = procedure->GetWorkspace();
  auto counter_var = workspace.GetVariable(kCounterVarName);

  procedure->Setup();

  // counter is 0 at the beginning
  sup::dto::AnyValue counter_value;
  counter_var->GetValue(counter_value);
  EXPECT_EQ(counter_value, 0U);

  DomainRunner runner(CreateNoopCallback(), *procedure);

  // starting and waiting a bit to let the counter to increment
  EXPECT_TRUE(runner.Start());

  auto is_incremented = [counter_var]()
  {
    sup::dto::AnyValue counter_value;
    counter_var->GetValue(counter_value);
    return counter_value.As<int>() > 1U;
  };
  EXPECT_TRUE(testutils::WaitFor(is_incremented, msec(50)));

  // pause
  EXPECT_TRUE(runner.Pause());
  auto is_paused = [&runner]() { return runner.GetCurrentState() == JobState::kPaused; };
  EXPECT_TRUE(testutils::WaitFor(is_paused, msec(50)));

  // It is paused now, increment counter should increase.
  counter_var->GetValue(counter_value);
  auto current_counter1 = counter_value.As<int>();
  EXPECT_TRUE(current_counter1 > 1);

  // continuing till the end
  EXPECT_TRUE(runner.Start());

  auto is_running = [&runner]() { return runner.GetCurrentState() == JobState::kRunning; };
  EXPECT_TRUE(testutils::WaitFor(is_running, msec(50)));

  std::this_thread::sleep_for(msec(25));

  // to stop incrementing use interruption variable
  sup::dto::AnyValue interrupt_value(0U);
  procedure->GetWorkspace().SetValue("to_continue", interrupt_value);

  std::this_thread::sleep_for(msec(25));

  runner.WaitForFinished();

  counter_var->GetValue(counter_value);
  auto current_counter2 = counter_value.As<int>();
  EXPECT_TRUE(current_counter2 > current_counter1);

  // it is failed because we've made a sequence quite
  EXPECT_EQ(runner.GetCurrentState(), sup::sequencer::JobState::kFailed);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);
}
