/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/jobsystem/function_runner.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_callback_listener.h>
#include <testutils/test_utils.h>

#include <stdexcept>
#include <thread>

using namespace sequencergui;
using msec = std::chrono::milliseconds;

//! Tests for JobStates.

class FunctionRunnerTest : public ::testing::Test
{
public:
  using listener_t = testutils::MockCallbackListener<sequencergui::RunnerStatus>;
  listener_t m_listener;
};

//! Checking that listener works as expected.

TEST_F(FunctionRunnerTest, CheckListener)
{
  EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));

  auto callback = m_listener.CreateCallback();
  callback(RunnerStatus::kRunning);
}

//! Initial state of FunctionRunner.

TEST_F(FunctionRunnerTest, InitialState)
{
  const auto timeout = msec(50);

  FunctionRunner runner([]() { return false; });
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kIdle);
  EXPECT_FALSE(runner.IsBusy());
  EXPECT_TRUE(testutils::WaitForCompletion(runner, timeout));
}

//! Start and normal completion of the short task.

TEST_F(FunctionRunnerTest, ShortTaskNormalCompletion)
{
  const auto timeout = msec(50);

  auto worker = []() { return false; };  // worker asks to exit immediately
  FunctionRunner runner(worker, m_listener.CreateCallback());

  {  // expecting calls with status change in this order
    const ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kCompleted));
  }

  EXPECT_TRUE(runner.Start());  // triggering action
  EXPECT_TRUE(testutils::WaitForCompletion(runner, timeout));
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kCompleted);
  EXPECT_FALSE(runner.IsBusy());
}

//! Task is failing during execution.

TEST_F(FunctionRunnerTest, TaskFailingDuringExecution)
{
  const auto timeout = msec(50);

  auto worker = []()
  {
    throw std::runtime_error("Failed during runtime");
    return true;
  };
  FunctionRunner runner(worker, m_listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kFailed));
  }

  EXPECT_TRUE(runner.Start());  // triggering action
  EXPECT_TRUE(testutils::WaitForCompletion(runner, timeout));
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
  auto worker = []()
  {
    std::this_thread::sleep_for(msec(10));
    return true;
  };  // executes forever
  FunctionRunner runner(worker, m_listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kStopping));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kStopped));
  }

  EXPECT_TRUE(runner.Start());  // triggering action
  EXPECT_TRUE(runner.IsBusy());
  std::this_thread::sleep_for(msec(20));

  EXPECT_FALSE(testutils::WaitForCompletion(runner, msec(10)));

  runner.Stop();
  std::this_thread::sleep_for(msec(10));

  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kStopped);
}

//! Terminating the procedure that runs too long.

TEST_F(FunctionRunnerTest, PrematureDeletion)
{
  auto worker = []()
  {
    std::this_thread::sleep_for(msec(10));
    return true;
  };  // executes forever

  auto runner = std::make_unique<FunctionRunner>(worker, m_listener.CreateCallback());

  // The feature of FunctionRunner that the last signal is RunningState on sudden destruction
  {
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
  }

  EXPECT_TRUE(runner->Start());  // triggering action
  EXPECT_TRUE(runner->IsBusy());
  std::this_thread::sleep_for(msec(20));

  EXPECT_FALSE(testutils::WaitForCompletion(*runner, msec(10)));

  EXPECT_NO_FATAL_FAILURE(runner.reset());
}

//! Stepwise task execution. The task is launched in step wise mode. After 3 steps the task is
//! expected to be completed.

