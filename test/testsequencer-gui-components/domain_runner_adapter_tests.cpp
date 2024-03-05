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

// #include "sequencergui/jobsystem/domain_runner_adapter.h"

// #include <sequencergui/core/exceptions.h>
// #include <sup/gui/model/anyvalue_utils.h>

// #include <mvvm/test/mock_callback_listener.h>

// #include <sup/dto/anyvalue.h>
// #include <sup/sequencer/procedure.h>
// #include <sup/sequencer/runner.h>
// #include <sup/sequencer/variable.h>
// #include <sup/sequencer/workspace.h>

// #include <gtest/gtest.h>
// #include <testutils/mock_sequencer_observer.h>
// #include <testutils/standard_procedures.h>
// #include <testutils/test_utils.h>

// #include <chrono>
// #include <memory>
// #include <thread>

// using namespace sequencergui;
// using ::testing::_;
// using ::testing::AtLeast;

// class DomainRunnerAdapterTest : public ::testing::Test
// {
// public:
//   using clock_used = std::chrono::high_resolution_clock;
//   using time_t = std::chrono::time_point<clock_used>;
//   using duration_unit = std::chrono::milliseconds;
//   using msec = std::chrono::milliseconds;

//   std::unique_ptr<DomainRunnerAdapter> CreateRunnerAdapter(procedure_t* procedure)
//   {
//     procedure->Setup();
//     DomainRunnerAdapterContext context{procedure, &m_observer, m_runner_listener.CreateCallback(),
//                                 m_tick_listener.CreateCallback()};
//     auto result = std::make_unique<DomainRunnerAdapter>(context);

//     return result;
//   }

//   testutils::MockSequencerObserver m_observer;
//   mvvm::test::MockCallbackListener<sequencergui::RunnerStatus> m_runner_listener;
//   mvvm::test::MockCallbackListener<procedure_t> m_tick_listener;
// };

// // NOTE: MIGRATION_DONE

// TEST_F(DomainRunnerAdapterTest, InitialState)
// {
//   const std::chrono::milliseconds timeout(10);
//   auto procedure = testutils::CreateSingleWaitProcedure(timeout);

//   auto adapter = CreateRunnerAdapter(procedure.get());
//   EXPECT_FALSE(adapter->IsBusy());
//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kInitial);
// }

// //! Single instruction procedure started in normal way. Waiting upon completion.
// // NOTE: MIGRATION_DONE

// TEST_F(DomainRunnerAdapterTest, ShortProcedureThatExecutesNormally)
// {
//   auto procedure = testutils::CreateMessageProcedure("text");

//   auto adapter = CreateRunnerAdapter(procedure.get());

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kInitial);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//   {  // signaling related to the runner status change
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(1);
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kSucceeded));
//   }

//   EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(2);
//   EXPECT_CALL(m_observer, Message(_)).Times(1);

//   // triggering action
//   EXPECT_TRUE(adapter->Start());

//   auto is_completed = [&adapter]() { return !adapter->IsBusy(); };
//   EXPECT_TRUE(testutils::WaitFor(is_completed, msec(50)));

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kSucceeded);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);
// }

// //! Terminates procedure which runs too long.
// // NOTE: MIGRATION_DONE

// TEST_F(DomainRunnerAdapterTest, DISABLED_StartAndTerminate)
// {
//   const std::chrono::milliseconds wait_timeout(10000);

//   auto procedure = testutils::CreateSingleWaitProcedure(wait_timeout);

//   auto adapter = CreateRunnerAdapter(procedure.get());

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//   {  // signaling related to the runner status change
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kStopping));
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kStopped));
//     // tick is here (after runner status was) set because we do not accurate with stopping/stopped
//     // signals
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(1);
//   }

//   {  // observer signaling
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(2);
//   }

//   // triggering action
//   EXPECT_TRUE(adapter->Start());  // trigger action

//   auto has_started = [&adapter]() { return adapter->IsBusy(); };
//   EXPECT_TRUE(testutils::WaitFor(has_started, msec(50)));

