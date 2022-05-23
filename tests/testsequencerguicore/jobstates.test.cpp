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

#include "sequencergui/jobsystem/jobstates.h"

#include "sequencergui/jobsystem/abstractjob.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for JobStates.

class JobStatesTest : public ::testing::Test
{
public:
  class MockJob : public sequencergui::AbstractJob
  {
  public:
    MOCK_METHOD0(Start, void(void));
    MOCK_METHOD0(Pause, void(void));
    MOCK_METHOD0(Step, void(void));
    MOCK_METHOD0(Stop, void(void));
  };
};

TEST_F(JobStatesTest, IdleState)
{
  IdleState state;
  EXPECT_EQ(state.GetStatus(), RunnerStatus::kIdle);

  {  // starting job
    MockJob job;
    EXPECT_CALL(job, Start()).Times(1);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    auto result = state.Handle(JobAction::kStart, &job);
    ASSERT_TRUE(result.get());  // trigger action
    EXPECT_EQ(result->GetStatus(), RunnerStatus::kRunning);
  }

  {  // Pausing job
    MockJob job;
    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_FALSE(state.Handle(JobAction::kPause, &job));  // trigger action
  }

  {  // Make a step
    MockJob job;
    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_FALSE(state.Handle(JobAction::kStep, &job));  // trigger action
  }

  {  // Make a stop
    MockJob job;
    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_FALSE(state.Handle(JobAction::kStop, &job));  // trigger action
  }
}

TEST_F(JobStatesTest, RunningState)
{
  RunningState state;
  EXPECT_EQ(state.GetStatus(), RunnerStatus::kRunning);

  {  // starting job
    MockJob job;
    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_FALSE(state.Handle(JobAction::kStart, &job));  // trigger action
  }

  {  // Pausing job
    MockJob job;
    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(1);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    auto result = state.Handle(JobAction::kPause, &job);
    ASSERT_TRUE(result.get());  // trigger action
    EXPECT_EQ(result->GetStatus(), RunnerStatus::kPaused);
  }

  {  // Make a step
    MockJob job;
    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Step()).Times(1);
    EXPECT_CALL(job, Stop()).Times(0);
    auto result = state.Handle(JobAction::kStep, &job);
    ASSERT_TRUE(result.get());  // trigger action
    EXPECT_EQ(result->GetStatus(), RunnerStatus::kPaused);
  }

  {  // Make a stop
    MockJob job;
    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(1);
    auto result = state.Handle(JobAction::kStop, &job);
    ASSERT_TRUE(result.get());  // trigger action
    EXPECT_EQ(result->GetStatus(), RunnerStatus::kFailed);
  }
}

TEST_F(JobStatesTest, PausedState)
{
  PausedState state;
  EXPECT_EQ(state.GetStatus(), RunnerStatus::kPaused);

  {  // starting job
    MockJob job;
    EXPECT_CALL(job, Start()).Times(1);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    auto result = state.Handle(JobAction::kStart, &job);
    ASSERT_TRUE(result.get());  // trigger action
    EXPECT_EQ(result->GetStatus(), RunnerStatus::kRunning);
  }

  {  // Pausing job
    MockJob job;
    EXPECT_CALL(job, Start()).Times(0);
    EXPECT_CALL(job, Pause()).Times(0);
    EXPECT_CALL(job, Step()).Times(0);
    EXPECT_CALL(job, Stop()).Times(0);
    EXPECT_FALSE(state.Handle(JobAction::kPause, &job));  // trigger action
  }

//  {  // Make a step
//    MockJob job;
//    EXPECT_CALL(job, Start()).Times(0);
//    EXPECT_CALL(job, Pause()).Times(0);
//    EXPECT_CALL(job, Step()).Times(0);
//    EXPECT_CALL(job, Stop()).Times(0);
//    EXPECT_FALSE(state.Handle(JobAction::kStep, &job));  // trigger action
//  }

//  {  // Make a stop
//    MockJob job;
//    EXPECT_CALL(job, Start()).Times(0);
//    EXPECT_CALL(job, Pause()).Times(0);
//    EXPECT_CALL(job, Step()).Times(0);
//    EXPECT_CALL(job, Stop()).Times(0);
//    EXPECT_FALSE(state.Handle(JobAction::kStop, &job));  // trigger action
//  }

}

TEST_F(JobStatesTest, CompletedState)
{
  CompletedState state;
  EXPECT_EQ(state.GetStatus(), RunnerStatus::kCompleted);
}

TEST_F(JobStatesTest, CancelingState)
{
  CancelingState state;
  EXPECT_EQ(state.GetStatus(), RunnerStatus::kCanceling);
}

TEST_F(JobStatesTest, CanceledState)
{
  CanceledState state;
  EXPECT_EQ(state.GetStatus(), RunnerStatus::kCanceled);
}

TEST_F(JobStatesTest, FailedState)
{
  FailedState state;
  EXPECT_EQ(state.GetStatus(), RunnerStatus::kFailed);
}
