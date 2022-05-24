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
using ::testing::NiceMock;

//! Tests for AbstractJob.

class AbstractJobTest : public ::testing::Test
{
public:
  class TestJob : public sequencergui::AbstractJob
  {
  public:
    MOCK_METHOD0(Start, void(void));
    MOCK_METHOD0(Pause, void(void));
    MOCK_METHOD0(Release, void(void));
    MOCK_METHOD0(Step, void(void));
    MOCK_METHOD0(Stop, void(void));
  };
};

TEST_F(AbstractJobTest, InitialState)
{
  TestJob job;
  EXPECT_EQ(job.GetStatus(), RunnerStatus::kIdle);
}

//! The transition of TestJob from idle state to all other states.

TEST_F(AbstractJobTest, FromIdle)
{
  {  // to running
    TestJob job;
    EXPECT_CALL(job, Start()).Times(1);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Release()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_TRUE(job.PerformAction(JobAction::kStart));
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kRunning);
  }

  {  // to pause
    TestJob job;
    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Release()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_FALSE(job.PerformAction(JobAction::kPause));
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kIdle);
  }

  {  // to step
    TestJob job;
    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Release()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_FALSE(job.PerformAction(JobAction::kStep));
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kIdle);
  }

  {  // to stop
    TestJob job;
    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Release()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_FALSE(job.PerformAction(JobAction::kStop));
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kIdle);
  }
}

//! The transition of TestJob from running state to all other states.

TEST_F(AbstractJobTest, FromRunning)
{
  {  // to running
    NiceMock<TestJob> job;
    job.PerformAction(JobAction::kStart);

    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Release()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_FALSE(job.PerformAction(JobAction::kStart));
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kRunning);
  }

  {  // to pause
    NiceMock<TestJob> job;
    job.PerformAction(JobAction::kStart);

    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(1);
    EXPECT_CALL(job, Release()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_TRUE(job.PerformAction(JobAction::kPause));
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kPaused);
  }

  {  // to step
    NiceMock<TestJob> job;
    job.PerformAction(JobAction::kStart);

    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Release()).Times(0);
    EXPECT_CALL(job, Step()).Times(1);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_TRUE(job.PerformAction(JobAction::kStep));
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kPaused);
  }

  {  // to stop
    NiceMock<TestJob> job;
    job.PerformAction(JobAction::kStart);

    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Release()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(1);
    EXPECT_TRUE(job.PerformAction(JobAction::kStop));
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kStopped);
  }
}

//! The transition of TestJob from paused state to all other states.

TEST_F(AbstractJobTest, FromPaused)
{
  {  // to running
    NiceMock<TestJob> job;
    job.PerformAction(JobAction::kStart);
    job.PerformAction(JobAction::kPause);  // here we at paused state

    EXPECT_CALL(job, Start()).Times(1);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Release()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_TRUE(job.PerformAction(JobAction::kStart));
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kRunning);
  }
  {  // to pause
    NiceMock<TestJob> job;
    job.PerformAction(JobAction::kStart);
    job.PerformAction(JobAction::kPause);  // here we at paused state

    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Release()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_FALSE(job.PerformAction(JobAction::kPause));
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kPaused);
  }

  {  // to step
    NiceMock<TestJob> job;
    job.PerformAction(JobAction::kStart);
    job.PerformAction(JobAction::kPause);  // here we at paused state

    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Release()).Times(0);
    EXPECT_CALL(job, Step()).Times(1);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_TRUE(job.PerformAction(JobAction::kStep));
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kPaused);
  }

  {  // to stop
    NiceMock<TestJob> job;
    job.PerformAction(JobAction::kStart);
    job.PerformAction(JobAction::kPause);  // here we at paused state

    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Release()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(1);
    EXPECT_TRUE(job.PerformAction(JobAction::kStop));
    EXPECT_EQ(job.GetStatus(), RunnerStatus::kStopped);
  }
}
