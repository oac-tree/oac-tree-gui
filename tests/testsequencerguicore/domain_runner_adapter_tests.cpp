/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "Procedure.h"
#include "Runner.h"
#include "mock_runner_listener.h"
#include "mock_sequencer_observer.h"
#include "sequencergui/jobsystem/domain_runner_adapter.h"
#include "test_domain_procedures.h"

#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <thread>

namespace
{
const auto duration = [](auto time_interval)
{ return std::chrono::duration_cast<std::chrono::milliseconds>(time_interval).count(); };

//! hard-coded value in Wait instruction
const std::chrono::milliseconds kDefaultWaitPrecision(50);
}  // namespace

using namespace sequencergui;
using ::testing::_;

class DomainRunnerAdapterTest : public ::testing::Test
{
public:
  using clock_used = std::chrono::high_resolution_clock;
  using time_t = std::chrono::time_point<clock_used>;
  using duration_unit = std::chrono::milliseconds;
  using msec = std::chrono::milliseconds;

  std::unique_ptr<runner_t> CreateRunner(procedure_t* procedure)
  {
    auto result = std::make_unique<runner_t>(&m_observer);
    procedure->Setup();
    result->SetProcedure(procedure);
    return result;
  }

  std::unique_ptr<DomainRunnerAdapter> CreateRunnerAdapter(procedure_t* procedure)
  {
    if (!procedure->Setup())
    {
      throw std::runtime_error("Can't setup procedure");
    }
    auto result =
        std::make_unique<DomainRunnerAdapter>(procedure, &m_observer, m_listener.CreateCallback());

    return result;
  }

  testutils::MockSequencerObserver m_observer;
  testutils::MockRunnerListener m_listener;
};

//! Single wait procedure started in normal way. Waiting upon completion.

TEST_F(DomainRunnerAdapterTest, ShortProcedureThatExecutesNormally)
{
  std::chrono::milliseconds timeout_msec(10);
  auto procedure = testutils::CreateSingleWaitProcedure(timeout_msec);

  auto adapter = CreateRunnerAdapter(procedure.get());

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  {  // signaling related to the runner status change
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kCompleted));
  }

  {  // observer signaling
    ::testing::InSequence seq;
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(2);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
  }

  // triggering action
  EXPECT_TRUE(adapter->Start());

  EXPECT_TRUE(adapter->WaitForCompletion(kDefaultWaitPrecision + msec(timeout_msec)));
  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);
}

//! Terminates procedure which runs too long.

TEST_F(DomainRunnerAdapterTest, StartAndTerminate)
{
  std::chrono::milliseconds timeout_msec(10000);
  auto procedure = testutils::CreateSingleWaitProcedure(timeout_msec);

  auto adapter = CreateRunnerAdapter(procedure.get());

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  {  // signaling related to the runner status change
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kStopping));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kStopped));
  }

  {  // observer signaling
    ::testing::InSequence seq;
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(2);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
  }

  // triggering action
  EXPECT_TRUE(adapter->Start());  // trigger action

  EXPECT_TRUE(adapter->IsBusy());
  std::this_thread::sleep_for(msec(20));

  EXPECT_FALSE(adapter->WaitForCompletion(msec(10)));

  adapter->Stop();
  std::this_thread::sleep_for(msec(10));

  EXPECT_FALSE(adapter->IsBusy());
  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kStopped);
  // it is FAILURE here (and not NOT_FINISHED) necause we have interrupted Wait with the Halt
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);
}

//! Runner dies before procedure has finished.
//! Test is commented. For the moment an attempt to delete the runner during procedure execution
//! will lead to UB. It is not clear how to provide mutual safety for time of life of Procedure
//! and DomainRunnerAdapter.

// TEST_F(DomainRunnerAdapterTest, PrematureDeletion)
//{
//   std::chrono::milliseconds timeout_msec(10000);
//   auto procedure = testutils::CreateSingleWaitProcedure(timeout_msec);
//   auto runner = CreateRunner(procedure.get());

//  auto adapter =
//      std::make_unique<DomainRunnerAdapter>(std::move(runner), m_listener.CreateCallback());
//  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
//  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

//  {  // signaling related to the runner status change
//    ::testing::InSequence seq;
//    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
//    // We do not expect any ohter signals during premature DomainRunnerAdapter destruction.
//    // This is how internal FunctionRunner is implemented.
//  }

