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

#include "sequencergui/jobsystem/local_domain_runner.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/domain_events.h>
#include <sequencergui/jobsystem/user_context.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/workspace.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_domain_event_listener.h>
#include <testutils/mock_user_context.h>
#include <testutils/standard_procedures.h>
#include <testutils/test_utils.h>

#include <QTest>
#include <thread>

using namespace sequencergui;
using ::testing::_;
using ::testing::AtLeast;

/**
 * @brief Tests for LocalDomainRunner class.
 */
class LocalDomainRunnerTest : public ::testing::Test
{
public:
  using mock_event_listener_t = ::testing::StrictMock<testutils::MockDomainEventListener>;
  using mock_user_listener_t = ::testing::StrictMock<testutils::MockUserContext>;

  using clock_used = std::chrono::high_resolution_clock;
  using time_t = std::chrono::time_point<clock_used>;
  using msec = std::chrono::milliseconds;

  /**
   * @brief Creates runner for given procedure.
   *
   * @param listen_callbacks Lsiten for event queue and user interaction callbacks.
   */
  std::unique_ptr<LocalDomainRunner> CreateRunner(std::unique_ptr<procedure_t> procedure,
                                                  bool listen_callbacks = true)
  {
    if (listen_callbacks)
    {
      return std::make_unique<LocalDomainRunner>(m_event_listener.CreateDispatcherContext(),
                                                 m_user_listener.CreateContext(),
                                                 std::move(procedure));
    }

    return std::make_unique<LocalDomainRunner>(DomainEventDispatcherContext{}, UserContext{},
                                               std::move(procedure));
  }

  /**
   * @brief Wait for runner receives the given state, and let all queued signals reach their
   * destinations.
   *
   * Provides additional waiting to complete all queued signals. The method shall be used when we
   * test the runner and are interested to receive and process all mock notifications.
   */
  bool WaitForStateAndEmptyQueue(const LocalDomainRunner& runner,
                                 sup::sequencer::JobState job_state, msec msec_value)
  {
    auto predicate = [this, &runner, job_state]()
    { return runner.GetJobState() == job_state && runner.GetEventCount() == 0; };
    return QTest::qWaitFor(predicate, static_cast<int>(msec_value.count()));
  }

  mock_event_listener_t m_event_listener;
  mock_user_listener_t m_user_listener;
};

TEST_F(LocalDomainRunnerTest, InitialState)
{
  const auto runner = CreateRunner(testutils::CreateMessageProcedure("text"));

  EXPECT_EQ(runner->GetJobState(), sup::sequencer::JobState::kInitial);
  EXPECT_FALSE(runner->IsFinished());
  EXPECT_FALSE(runner->IsBusy());
}

