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

#include "sequencergui/jobsystem/job_utils.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for SequencerObserver class.

class JobUtilsTest : public ::testing::Test
{
};

TEST_F(JobUtilsTest, RunnerStatusToString)
{
  EXPECT_EQ(RunnerStatusToString(RunnerStatus::kIdle), "Idle");
  EXPECT_EQ(RunnerStatusToString(RunnerStatus::kRunning), "Running");
  EXPECT_EQ(RunnerStatusToString(RunnerStatus::kPaused), "Paused");
  EXPECT_EQ(RunnerStatusToString(RunnerStatus::kCompleted), "Completed");
  EXPECT_EQ(RunnerStatusToString(RunnerStatus::kStopping), "Stopping");
  EXPECT_EQ(RunnerStatusToString(RunnerStatus::kStopped), "Stopped");
  EXPECT_EQ(RunnerStatusToString(RunnerStatus::kFailed), "Failed");
}

TEST_F(JobUtilsTest, GetRunnerStatus)
{
  EXPECT_EQ(GetRunnerStatus("Idle"), RunnerStatus::kIdle);
  EXPECT_EQ(GetRunnerStatus("Running"), RunnerStatus::kRunning);
  EXPECT_EQ(GetRunnerStatus("Paused"), RunnerStatus::kPaused);
  EXPECT_EQ(GetRunnerStatus("Completed"), RunnerStatus::kCompleted);
  EXPECT_EQ(GetRunnerStatus("Stopping"), RunnerStatus::kStopping);
  EXPECT_EQ(GetRunnerStatus("Stopped"), RunnerStatus::kStopped);
  EXPECT_EQ(GetRunnerStatus("Failed"), RunnerStatus::kFailed);
}