//  {  // observer signaling
//    ::testing::InSequence seq;
//    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
//    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(2);
//    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
//  }

//  // triggering action
//  EXPECT_TRUE(adapter->Start());  // trigger action

//  EXPECT_TRUE(adapter->IsBusy());
//  std::this_thread::sleep_for(msec(20));

//  EXPECT_FALSE(adapter->WaitForCompletion(msec(10)));

//  // delete before end
//  EXPECT_NO_FATAL_FAILURE(adapter.reset());
//  std::this_thread::sleep_for(msec(10));

//  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::FAILURE);
//}

//! Sequence with single wait in normal start mode.

TEST_F(DomainRunnerAdapterTest, SequenceWithSingleWait)
{
  const int tick_timeout_msec(1000);
  std::chrono::milliseconds timeout_msec(10);

  auto procedure = testutils::CreateSequenceWithWaitProcedure(timeout_msec);
  auto adapter = CreateRunnerAdapter(procedure.get());

  adapter->SetTickTimeout(tick_timeout_msec);

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  {  // signaling related to the runner status changer
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kCompleted));
  }

  {  // observer signaling
    ::testing::InSequence seq;
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(4);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
  }

  // triggering action
  time_t start_time = clock_used::now();
  EXPECT_TRUE(adapter->Start());

  EXPECT_TRUE(adapter->WaitForCompletion(kDefaultWaitPrecision + 2 * msec(timeout_msec)));

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

  time_t end_time = clock_used::now();

  // Here we test that adapter.SetTickTimeout(1000) doesn't influence execution time,
  // since we have only one child that gets executed during single step.
  EXPECT_TRUE(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
              < msec(kDefaultWaitPrecision * 2));
}

//! Sequence with single wait in normal start mode.
//! Additional tick timeout slows down the execution.

TEST_F(DomainRunnerAdapterTest, SequenceWithTwoWaits)
{
  const int tick_timeout_msec(100);

  auto procedure = testutils::CreateSequenceWithTwoWaitsProcedure(msec(10), msec(10));
  auto adapter = CreateRunnerAdapter(procedure.get());

  adapter->SetTickTimeout(tick_timeout_msec);

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  {  // signaling related to the runner status changer
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kCompleted));
  }

  {  // observer signaling
    ::testing::InSequence seq;
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(3);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(3);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
  }

  // triggering action
  time_t start_time = clock_used::now();
  EXPECT_TRUE(adapter->Start());

  EXPECT_TRUE(adapter->WaitForCompletion(msec(1000)));

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

  time_t end_time = clock_used::now();

  // here we test that adapter.SetTickTimeout(100) was invoked once
  EXPECT_TRUE(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
              > msec(tick_timeout_msec));
}

//! Sequence with two waits in step mode.

TEST_F(DomainRunnerAdapterTest, SequenceWithTwoWaitsInStepMode)
{
  auto procedure = testutils::CreateSequenceWithTwoWaitsProcedure(msec(10), msec(10));
  auto adapter = CreateRunnerAdapter(procedure.get());

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  {  // signaling related to the runner status changer
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kPaused));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kCompleted));
  }

  {  // observer signaling
    ::testing::InSequence seq;
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(3);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(3);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
  }

  // triggering action
  EXPECT_TRUE(adapter->Step());
  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kRunning);
  std::this_thread::sleep_for(msec(kDefaultWaitPrecision * 3));
  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kPaused);
  EXPECT_TRUE(adapter->Step());

  EXPECT_TRUE(adapter->WaitForCompletion(msec(1000)));

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);
}

//! Running procedure (sequence with message) and let is finish.
//! Then run same procedure again.

