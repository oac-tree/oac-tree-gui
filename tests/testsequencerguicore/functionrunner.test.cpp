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

#include "mockrunnerlistener.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <stdexcept>
#include <thread>

using namespace sequencergui;
using msec = std::chrono::milliseconds;

//! Tests for JobStates.

class FunctionRunnerTest : public ::testing::Test
{
};

//! Checking that listener works as expected.

TEST_F(FunctionRunnerTest, CheckListener)
{
  testutils::MockRunnerListener listener;

  EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));

  auto callback = listener.CreateCallback();
  callback(RunnerStatus::kRunning);
}

//! Initial state of FunctionRunner.

TEST_F(FunctionRunnerTest, InitialState)
{
  FunctionRunner runner([]() { return false; });
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kIdle);
  EXPECT_FALSE(runner.IsBusy());
  EXPECT_TRUE(WaitForCompletion(runner, msec(1)));
}

//! Start and normal completion of the short task.

TEST_F(FunctionRunnerTest, ShortTaskNormalCompletion)
{
  testutils::MockRunnerListener listener;
  auto worker = []() { return false; };  // worker asks to exit immediately
  FunctionRunner runner(worker, listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kCompleted));
  }

  EXPECT_TRUE(runner.Start());  // triggering action
  EXPECT_TRUE(WaitForCompletion(runner, msec(20)));
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kCompleted);
  EXPECT_FALSE(runner.IsBusy());
}

//! Task is failing during execution.

TEST_F(FunctionRunnerTest, TaskFailingDuringExecution)
{
  testutils::MockRunnerListener listener;
  auto worker = []()
  {
    throw std::runtime_error("Failed during runtime");
    return true;
  };
  FunctionRunner runner(worker, listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kFailed));
  }

  EXPECT_TRUE(runner.Start());  // triggering action
  EXPECT_TRUE(WaitForCompletion(runner, msec(20)));
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kFailed);
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
  EXPECT_EQ(runner->GetStatus(), RunnerStatus::kRunning);
  EXPECT_TRUE(ncount > 0);

  ASSERT_NO_FATAL_FAILURE(runner.reset());
  // event loop was interrupted, thread was succesfully joined
}

//! Terminating the procedure that runs too long.

TEST_F(FunctionRunnerTest, StartAndTerminate)
{
  testutils::MockRunnerListener listener;
  auto worker = []()
  {
    std::this_thread::sleep_for(msec(10));
    return true;
  };  // executes forever
  FunctionRunner runner(worker, listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kStopping));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kStopped));
  }

  EXPECT_TRUE(runner.Start());  // triggering action
  EXPECT_TRUE(runner.IsBusy());
  std::this_thread::sleep_for(msec(20));

  EXPECT_FALSE(WaitForCompletion(runner, msec(10)));

  runner.Stop();
  std::this_thread::sleep_for(msec(10));

  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kStopped);
}

//! Terminating the procedure that runs too long.

TEST_F(FunctionRunnerTest, PrematureDeletion)
{
  testutils::MockRunnerListener listener;
  auto worker = []()
  {
    std::this_thread::sleep_for(msec(10));
    return true;
  };  // executes forever

  auto runner = std::make_unique<FunctionRunner>(worker, listener.CreateCallback());

  // The feature of FunctionRunner that the last signal is RunningState on sudden destruction
  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
  }

  EXPECT_TRUE(runner->Start());  // triggering action
  EXPECT_TRUE(runner->IsBusy());
  std::this_thread::sleep_for(msec(20));

  EXPECT_FALSE(WaitForCompletion(*runner, msec(10)));

  EXPECT_NO_FATAL_FAILURE(runner.reset());
}


//! Stepwise task execution. The task is launched in step wise mode. After 3 steps the task is
//! expected to be completed.

TEST_F(FunctionRunnerTest, StepwiseExecutionAndNormalCompletion)
{
  int nsteps{0};
  auto worker = [&nsteps]()
  {
    std::this_thread::sleep_for(msec(10));
    nsteps++;
    return nsteps < 3;  // should stop when nsteps==3
  };

  FunctionRunner runner(worker);

  EXPECT_TRUE(runner.Step());
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kRunning);
  EXPECT_TRUE(runner.IsBusy());
  std::this_thread::sleep_for(msec(20));
  EXPECT_EQ(nsteps, 1);
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kPaused);
  EXPECT_TRUE(runner.IsBusy());

  runner.Step();
  std::this_thread::sleep_for(msec(20));
  EXPECT_EQ(nsteps, 2);
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kPaused);
  EXPECT_TRUE(runner.IsBusy());

  runner.Step();
  std::this_thread::sleep_for(msec(20));
  EXPECT_EQ(nsteps, 3);
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kCompleted);
  EXPECT_FALSE(runner.IsBusy());
}

//! Stepwise task execution (checking signals). The task is launched in step wise mode. After 2
//! steps the task is expected to be completed.

