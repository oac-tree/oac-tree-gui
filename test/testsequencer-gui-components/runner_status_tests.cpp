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

#include "sequencergui/jobsystem/runner_status.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for utility methods in runner_status.h

class RunnerStatusTest : public ::testing::Test
{
};

TEST_F(RunnerStatusTest, RunnerStatusToString)
{
  EXPECT_EQ(RunnerStatusToString(RunnerStatus::kInitial), "Initial");
  EXPECT_EQ(RunnerStatusToString(RunnerStatus::kPaused), "Paused");
  EXPECT_EQ(RunnerStatusToString(RunnerStatus::kStepping), "Stepping");
  EXPECT_EQ(RunnerStatusToString(RunnerStatus::kRunning), "Running");
  EXPECT_EQ(RunnerStatusToString(RunnerStatus::kSucceeded), "Success");
  EXPECT_EQ(RunnerStatusToString(RunnerStatus::kFailed), "Failure");
  EXPECT_EQ(RunnerStatusToString(RunnerStatus::kHalted), "Halted");
}

TEST_F(RunnerStatusTest, GetRunnerStatus)
{
  EXPECT_EQ(GetRunnerStatus("Initial"), RunnerStatus::kInitial);
  EXPECT_EQ(GetRunnerStatus("Paused"), RunnerStatus::kPaused);
  EXPECT_EQ(GetRunnerStatus("Stepping"), RunnerStatus::kStepping);
  EXPECT_EQ(GetRunnerStatus("Running"), RunnerStatus::kRunning);
  EXPECT_EQ(GetRunnerStatus("Success"), RunnerStatus::kSucceeded);
  EXPECT_EQ(GetRunnerStatus("Failure"), RunnerStatus::kFailed);
  EXPECT_EQ(GetRunnerStatus("Halted"), RunnerStatus::kHalted);
}
