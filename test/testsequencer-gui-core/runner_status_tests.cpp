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

#include "sequencergui/model/runner_status.h"

#include <sequencergui/core/exceptions.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for utility methods in runner_status.h

class RunnerStatusTest : public ::testing::Test
{
};

TEST_F(RunnerStatusTest, ToString)
{
  EXPECT_EQ(ToString(RunnerStatus::kInitial), "Initial");
  EXPECT_EQ(ToString(RunnerStatus::kPaused), "Paused");
  EXPECT_EQ(ToString(RunnerStatus::kStepping), "Stepping");
  EXPECT_EQ(ToString(RunnerStatus::kRunning), "Running");
  EXPECT_EQ(ToString(RunnerStatus::kSucceeded), "Success");
  EXPECT_EQ(ToString(RunnerStatus::kFailed), "Failure");
  EXPECT_EQ(ToString(RunnerStatus::kHalted), "Halted");
  EXPECT_EQ(ToString(RunnerStatus::kUndefined), "");
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
  EXPECT_THROW(GetRunnerStatus("abc"), RuntimeException);
}
