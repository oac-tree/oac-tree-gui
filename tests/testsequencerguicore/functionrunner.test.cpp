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

#include "sequencergui/jobsystem/functionrunner.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <thread>

using namespace sequencergui;
using ::testing::NiceMock;
using msec = std::chrono::milliseconds;

//! Tests for JobStates.

class FunctionRunnerTest : public ::testing::Test
{
public:
  //! Auxiliary class to listen for RunnerStatus changed events.
  class MockListener
  {
  public:
    MOCK_METHOD1(StatusChanged, void(RunnerStatus status));

    //! Creates callback to listen for status change.
    //! The call will be propagated to StatusChanged mock method to benefit from gmock.
    std::function<void(RunnerStatus)> CreateCallback()
    {
      return [this](auto status) { StatusChanged(status); };
    }
  };
};

//! Checking that listener works as expected.

TEST_F(FunctionRunnerTest, CheckListener)
{
  MockListener listener;

  EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));

  auto callback = listener.CreateCallback();
  callback(RunnerStatus::kRunning);
}

//! Initial state of FunctionRunner.

TEST_F(FunctionRunnerTest, InitialState)
{
  FunctionRunner runner([]() { return false; });
  EXPECT_EQ(runner.GetRunnerStatus(), RunnerStatus::kIdle);
  EXPECT_FALSE(runner.IsBusy());
  EXPECT_TRUE(WaitForCompletion(runner, 0.001));
}

//! Running worker which immediately returns `false`.
//! Represent short task which is executed normally.

TEST_F(FunctionRunnerTest, StartSingleCall)
{
  MockListener listener;
  auto worker = []() { return false; };
  FunctionRunner runner(worker, listener.CreateCallback());

  // expecting calls with status change kRunning, kCompleted
  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kCompleted));
  }

  EXPECT_TRUE(runner.Start());  // triggering action
  WaitForCompletion(runner, 0.02);
  EXPECT_EQ(runner.GetRunnerStatus(), RunnerStatus::kCompleted);
  EXPECT_FALSE(runner.IsBusy());
}

//! Premature destruction. Runner dies before task is finished.

TEST_F(FunctionRunnerTest, PrematureDeletionDuringRun)
{
  int ncount(0);
  auto worker = [&ncount]()  // executes forever
  {
    ncount++;
    std::this_thread::sleep_for(msec(5));
    return true;
  };

  auto runner = std::make_unique<FunctionRunner>(worker);

  runner->Start();
  std::this_thread::sleep_for(msec(20));
  EXPECT_EQ(runner->GetRunnerStatus(), RunnerStatus::kRunning);
  EXPECT_TRUE(ncount > 0);

  ASSERT_NO_FATAL_FAILURE(runner.reset());
  // event loop was interrupted, thread was succesfully joined
}

//! Terminating the procedure that runs too long.

TEST_F(FunctionRunnerTest, StartAndTerminate)
{
  MockListener listener;
  auto worker = []()
  {
    std::this_thread::sleep_for(msec(10));
    return true;
  };  // executes forever
  FunctionRunner runner(worker, listener.CreateCallback());

  // expecting calls with status change kRunning, kCanceling, kStopped
  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kStopping));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kStopped));
  }

  EXPECT_TRUE(runner.Start());  // triggering action
  EXPECT_TRUE(runner.IsBusy());
  std::this_thread::sleep_for(msec(20));

  EXPECT_FALSE(WaitForCompletion(runner, 0.01));

  runner.Stop();
  std::this_thread::sleep_for(msec(10));

  EXPECT_EQ(runner.GetRunnerStatus(), RunnerStatus::kStopped);
}

//! Stepwise task execution.
//! The task is launched in step wise mode. After 3 steps the task is expected to be completed.

TEST_F(FunctionRunnerTest, StepwiseExecutionAndNormalCompletion)
{
  //  MockListener listener;
  int nsteps{0};
  auto worker = [&nsteps]()
  {
    std::this_thread::sleep_for(msec(10));
    nsteps++;
    std::cout << "worker " << nsteps << std::endl;
    return nsteps < 3;  // should stop when nsteps==3
  };

  FunctionRunner runner(worker);
  //  FunctionRunner runner(worker, listener.CreateCallback());
  runner.SetWaitingMode(WaitingMode::kWaitForRelease);

  //  // expecting calls with status change kRunning, kCanceling, kStopped
  //  {
  //    ::testing::InSequence seq;
  //    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
  //    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kStopping));
  //    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kStopped));
  //  }

  EXPECT_TRUE(runner.Start());  // triggering action
  EXPECT_EQ(runner.GetRunnerStatus(), RunnerStatus::kRunning);
  EXPECT_TRUE(runner.IsBusy());
  std::this_thread::sleep_for(msec(20));

  EXPECT_EQ(nsteps, 1);
  EXPECT_EQ(runner.GetRunnerStatus(), RunnerStatus::kRunning);
  EXPECT_TRUE(runner.IsBusy());

  runner.Step();
  std::this_thread::sleep_for(msec(20));

  EXPECT_EQ(nsteps, 2);
  EXPECT_EQ(runner.GetRunnerStatus(), RunnerStatus::kRunning);
  EXPECT_TRUE(runner.IsBusy());

  runner.Step();
  std::this_thread::sleep_for(msec(20));

  EXPECT_EQ(nsteps, 3);
  EXPECT_EQ(runner.GetRunnerStatus(), RunnerStatus::kCompleted);
  EXPECT_FALSE(runner.IsBusy());
}