TEST_F(FunctionRunnerTest, StepwiseExecutionAndNormalCompletion)
{
  int nsteps{0};
  auto worker = [&nsteps]()
  {
    std::this_thread::sleep_for(msec(5));
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
  int nsteps{0};
  auto worker = [&nsteps]()
  {
    std::this_thread::sleep_for(msec(5));
    nsteps++;
    return nsteps < 2;  // should stop when nsteps==2
  };

  FunctionRunner runner(worker, m_listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kPaused));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kCompleted));
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
  int nsteps{0};
  auto worker = [&nsteps]()
  {
    std::this_thread::sleep_for(msec(10));
    nsteps++;
    return nsteps < 2;  // should stop when nsteps==2
  };

  FunctionRunner runner(worker, m_listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kPaused));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kStopping));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kStopped));
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
  int nsteps{0};
  bool is_continue{true};
  auto worker = [&nsteps, &is_continue]()
  {
    std::this_thread::sleep_for(msec(5));
    nsteps++;
    return is_continue;
  };

  FunctionRunner runner(worker, m_listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kPaused));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kCompleted));
  }

  EXPECT_TRUE(runner.Start());  // triggering action
  std::this_thread::sleep_for(msec(20));

  auto predicate = [&nsteps, &runner]() { return (runner.IsBusy() && nsteps > 0); };
  EXPECT_TRUE(testutils::WaitFor(predicate, msec(50)));
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
  EXPECT_TRUE(testutils::WaitForCompletion(runner, msec(50)));
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kCompleted);
  EXPECT_TRUE(nsteps > last_step);
}

//! We start normally, then make a pause, then step, then continue till normal termination.

TEST_F(FunctionRunnerTest, RunPauseStepRun)
{
  int nsteps{0};
  bool is_continue{true};
  auto worker = [&nsteps, &is_continue]()
  {
    std::this_thread::sleep_for(msec(5));
    nsteps++;
    return is_continue;
  };

  FunctionRunner runner(worker, m_listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kPaused));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kPaused));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kCompleted));
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
  EXPECT_TRUE(testutils::WaitForCompletion(runner, msec(50)));
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kCompleted);
  EXPECT_TRUE(nsteps > last_step);
}

//! We start short task and let if finish. Then we run it again.
//! Here we check the use case of running same job one after another.

TEST_F(FunctionRunnerTest, TwoConsequitiveRuns)
{
  int nsteps{0};
  bool is_continue{true};
  auto worker = [&nsteps, &is_continue]()
  {
    std::this_thread::sleep_for(msec(5));
    nsteps++;
    return nsteps % 10 != 0;  // will exit after each 10 counts
  };

  FunctionRunner runner(worker, m_listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kCompleted));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kCompleted));
  }

  EXPECT_TRUE(runner.Start());  // triggering action

  auto predicate = [&nsteps, &runner]() { return (runner.IsBusy() && nsteps > 0); };
  EXPECT_TRUE(testutils::WaitFor(predicate, msec(50)));
  EXPECT_TRUE(runner.IsBusy());
  EXPECT_TRUE(nsteps > 0);
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kRunning);

  // waiting until completion
  EXPECT_TRUE(testutils::WaitForCompletion(runner, msec(100)));
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(nsteps, 10);

  // running again should
  EXPECT_TRUE(runner.Start());  // triggering action
  EXPECT_TRUE(testutils::WaitFor(predicate, msec(50)));
  EXPECT_TRUE(runner.IsBusy());
  EXPECT_TRUE(nsteps > 0);
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kRunning);

  EXPECT_TRUE(testutils::WaitForCompletion(runner, msec(100)));
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(nsteps, 20);
}

//! We start normally, then make a pause, then stop execution.
//! After that we start again and let it finish.

TEST_F(FunctionRunnerTest, ContinueAfterStopInStepMode)
{
  int nsteps{0};
  auto worker = [&nsteps]()
  {
    std::this_thread::sleep_for(msec(5));
    nsteps++;
    return nsteps / 10 != 1;  // should stop when nsteps==10
  };

  FunctionRunner runner(worker, m_listener.CreateCallback());

  {  // expecting calls with status change in this order
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kPaused));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kStopping));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kStopped));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kRunning));
    EXPECT_CALL(m_listener, OnCallback(RunnerStatus::kCompleted));
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

  // run from the beginning
  nsteps = 0;
  runner.Start();
  EXPECT_TRUE(testutils::WaitForCompletion(runner, msec(100)));
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kCompleted);
  EXPECT_EQ(nsteps, 10);
}