TEST_F(DomainRunnerAdapterTest, ConsequitiveProcedureExecution)
{
  auto procedure = testutils::CreateSequenceWithSingleMessageProcedure();
  auto adapter = CreateRunnerAdapter(procedure.get());

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  {  // signaling related to the runner status changer
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kCompleted));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kCompleted));
  }

  {  // observer signaling
    ::testing::InSequence seq;
    // first run (sequence, message
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(1);  // sequence
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(1);  // message
    EXPECT_CALL(m_observer, MessageImpl(_)).Times(1);                  // message
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(1);  // message
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(1);  // sequence
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);

    // second run (sequence, message
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(1);  // sequence
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(1);  // message
    EXPECT_CALL(m_observer, MessageImpl(_)).Times(1);                  // message
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(1);  // message
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(1);  // sequence
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
  }

  // triggering action
  EXPECT_TRUE(adapter->Start());
  EXPECT_TRUE(adapter->WaitForCompletion(msec(50)));
  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

  // starting again
  EXPECT_TRUE(adapter->Start());
  EXPECT_TRUE(adapter->WaitForCompletion(msec(50)));
  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);
}

//! Sequence with two waits in step mode. After first step it is interrupted.

TEST_F(DomainRunnerAdapterTest, SequenceWithTwoWaitsInStepModeInterrupted)
{
  auto procedure = testutils::CreateSequenceWithTwoWaitsProcedure(msec(10), msec(10));
  auto adapter = CreateRunnerAdapter(procedure.get());

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  {  // signaling related to the runner status changer
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kPaused));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kStopping));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kStopped));
  }

  {  // observer signaling
    ::testing::InSequence seq;
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(3);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
    //    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    //    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(3);
    //    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
  }

  // triggering action
  EXPECT_TRUE(adapter->Step());
  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kRunning);
  std::this_thread::sleep_for(msec(kDefaultWaitPrecision * 3));
  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kPaused);

  // stopping job
  EXPECT_TRUE(adapter->Stop());
  EXPECT_TRUE(adapter->WaitForCompletion(msec(1000)));

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kStopped);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_FINISHED);
}

//! Sequence with two waits in step mode. After first step it is interrupted, and then started from
//! the beginning.

TEST_F(DomainRunnerAdapterTest, SequenceWithTwoWaitsInStepModeInterruptedAndRestarted)
{
  auto procedure = testutils::CreateSequenceWithTwoWaitsProcedure(msec(10), msec(10));
  auto adapter = CreateRunnerAdapter(procedure.get());

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  {  // signaling related to the runner status changer
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kPaused));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kStopping));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kStopped));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kCompleted));
  }

  {  // observer signaling
    ::testing::InSequence seq;
    // first step
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(3);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
    // after job restart
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(3);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(3);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
  }

  // triggering action
  EXPECT_TRUE(adapter->Step());
  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kRunning);
  std::this_thread::sleep_for(msec(kDefaultWaitPrecision * 3));
  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kPaused);

  // stopping job
  EXPECT_TRUE(adapter->Stop());
  EXPECT_TRUE(adapter->WaitForCompletion(msec(1000)));

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kStopped);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_FINISHED);

  // restarting job
  EXPECT_TRUE(adapter->Start());
  EXPECT_TRUE(adapter->WaitForCompletion(msec(1000)));
  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);
}

//! Sequence with two waits in step mode. We start procedure normally and let is run till
//! completion. Then start again in step mode.

TEST_F(DomainRunnerAdapterTest, SequenceWithTwoWaitsRunTillCompletionThenStep)
{
  auto procedure = testutils::CreateSequenceWithTwoWaitsProcedure(msec(10), msec(10));
  auto adapter = CreateRunnerAdapter(procedure.get());

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kIdle);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::NOT_STARTED);

  {  // signaling related to the runner status changer
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kCompleted));

    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kPaused));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, StatusChanged(RunnerStatus::kCompleted));
  }

  {  // observer signaling
    ::testing::InSequence seq;

    // first execution
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(3);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(3);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);

    // execution in step mode
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(3);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, StartSingleStepImpl()).Times(1);
    EXPECT_CALL(m_observer, UpdateInstructionStatusImpl(_)).Times(3);
    EXPECT_CALL(m_observer, EndSingleStepImpl()).Times(1);
  }

  // triggering action
  EXPECT_TRUE(adapter->Start());
  EXPECT_TRUE(adapter->WaitForCompletion(msec(1000)));
  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

  // triggering action
  EXPECT_TRUE(adapter->Step());
  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kRunning);
  std::this_thread::sleep_for(msec(kDefaultWaitPrecision * 3));
  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kPaused);
  EXPECT_TRUE(adapter->Step());

  EXPECT_TRUE(adapter->WaitForCompletion(msec(1000)));

  EXPECT_EQ(adapter->GetStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);
}