//   EXPECT_TRUE(adapter->IsBusy());

//   adapter->Stop();

//   auto is_completed = [&adapter]()
//   { return !adapter->IsBusy() && adapter->GetStatus() == RunnerStatus::kStopped; };
//   EXPECT_TRUE(testutils::WaitFor(is_completed, msec(100)));

//   EXPECT_FALSE(adapter->IsBusy());
//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kStopped);
//   // it is FAILURE here (and not NOT_FINISHED) because we have interrupted Wait with the Halt
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);
// }

// //! Sequence with single message in normal start mode.
// //! Validation that tick timeout is ignored for single instructions.
// // NOTE: MIGRATION_DONE

// TEST_F(DomainRunnerAdapterTest, SequenceWithSingleMessage)
// {
//   const int tick_timeout_msec(1000);

//   auto procedure = testutils::CreateSequenceWithSingleMessageProcedure();
//   auto adapter = CreateRunnerAdapter(procedure.get());

//   adapter->SetTickTimeout(tick_timeout_msec);

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//   {  // signaling related to the runner status changer
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(1);
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kCompleted));
//   }

//   {  // observer signaling
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(4);
//   }

//   EXPECT_CALL(m_observer, Message(_)).Times(1);  // message

//   // triggering action
//   time_t start_time = clock_used::now();
//   EXPECT_TRUE(adapter->Start());

//   auto is_completed = [&adapter]() { return !adapter->IsBusy(); };
//   EXPECT_TRUE(testutils::WaitFor(is_completed, msec(50)));

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

//   time_t end_time = clock_used::now();

//   // Here we test that adapter.SetTickTimeout(1000) doesn't influence execution time,
//   // since we have only one child that gets executed during single step.
//   EXPECT_TRUE(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
//               < msec(tick_timeout_msec));
// }

// //! Sequence with two messages in normal start mode.
// //! Additional tick timeout slows down the execution.
// // NOTE: MIGRATION_DONE

// TEST_F(DomainRunnerAdapterTest, SequenceWithTwoMessages)
// {
//   const int tick_timeout_msec(50);

//   auto procedure = testutils::CreateSequenceWithTwoMessagesProcedure();
//   auto adapter = CreateRunnerAdapter(procedure.get());

//   adapter->SetTickTimeout(tick_timeout_msec);

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//   {  // signaling related to the runner status changer
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(2);
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kCompleted));
//   }

//   {  // observer signaling
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(3);
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(3);
//   }

//   EXPECT_CALL(m_observer, Message(_)).Times(2);  // message

//   // triggering action
//   time_t start_time = clock_used::now();
//   EXPECT_TRUE(adapter->Start());

//   EXPECT_TRUE(testutils::WaitForCompletion(*adapter, msec(200)));

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

//   time_t end_time = clock_used::now();

//   // here we test that adapter.SetTickTimeout(100) was invoked once
//   EXPECT_TRUE(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
//               >= msec(tick_timeout_msec));
// }

// //! Sequence with two waits in step mode. Making steps until complete.
// // NOTE: MIGRATION_DONE

// TEST_F(DomainRunnerAdapterTest, SequenceWithTwoWaitsInStepMode)
// {
//   auto procedure = testutils::CreateSequenceWithTwoWaitsProcedure(msec(10), msec(10));
//   auto adapter = CreateRunnerAdapter(procedure.get());

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//   {  // signaling related to the runner status changer
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(1);
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kPaused));
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(1);
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kCompleted));
//   }

//   {  // observer signaling
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(3);
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(3);
//   }

//   // triggering action
//   EXPECT_TRUE(adapter->Step());
//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kRunning);
//   std::this_thread::sleep_for(msec(testutils::kDefaultWaitPrecision * 3));
//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kPaused);
//   EXPECT_TRUE(adapter->Step());

//   EXPECT_TRUE(testutils::WaitForCompletion(*adapter, msec(1000)));

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);
// }

// //! Sequencer with two messages in step mode. Making steps until complete.
// // NOTE: MIGRATION_NO_NEED

