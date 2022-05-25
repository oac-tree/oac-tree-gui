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

#include "sequencergui/jobsystem/abstractjob.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for AbstractJob.

class AbstractJobTest : public ::testing::Test
{
public:
  class TestJob : public sequencergui::AbstractJob
  {
  public:
    MOCK_METHOD0(StartRequest, void(void));
    MOCK_METHOD0(PauseModeOnRequest, void(void));
    MOCK_METHOD0(PauseModeOffRequest, void(void));
    MOCK_METHOD0(StepRequest, void(void));
    MOCK_METHOD0(StopRequest, void(void));
  };
};

TEST_F(AbstractJobTest, InitialState)
{
  TestJob job;
  EXPECT_EQ(job.GetStatus(), RunnerStatus::kIdle);
}

//! The transition of TestJob from idle state to all other states.
//! In fact, no change in state occurs, we are testing various requests.

TEST_F(AbstractJobTest, FromIdle)
{
  {  // IdleState + StartAction -> StartRequest
    TestJob job;
    EXPECT_CALL(job, StartRequest()).Times(1);
    EXPECT_CALL(job, PauseModeOnRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOffRequest()).Times(0);
    EXPECT_CALL(job, StepRequest()).Times(0);
    EXPECT_CALL(job, StopRequest()).Times(0);
    EXPECT_TRUE(job.Start());
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kIdle);
  }

  {  // IdleState + PauseAction -> no activity
    TestJob job;
    EXPECT_CALL(job, StartRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOnRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOffRequest()).Times(0);
    EXPECT_CALL(job, StepRequest()).Times(0);
    EXPECT_CALL(job, StopRequest()).Times(0);
    EXPECT_FALSE(job.Pause());
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kIdle);
  }

  {  // IdleState + StepAction -> PauseModeOnRequest, StartRequest
    TestJob job;
    {
      EXPECT_CALL(job, PauseModeOnRequest()).Times(1);
      EXPECT_CALL(job, StartRequest()).Times(1);
    }

    EXPECT_CALL(job, PauseModeOffRequest()).Times(0);
    EXPECT_CALL(job, StepRequest()).Times(0);
    EXPECT_CALL(job, StopRequest()).Times(0);
    EXPECT_TRUE(job.Step());
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kIdle);
  }

  {  // IdleState + StopAction -> no activity
    TestJob job;
    EXPECT_CALL(job, StartRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOnRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOffRequest()).Times(0);
    EXPECT_CALL(job, StepRequest()).Times(0);
    EXPECT_CALL(job, StopRequest()).Times(0);
    EXPECT_FALSE(job.Stop());
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kIdle);
  }
}

//! The transition of TestJob from running state to all other states.

TEST_F(AbstractJobTest, FromRunning)
{
  {  // RunningState + StartAction -> no activity
    TestJob job;
    job.SetStatus(RunnerStatus::kRunning);

    EXPECT_CALL(job, StartRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOnRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOffRequest()).Times(0);
    EXPECT_CALL(job, StepRequest()).Times(0);
    EXPECT_CALL(job, StopRequest()).Times(0);
    EXPECT_FALSE(job.Start());
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kRunning);
  }

  {  // RunningState + PauseAction -> PauseModeOnRequest
    TestJob job;
    job.SetStatus(RunnerStatus::kRunning);

    EXPECT_CALL(job, StartRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOnRequest()).Times(1);
    EXPECT_CALL(job, PauseModeOffRequest()).Times(0);
    EXPECT_CALL(job, StepRequest()).Times(0);
    EXPECT_CALL(job, StopRequest()).Times(0);
    EXPECT_TRUE(job.Pause());
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kRunning);
  }

  {  // RunningState + StepAction -> no activity
    TestJob job;
    job.SetStatus(RunnerStatus::kRunning);

    EXPECT_CALL(job, StartRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOnRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOffRequest()).Times(0);
    EXPECT_CALL(job, StepRequest()).Times(0);
    EXPECT_CALL(job, StopRequest()).Times(0);
    EXPECT_FALSE(job.Step());
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kRunning);
  }

  {  // RunningState + StopAction -> StopRequest
    TestJob job;
    job.SetStatus(RunnerStatus::kRunning);

    EXPECT_CALL(job, StartRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOnRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOffRequest()).Times(0);
    EXPECT_CALL(job, StepRequest()).Times(0);
    EXPECT_CALL(job, StopRequest()).Times(1);
    EXPECT_TRUE(job.Stop());
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kRunning);
  }
}

//! The transition of TestJob from paused state to all other states.

TEST_F(AbstractJobTest, FromPaused)
{
  {  // PausedState + StartAction -> PauseModeOffRequest (+ automatic release)
    TestJob job;
    job.SetStatus(RunnerStatus::kPaused);

    EXPECT_CALL(job, StartRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOnRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOffRequest()).Times(1);
    EXPECT_CALL(job, StepRequest()).Times(0);
    EXPECT_CALL(job, StopRequest()).Times(0);
    EXPECT_TRUE(job.Start());
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kPaused);
  }

  {  // PausedState + PauseAction -> no activity
    TestJob job;
    job.SetStatus(RunnerStatus::kPaused);

    EXPECT_CALL(job, StartRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOnRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOffRequest()).Times(0);
    EXPECT_CALL(job, StepRequest()).Times(0);
    EXPECT_CALL(job, StopRequest()).Times(0);
    EXPECT_FALSE(job.Pause());
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kPaused);
  }

  {  // PausedSate + StepAction -> StepRequest
    TestJob job;
    job.SetStatus(RunnerStatus::kPaused);

    EXPECT_CALL(job, StartRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOnRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOffRequest()).Times(0);
    EXPECT_CALL(job, StepRequest()).Times(1);
    EXPECT_CALL(job, StopRequest()).Times(0);
    EXPECT_TRUE(job.Step());
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kPaused);
  }

  {  // PausedState + StopAction -> StopRequest
    TestJob job;
    job.SetStatus(RunnerStatus::kPaused);

    EXPECT_CALL(job, StartRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOnRequest()).Times(0);
    EXPECT_CALL(job, PauseModeOffRequest()).Times(0);
    EXPECT_CALL(job, StepRequest()).Times(0);
    EXPECT_CALL(job, StopRequest()).Times(1);
    EXPECT_TRUE(job.Stop());
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kPaused);
  }
}