//! Running short domain procedure that executes normally.
TEST_F(LocalDomainRunnerTest, ShortProcedureThatExecutesNormally)
{
  using ::sup::sequencer::ExecutionStatus;
  using ::sup::sequencer::InstructionState;
  using ::sup::sequencer::JobState;

  auto procedure = testutils::CreateMessageProcedure("text");
  auto procedure_ptr = procedure.get();
  auto instruction_ptr = procedure_ptr->RootInstruction();
  const sup::dto::uint32 instruction_index{0};

  {
    const ::testing::InSequence seq;

    // triggered by JobController c-tor
    const JobStateChangedEvent event1{JobState::kInitial};
    EXPECT_CALL(m_event_listener, OnJobStateChanged(event1)).Times(1);

    const JobStateChangedEvent event2{JobState::kRunning};
    EXPECT_CALL(m_event_listener, OnJobStateChanged(event2)).Times(1);

    const InstructionStateUpdatedEvent event3{
        instruction_index, InstructionState{false, ExecutionStatus::NOT_FINISHED}};
    EXPECT_CALL(m_event_listener, OnInstructionStateUpdated(event3)).Times(1);

    // message instruction (too difficult to make proper comparison because of time stamp)
    EXPECT_CALL(m_event_listener, OnLogEvent(_)).Times(1);

    const InstructionStateUpdatedEvent event5{instruction_index,
                                              InstructionState{false, ExecutionStatus::SUCCESS}};
    EXPECT_CALL(m_event_listener, OnInstructionStateUpdated(event5)).Times(1);

    const JobStateChangedEvent event6{JobState::kSucceeded};
    EXPECT_CALL(m_event_listener, OnJobStateChanged(event6)).Times(1);

    const NextLeavesChangedEvent event7;
    EXPECT_CALL(m_event_listener, OnNextLeavesChanged(event7)).Times(1);
  }

  auto runner = CreateRunner(std::move(procedure));
  EXPECT_EQ(runner->GetJobState(), sup::sequencer::JobState::kInitial);
  EXPECT_EQ(procedure_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  runner->Start();

  WaitForStateAndEmptyQueue(*runner, sup::sequencer::JobState::kSucceeded, msec(100));

  auto final_state = runner->WaitForFinished();
  EXPECT_EQ(final_state, sup::sequencer::JobState::kSucceeded);
  EXPECT_EQ(runner->GetJobState(), sup::sequencer::JobState::kSucceeded);
  EXPECT_EQ(procedure_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);
  EXPECT_TRUE(runner->IsFinished());
  EXPECT_FALSE(runner->IsBusy());
}

//! Running short domain procedure till the end, and then reset.
TEST_F(LocalDomainRunnerTest, Reset)
{
  using ::sup::sequencer::ExecutionStatus;
  using ::sup::sequencer::JobState;

  auto procedure = testutils::CreateMessageProcedure("text");
  auto procedure_ptr = procedure.get();
  auto instruction_ptr = procedure_ptr->RootInstruction();

  auto runner = CreateRunner(std::move(procedure));
  EXPECT_EQ(runner->GetJobState(), sup::sequencer::JobState::kInitial);
  EXPECT_EQ(procedure_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);
  EXPECT_EQ(instruction_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  runner->Start();

  auto final_state = runner->WaitForFinished();
  EXPECT_EQ(final_state, sup::sequencer::JobState::kSucceeded);
  EXPECT_EQ(runner->GetJobState(), sup::sequencer::JobState::kSucceeded);
  EXPECT_EQ(procedure_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);
  EXPECT_EQ(instruction_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

  EXPECT_TRUE(runner->IsFinished());
  EXPECT_FALSE(runner->IsBusy());

  runner->Reset();

  // EXPECT_TRUE(runner->WaitForState(sup::sequencer::JobState::kInitial,
  //                                  testutils::GetTimeoutInSec(msec(100))));
  auto has_finished = [&runner]()
  { return runner->GetJobState() == sup::sequencer::JobState::kInitial; };
  EXPECT_TRUE(testutils::WaitFor(has_finished, msec(50)));

  EXPECT_EQ(runner->GetJobState(), sup::sequencer::JobState::kInitial);
  EXPECT_EQ(procedure_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);
  EXPECT_EQ(instruction_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);
}

//! Terminates procedure that runs too long.
TEST_F(LocalDomainRunnerTest, StartAndTerminate)
{
  using ::sup::sequencer::ExecutionStatus;
  using ::sup::sequencer::JobState;
  const std::chrono::milliseconds wait_timeout(10000);

  auto procedure = testutils::CreateSingleWaitProcedure(wait_timeout);
  auto procedure_ptr = procedure.get();
  auto instruction_ptr = procedure_ptr->RootInstruction();

  auto runner = CreateRunner(std::move(procedure), /*listen_callbacks*/ false);

  EXPECT_TRUE(runner->Start());  // trigger action

  std::this_thread::sleep_for(msec(50));

  auto has_started = [instruction_ptr, &runner]()
  {
    return instruction_ptr->GetStatus() == ExecutionStatus::NOT_FINISHED
           && runner->GetJobState() == JobState::kRunning;
  };
  EXPECT_TRUE(testutils::WaitFor(has_started, msec(50)));

  EXPECT_FALSE(runner->IsFinished());
  EXPECT_TRUE(runner->IsBusy());

  runner->Stop();

  auto is_finished = [&runner]() { return runner->IsFinished(); };
  EXPECT_TRUE(testutils::WaitFor(is_finished, msec(100)));

  EXPECT_TRUE(runner->IsFinished());

  EXPECT_EQ(runner->GetJobState(), JobState::kFailed);
  // it is FAILURE here (and not NOT_FINISHED) because we have interrupted Wait with the Halt
  EXPECT_EQ(procedure_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);
  EXPECT_EQ(instruction_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);
}

//! Sequence with single message in normal start mode. Validating that tick timeout is ignored for
//! a single instructions.
TEST_F(LocalDomainRunnerTest, SequenceWithSingleMessage)
{
  const int tick_timeout_msec(1000);  // intentionally long timeout

  auto procedure = testutils::CreateSequenceWithSingleMessageProcedure();
  auto procedure_ptr = procedure.get();
  auto runner = CreateRunner(std::move(procedure), /*listen_callbacks*/ false);

  runner->SetTickTimeout(tick_timeout_msec);

  const time_t start_time = clock_used::now();

  // triggering action
  EXPECT_TRUE(runner->Start());

  auto has_finished = [&runner]() { return runner->IsFinished(); };
  EXPECT_TRUE(testutils::WaitFor(has_finished, msec(50)));

  EXPECT_EQ(runner->GetJobState(), sup::sequencer::JobState::kSucceeded);
  EXPECT_EQ(procedure_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

  const time_t end_time = clock_used::now();

  // Here we test that runner.SetTickTimeout(1000) doesn't influence execution time,
  // since we have only one child that gets executed during single step.
  EXPECT_TRUE(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
              < msec(tick_timeout_msec));
}

//! Sequence with two messages in normal start mode. Additional tick timeout slows down the
//! execution.
TEST_F(LocalDomainRunnerTest, SequenceWithTwoMessages)
{
  const int tick_timeout_msec(50);

  auto procedure = testutils::CreateSequenceWithTwoMessagesProcedure();
  auto procedure_ptr = procedure.get();
  auto runner = CreateRunner(std::move(procedure), /*listen_callbacks*/ false);

  runner->SetTickTimeout(tick_timeout_msec);

  const time_t start_time = clock_used::now();

  // triggering action
  EXPECT_TRUE(runner->Start());

  auto has_finished = [&runner]() { return runner->IsFinished(); };
  EXPECT_TRUE(testutils::WaitFor(has_finished, msec(200)));

  EXPECT_EQ(runner->GetJobState(), sup::sequencer::JobState::kSucceeded);
  EXPECT_EQ(procedure_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

  const time_t end_time = clock_used::now();

  // Here we test that tick timeout of the runner was invoked at least once.
  EXPECT_TRUE(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
              >= msec(tick_timeout_msec));
}

//! Sequence with two waits in step mode. Making steps until complete.
TEST_F(LocalDomainRunnerTest, SequenceWithTwoWaitsInStepMode)
{
  using ::sup::sequencer::ExecutionStatus;
  using ::sup::sequencer::InstructionState;
  using ::sup::sequencer::JobState;

  const msec wait_time(10);   // parameter for Wait instruction
  const msec safety_gap(50);  // some additional waiting time safety gap

  // wait time after each step for procedure containing two Wait instructions
  const msec max_after_step_wait_time(2 * (testutils::kDefaultWaitPrecision + wait_time)
                                      + safety_gap);

  auto procedure = testutils::CreateSequenceWithTwoWaitsProcedure(wait_time, wait_time);
  auto procedure_ptr = procedure.get();

  const sup::dto::uint32 sequence_index{0};
  const sup::dto::uint32 wait0_index{1};
  const sup::dto::uint32 wait1_index{2};

  {
    const ::testing::InSequence seq;

    const JobStateChangedEvent event0{JobState::kInitial};
    EXPECT_CALL(m_event_listener, OnJobStateChanged(event0)).Times(1);

    const JobStateChangedEvent event1{JobState::kStepping};
    EXPECT_CALL(m_event_listener, OnJobStateChanged(event1)).Times(1);

    const InstructionStateUpdatedEvent event2{
        sequence_index, InstructionState{false, ExecutionStatus::NOT_FINISHED}};
    EXPECT_CALL(m_event_listener, OnInstructionStateUpdated(event2)).Times(1);

    const InstructionStateUpdatedEvent event3{
        wait0_index, InstructionState{false, ExecutionStatus::NOT_FINISHED}};
    EXPECT_CALL(m_event_listener, OnInstructionStateUpdated(event3)).Times(1);

    const InstructionStateUpdatedEvent event4{wait0_index,
                                              InstructionState{false, ExecutionStatus::SUCCESS}};
    EXPECT_CALL(m_event_listener, OnInstructionStateUpdated(event4)).Times(1);

    const NextLeavesChangedEvent event4a{std::vector<sup::dto::uint32>({wait1_index})};
    EXPECT_CALL(m_event_listener, OnNextLeavesChanged(event4a)).Times(1);

    const JobStateChangedEvent event5{JobState::kPaused};
    EXPECT_CALL(m_event_listener, OnJobStateChanged(event5)).Times(1);
  }

  auto runner = CreateRunner(std::move(procedure));

  EXPECT_EQ(runner->GetJobState(), sup::sequencer::JobState::kInitial);
  EXPECT_EQ(procedure_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  // Step #1 right from initial state
  EXPECT_TRUE(runner->Step());

  WaitForStateAndEmptyQueue(*runner, JobState::kPaused, max_after_step_wait_time);

  EXPECT_FALSE(runner->IsFinished());
  EXPECT_TRUE(runner->IsBusy());

  {
    const ::testing::InSequence seq;

    const JobStateChangedEvent event1{JobState::kStepping};
    EXPECT_CALL(m_event_listener, OnJobStateChanged(event1)).Times(1);

    const InstructionStateUpdatedEvent event2{
        wait1_index, InstructionState{false, ExecutionStatus::NOT_FINISHED}};
    EXPECT_CALL(m_event_listener, OnInstructionStateUpdated(event2)).Times(1);

    const InstructionStateUpdatedEvent event3{wait1_index,
                                              InstructionState{false, ExecutionStatus::SUCCESS}};
    EXPECT_CALL(m_event_listener, OnInstructionStateUpdated(event3)).Times(1);

    const InstructionStateUpdatedEvent event4{sequence_index,
                                              InstructionState{false, ExecutionStatus::SUCCESS}};
    EXPECT_CALL(m_event_listener, OnInstructionStateUpdated(event4)).Times(1);

    const NextLeavesChangedEvent event4a;
    EXPECT_CALL(m_event_listener, OnNextLeavesChanged(event4a)).Times(1);

    const JobStateChangedEvent event5{JobState::kSucceeded};
    EXPECT_CALL(m_event_listener, OnJobStateChanged(event5)).Times(1);
  }

  // Step #2 and waiting till the end
  EXPECT_TRUE(runner->Step());

  WaitForStateAndEmptyQueue(*runner, JobState::kSucceeded, max_after_step_wait_time);
}

//! Sequence with two messages in a step mode. After first step it is interrupted.
TEST_F(LocalDomainRunnerTest, SequenceWithTwoWaitsInStepModeInterrupted)
{
  using ::sup::sequencer::ExecutionStatus;
  using ::sup::sequencer::JobState;

  auto procedure = testutils::CreateSequenceWithTwoMessagesProcedure();
  auto procedure_ptr = procedure.get();

  auto runner = CreateRunner(std::move(procedure), /*listen_callbacks*/ false);
  EXPECT_EQ(runner->GetJobState(), sup::sequencer::JobState::kInitial);
  EXPECT_EQ(procedure_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  runner->Step();

  auto has_paused = [&runner]() { return runner->GetJobState() == JobState::kPaused; };
  EXPECT_TRUE(testutils::WaitFor(has_paused, msec(50)));

  EXPECT_TRUE(runner->Stop());

  runner->WaitForFinished();

  EXPECT_EQ(runner->GetJobState(), sup::sequencer::JobState::kHalted);
  EXPECT_EQ(procedure_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_FINISHED);
}

//! Repeat procedure with increment instruction inside. We start in step mode, and after the first
//! step continue till the end without interruptions.
TEST_F(LocalDomainRunnerTest, StepAndRunTillTheEnd)
{
  auto procedure = testutils::CreateCounterProcedure(3);
  auto procedure_ptr = procedure.get();
  auto variable = procedure_ptr->GetWorkspace().GetVariable("counter");
  procedure_ptr->Setup();

  sup::dto::AnyValue counter_value;
  variable->GetValue(counter_value);
  EXPECT_EQ(counter_value, 0U);

  // JobState: initial, stepping, paused
  EXPECT_CALL(m_event_listener, OnJobStateChanged(_)).Times(3);
  // Instruction: repeat (not finished) + one increment (not finished, success)
  EXPECT_CALL(m_event_listener, OnInstructionStateUpdated(_)).Times(3);
  EXPECT_CALL(m_event_listener, OnVariableUpdated(_)).Times(1);  // variable changed (increment 1)
  EXPECT_CALL(m_event_listener, OnNextLeavesChanged(_)).Times(3);

  auto runner = CreateRunner(std::move(procedure));

  // making step
  EXPECT_TRUE(runner->Step());

  auto is_incremented = [variable, &runner]()
  {
    sup::dto::AnyValue counter_value;
    variable->GetValue(counter_value);
    return counter_value == 1U && runner->GetEventCount() == 0;
  };
  EXPECT_TRUE(QTest::qWaitFor(is_incremented, 100));

  // JobState: running, success
  EXPECT_CALL(m_event_listener, OnJobStateChanged(_)).Times(2);
  // Instruction: two increments 2*(not started, not finished, success) + repeat (success)
  EXPECT_CALL(m_event_listener, OnInstructionStateUpdated(_)).Times(7);
  EXPECT_CALL(m_event_listener, OnVariableUpdated(_))
      .Times(2);  // variable changed (increment 2 and 3)

  // continuing till the end
  EXPECT_TRUE(runner->Start());

  EXPECT_TRUE(WaitForStateAndEmptyQueue(*runner, sup::sequencer::JobState::kSucceeded, msec(100)));

  EXPECT_EQ(runner->GetJobState(), sup::sequencer::JobState::kSucceeded);
  EXPECT_EQ(procedure_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

  variable->GetValue(counter_value);
  EXPECT_EQ(counter_value, 3U);
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
TEST_F(LocalDomainRunnerTest, RunPauseRun)
{
  using ::sup::sequencer::ExecutionStatus;
  using ::sup::sequencer::JobState;

  // The procedure contains two variables: a counter, and variable for interruption. By magic we
  // know variable names.
  auto procedure = testutils::CreateRepeatIncrementAndCompare();
  auto procedure_ptr = procedure.get();

  const std::string kCounterVarName("counter");
  auto& workspace = procedure_ptr->GetWorkspace();
  auto counter_var = workspace.GetVariable(kCounterVarName);

  procedure_ptr->Setup();

  // counter is 0 at the beginning
  sup::dto::AnyValue counter_value;
  counter_var->GetValue(counter_value);
  EXPECT_EQ(counter_value, 0U);

  auto runner = CreateRunner(std::move(procedure), /*listen_callbacks*/ false);

  // starting and waiting a bit to let the counter to increment
  EXPECT_TRUE(runner->Start());

  auto is_incremented = [counter_var]()
  {
    sup::dto::AnyValue counter_value;
    counter_var->GetValue(counter_value);
    return counter_value.As<int>() > 1U;
  };
  EXPECT_TRUE(QTest::qWaitFor(is_incremented, 100));

  // pause
  EXPECT_TRUE(runner->Pause());
  EXPECT_TRUE(runner->WaitForState(sup::sequencer::JobState::kPaused, 200));

  // It is paused now, increment counter should increase.
  counter_var->GetValue(counter_value);
  auto current_counter1 = counter_value.As<int>();
  EXPECT_TRUE(current_counter1 > 1);

  // continuing till the end
  EXPECT_TRUE(runner->Start());

  EXPECT_TRUE(runner->WaitForState(sup::sequencer::JobState::kRunning, 200));

  // to stop incrementing use interruption variable
  const sup::dto::AnyValue interrupt_value(0U);
  procedure_ptr->GetWorkspace().SetValue("to_continue", interrupt_value);

  runner->WaitForFinished();

  counter_var->GetValue(counter_value);
  auto current_counter2 = counter_value.As<int>();
  EXPECT_TRUE(current_counter2 >= current_counter1);

  // it is failed because we've made a sequence quite
  EXPECT_EQ(runner->GetJobState(), sup::sequencer::JobState::kFailed);
  EXPECT_EQ(procedure_ptr->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);
}