// TEST_F(DomainRunnerAdapterTest, StepwiseExecution)
// {
//   auto procedure = testutils::CreateSequenceWithTwoMessagesProcedure();
//   auto adapter = CreateRunnerAdapter(procedure.get());

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//   {  // signaling related to the runner status changer
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(1);
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kPaused));
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(1);
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kCompleted));
//   }

//   {  // observer signaling
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(3);
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(3);
//   }

//   EXPECT_CALL(m_observer, Message(_)).Times(2);

//   // triggering action
//   EXPECT_TRUE(adapter->Step());
//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kRunning);

//   testutils::WaitFor([&adapter]() { return adapter->GetStatus() == RunnerStatus::kPaused; },
//                      msec(50));

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kPaused);
//   EXPECT_TRUE(adapter->Step());

//   testutils::WaitFor([&adapter]() { return adapter->GetStatus() == RunnerStatus::kCompleted; },
//                      msec(50));

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);
// }

// //! Running procedure (sequence with message) and let is finish.
// //! Then run same procedure again.
// // NOTE: MIGRATION_TO_CONSIDER
// TEST_F(DomainRunnerAdapterTest, ConsequitiveProcedureExecution)
// {
//   auto procedure = testutils::CreateSequenceWithSingleMessageProcedure();
//   auto adapter = CreateRunnerAdapter(procedure.get());

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//   {  // signaling related to the runner status changer
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(1);
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kCompleted));
//   }

//   {  // observer signaling
//     ::testing::InSequence seq;
//     // first run (sequence, message
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(1);  // sequence
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(1);  // message
//     EXPECT_CALL(m_observer, Message(_)).Times(1);                  // message
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(1);  // message
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(1);  // sequence
//   }

//   // triggering action
//   EXPECT_TRUE(adapter->Start());
//   EXPECT_TRUE(testutils::WaitForCompletion(*adapter, msec(50)));
//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

//   // Adapter is not intended to be run twice with the same procedure.
//   EXPECT_THROW(adapter->Start(), RuntimeException);
// }

// //! Sequence with two waits in step mode. After first step it is interrupted.
// // NOTE: MIGRATION_DONE

// TEST_F(DomainRunnerAdapterTest, SequenceWithTwoWaitsInStepModeInterrupted)
// {
//   auto procedure = testutils::CreateSequenceWithTwoWaitsProcedure(msec(10), msec(10));
//   auto adapter = CreateRunnerAdapter(procedure.get());

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//   {  // signaling related to the runner status changer
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(1);
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kPaused));
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kStopping));
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kStopped));
//   }

//   {  // observer signaling
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(3);
//   }

//   // triggering action
//   EXPECT_TRUE(adapter->Step());
//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kRunning);
//   std::this_thread::sleep_for(msec(testutils::kDefaultWaitPrecision * 3));
//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kPaused);

//   // stopping job
//   EXPECT_TRUE(adapter->Stop());
//   EXPECT_TRUE(testutils::WaitForCompletion(*adapter, msec(1000)));

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kStopped);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_FINISHED);
// }

// //! Sequence with two waits in step mode. After first step it is interrupted, and then started from
// //! the beginning. This time exception should be thrown, since same adapter can't be run twice with
// //! the same procedure.
// // NOTE: MIGRATION_TO_CONSIDER

// TEST_F(DomainRunnerAdapterTest, SequenceWithTwoWaitsInStepModeInterruptedAndRestarted)
// {
//   auto procedure = testutils::CreateSequenceWithTwoWaitsProcedure(msec(10), msec(10));
//   auto adapter = CreateRunnerAdapter(procedure.get());

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//   {  // signaling related to the runner status changer
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(1);
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kPaused));
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kStopping));
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kStopped));
//   }

//   {  // observer signaling
//     ::testing::InSequence seq;
//     // first step
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(3);
//   }

//   // triggering action
//   EXPECT_TRUE(adapter->Step());
//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kRunning);
//   std::this_thread::sleep_for(msec(testutils::kDefaultWaitPrecision * 3));
//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kPaused);