TEST_F(FunctionRunnerTest, SignalingDuringStepwiseExecutionAndNormalCompletion)
{
  testutils::MockRunnerListener listener;
  int nsteps{0};
  auto worker = [&nsteps]()
  {
    std::this_thread::sleep_for(msec(10));
    nsteps++;
    return nsteps < 2;  // should stop when nsteps==2
  };

  FunctionRunner runner(worker, listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kPaused));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kCompleted));
  }

  EXPECT_TRUE(runner.Step());  // triggering action
  std::this_thread::sleep_for(msec(20));
  EXPECT_TRUE(runner.IsBusy());
  EXPECT_EQ(nsteps, 1);
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kPaused);
  EXPECT_TRUE(runner.IsBusy());

  runner.Step();
  std::this_thread::sleep_for(msec(20));
  EXPECT_EQ(nsteps, 2);
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kCompleted);
  EXPECT_FALSE(runner.IsBusy());
}

//! Stepwise task execution. The task is launched in step wise mode. After
//! second step it is stopped.

TEST_F(FunctionRunnerTest, TerminateDuringStepwiseExecution)
{
  testutils::MockRunnerListener listener;
  int nsteps{0};
  auto worker = [&nsteps]()
  {
    std::this_thread::sleep_for(msec(10));
    nsteps++;
    return nsteps < 2;  // should stop when nsteps==2
  };

  FunctionRunner runner(worker, listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kPaused));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kStopping));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kStopped));
  }

  EXPECT_TRUE(runner.Step());  // triggering action
  std::this_thread::sleep_for(msec(20));
  EXPECT_TRUE(runner.IsBusy());
  EXPECT_EQ(nsteps, 1);
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kPaused);

  // let's terminate while being in Pause mode
  runner.Stop();

  std::this_thread::sleep_for(msec(20));
  EXPECT_EQ(nsteps, 1);
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kStopped);
  EXPECT_FALSE(runner.IsBusy());
}

//! We start normally, then make a pause, then continue till normal termination.

TEST_F(FunctionRunnerTest, RunPauseRun)
{
  testutils::MockRunnerListener listener;
  int nsteps{0};
  bool is_continue{true};
  auto worker = [&nsteps, &is_continue]()
  {
    std::this_thread::sleep_for(msec(5));
    nsteps++;
    return is_continue;
  };

  FunctionRunner runner(worker, listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kPaused));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kCompleted));
  }

  EXPECT_TRUE(runner.Start());  // triggering action
  std::this_thread::sleep_for(msec(20));
  EXPECT_TRUE(runner.IsBusy());
  EXPECT_TRUE(nsteps > 0);
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kRunning);

  // let's pause
  runner.Pause();
  std::this_thread::sleep_for(msec(10));
  auto last_step = nsteps;
  EXPECT_TRUE(runner.IsBusy());
  std::this_thread::sleep_for(msec(20));

  EXPECT_EQ(last_step, nsteps);  // it is not running anymore
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kPaused);

  // releasing waiting
  runner.Start();
  std::this_thread::sleep_for(msec(10));
  is_continue = false;  // terminating in natural way
  EXPECT_TRUE(WaitForCompletion(runner, msec(20)));
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kCompleted);
  EXPECT_TRUE(nsteps > last_step);
}

//! We start normally, then make a pause, then step, then continue till normal termination.

TEST_F(FunctionRunnerTest, RunPauseStepRun)
{
  testutils::MockRunnerListener listener;
  int nsteps{0};
  bool is_continue{true};
  auto worker = [&nsteps, &is_continue]()
  {
    std::this_thread::sleep_for(msec(5));
    nsteps++;
    return is_continue;
  };

  FunctionRunner runner(worker, listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kPaused));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kPaused));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kCompleted));
  }

  EXPECT_TRUE(runner.Start());  // triggering action
  std::this_thread::sleep_for(msec(20));
  EXPECT_TRUE(runner.IsBusy());
  EXPECT_TRUE(nsteps > 0);
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kRunning);

  // let's pause
  runner.Pause();
  std::this_thread::sleep_for(msec(10));
  auto last_step = nsteps;
  EXPECT_TRUE(runner.IsBusy());
  std::this_thread::sleep_for(msec(20));

  EXPECT_EQ(last_step, nsteps);  // it is not running anymore
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kPaused);

  // making step
  runner.Step();
  std::this_thread::sleep_for(msec(20));
  EXPECT_EQ(nsteps, last_step + 1);
  EXPECT_TRUE(runner.IsBusy());
  last_step = nsteps;

  // releasing waiting
  runner.Start();
  std::this_thread::sleep_for(msec(10));
  is_continue = false;  // terminating in natural way
  EXPECT_TRUE(WaitForCompletion(runner, msec(20)));
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kCompleted);
  EXPECT_TRUE(nsteps > last_step);
}
