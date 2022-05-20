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

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for JobStates.

class JobStatesTest : public ::testing::Test
{
};

TEST_F(JobStatesTest, IdleState)
{
  IdleState state;
  EXPECT_EQ(state.GetStatus(), RunnerStatus::kIdle);
}

TEST_F(JobStatesTest, RunningState)
{
  RunningState state;
  EXPECT_EQ(state.GetStatus(), RunnerStatus::kRunning);
}

TEST_F(JobStatesTest, PausedState)
{
  PausedState state;
  EXPECT_EQ(state.GetStatus(), RunnerStatus::kPaused);
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