//   // stopping job
//   EXPECT_TRUE(adapter->Stop());
//   EXPECT_TRUE(testutils::WaitForCompletion(*adapter, msec(1000)));

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kStopped);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_FINISHED);

//   // restarting job
//   EXPECT_THROW(adapter->Start(), RuntimeException);
// }

// //! Sequence with two waits in step mode. We start procedure normally and let is run till
// //! completion. Then start again in step mode. This time exception should be thrown,
// //! since same adapter can't be run twice with the same procedure.
// // NOTE: MIGRATION_TO_CONSIDER

// TEST_F(DomainRunnerAdapterTest, SequenceWithTwoWaitsRunTillCompletionThenStep)
// {
//   auto procedure = testutils::CreateSequenceWithTwoWaitsProcedure(msec(10), msec(10));
//   auto adapter = CreateRunnerAdapter(procedure.get());

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//   {  // signaling related to the runner status changer
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(2);
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kCompleted));
//   }

//   {  // observer signaling
//     ::testing::InSequence seq;

//     // first execution
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(3);
//     EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(3);
//   }

//   // triggering action
//   EXPECT_TRUE(adapter->Start());
//   EXPECT_TRUE(testutils::WaitForCompletion(*adapter, msec(1000)));
//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

//   // triggering action
//   EXPECT_THROW(adapter->Step(), RuntimeException);
// }

// //! Long running procedure gets stopped, then started again.
// // NOTE: MIGRATION_TO_CONSIDER

// TEST_F(DomainRunnerAdapterTest, AttemptToStartAfterAbnormalStop)
// {
//   std::chrono::milliseconds timeout_msec(100);
//   auto procedure = testutils::CreateRepeatSequenceProcedure(-1, timeout_msec);

//   auto adapter = CreateRunnerAdapter(procedure.get());

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//   {  // signaling related to the runner status change
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kStopping));
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kStopped));
//     // tick is here (after runner status was) set because we do not accurate with stopping/stopped
//     // signals
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(1);
//   }

//   EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(AtLeast(1));

//   // triggering action
//   EXPECT_TRUE(adapter->Start());

//   EXPECT_TRUE(adapter->IsBusy());
//   std::this_thread::sleep_for(msec(20));

//   EXPECT_FALSE(testutils::WaitForCompletion(*adapter, msec(10)));

//   adapter->Stop();

//   auto is_completed = [&adapter]() { return !adapter->IsBusy(); };
//   EXPECT_TRUE(testutils::WaitFor(is_completed, msec(50)));

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kStopped);
//   // it is FAILURE here (and not NOT_FINISHED) because we have interrupted Wait with the Halt
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);

//   // second round
//   // For the moment we do not allow to job to be restarted after abnormal termination

//   EXPECT_THROW(adapter->Start(), RuntimeException);
// }

// //! Repeat procedure with increment instruction inside. We start in step mode, and after the first
// //! step continue till the end without interruptions.
// // NOTE: MIGRATION_DONE

// TEST_F(DomainRunnerAdapterTest, StepAndRunTillTheEnd)
// {
//   auto procedure = testutils::CreateCounterProcedure(3);
//   auto variable = procedure->GetWorkspace().GetVariable("counter");
//   procedure->Setup();

//   sup::dto::AnyValue counter_value;
//   variable->GetValue(counter_value);
//   EXPECT_EQ(counter_value, 0U);

//   auto adapter = CreateRunnerAdapter(procedure.get());

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//   {  // signaling related to the runner status change
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(1);
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kPaused));
//   }

//   EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(AtLeast(1));
//   EXPECT_CALL(m_observer, VariableUpdated(_, _, _)).Times(1);

//   // making step
//   EXPECT_TRUE(adapter->Step());

//   auto is_incremented = [variable]()
//   {
//     sup::dto::AnyValue counter_value;
//     variable->GetValue(counter_value);
//     return counter_value == 1U;
//   };

//   EXPECT_TRUE(testutils::WaitFor(is_incremented, msec(50)));

//   {  // signaling related to the runner status change
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_tick_listener, OnCallback(_)).Times(2);
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kCompleted));
//   }

//   EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(AtLeast(2));
//   EXPECT_CALL(m_observer, VariableUpdated(_, _, _)).Times(2);

//   // continuing till the end
//   EXPECT_TRUE(adapter->Start());

//   auto is_completed = [&adapter]() { return !adapter->IsBusy(); };
//   EXPECT_TRUE(testutils::WaitFor(is_completed, msec(50)));

//   variable->GetValue(counter_value);
//   EXPECT_EQ(counter_value, 3U);
// }

// //! Repeat procedure with increment instruction inside. We start in run mode, then pause, then make
// //! a step and finally continue till the end.
// //! Test is unstabled and disabled.
// // NOTE: MIGRATION_DONE

// TEST_F(DomainRunnerAdapterTest, DISABLED_RunPauseStepRun)
// {
//   auto procedure = testutils::CreateCounterProcedure(1000);
//   auto variable = procedure->GetWorkspace().GetVariable("counter");
//   procedure->Setup();

//   sup::dto::AnyValue counter_value;
//   variable->GetValue(counter_value);
//   EXPECT_EQ(counter_value, 0U);

//   auto adapter = CreateRunnerAdapter(procedure.get());

//   EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
//   EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//   // starting normally

//   {  // signaling related to the runner status change
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//   }

//   EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(AtLeast(1));
//   EXPECT_CALL(m_observer, VariableUpdated(_, _, _)).Times(AtLeast(1));

//   EXPECT_TRUE(adapter->Start());  // action

//   auto is_incremented = [variable]()
//   {
//     sup::dto::AnyValue counter_value;
//     variable->GetValue(counter_value);
//     return counter_value.As<int>() > 1U;
//   };

//   EXPECT_TRUE(testutils::WaitFor(is_incremented, msec(50)));

//   // pause

//   {  // signaling related to the runner status change
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kPaused));
//   }

//   EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(AtLeast(1));
//   EXPECT_CALL(m_observer, VariableUpdated(_, _, _)).Times(AtLeast(0));

//   EXPECT_TRUE(adapter->Pause());  // action

//   testutils::WaitFor([&adapter]() { return adapter->GetStatus() == RunnerStatus::kPaused; },
//                      msec(50));

//   variable->GetValue(counter_value);
//   auto current_counter1 = counter_value.As<int>();

//   EXPECT_TRUE(current_counter1 > 1);

//   // making a step

//   {  // signaling related to the runner status change
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kPaused));
//   }

//   EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(AtLeast(1));
//   EXPECT_CALL(m_observer, VariableUpdated(_, _, _)).Times(AtLeast(0));

//   EXPECT_TRUE(adapter->Step());  // action

//   testutils::WaitFor([&adapter]() { return adapter->GetStatus() == RunnerStatus::kPaused; },
//                      msec(50));

//   variable->GetValue(counter_value);
//   auto current_counter2 = counter_value.As<int>();

//   // Sometimes it fails here, and current_counter2 is incremented more than necessary.
//   // It can be because Pause status is set later than necessary.
//   EXPECT_EQ(current_counter2, current_counter1 + 1);

//   {  // signaling related to the runner status change
//     ::testing::InSequence seq;
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kRunning));
//     EXPECT_CALL(m_runner_listener, OnCallback(RunnerStatus::kCompleted));
//   }

//   EXPECT_CALL(m_observer, UpdateInstructionStatus(_)).Times(AtLeast(1));
//   EXPECT_CALL(m_observer, VariableUpdated(_, _, _)).Times(AtLeast(0));

//   // continuing till the end
//   EXPECT_TRUE(adapter->Start());

//   auto is_completed = [&adapter]() { return !adapter->IsBusy(); };
//   EXPECT_TRUE(testutils::WaitFor(is_completed, msec(50)));

//   variable->GetValue(counter_value);
//   EXPECT_EQ(counter_value, 1000);
// }
