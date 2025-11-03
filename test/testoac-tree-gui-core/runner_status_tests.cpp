/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/model/runner_status.h"

#include <oac_tree_gui/core/exceptions.h>

#include <sup/oac-tree/job_states.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

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
  EXPECT_EQ(ToString(RunnerStatus::kSubmitFailure), "SubmitFailure");
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
  EXPECT_EQ(GetRunnerStatus("SubmitFailure"), RunnerStatus::kSubmitFailure);
  EXPECT_THROW(GetRunnerStatus("abc"), RuntimeException);
}

TEST_F(RunnerStatusTest, CompareDomainJobState)
{
  using sup::oac_tree::JobState;

  // mapping between domain JobState and GUI RunnerStatus
  EXPECT_EQ(static_cast<std::underlying_type_t<JobState>>(JobState::kInitial),
            static_cast<std::underlying_type_t<RunnerStatus>>(RunnerStatus::kInitial));
  EXPECT_EQ(static_cast<std::underlying_type_t<JobState>>(JobState::kPaused),
            static_cast<std::underlying_type_t<RunnerStatus>>(RunnerStatus::kPaused));
  EXPECT_EQ(static_cast<std::underlying_type_t<JobState>>(JobState::kStepping),
            static_cast<std::underlying_type_t<RunnerStatus>>(RunnerStatus::kStepping));
  EXPECT_EQ(static_cast<std::underlying_type_t<JobState>>(JobState::kRunning),
            static_cast<std::underlying_type_t<RunnerStatus>>(RunnerStatus::kRunning));
  EXPECT_EQ(static_cast<std::underlying_type_t<JobState>>(JobState::kSucceeded),
            static_cast<std::underlying_type_t<RunnerStatus>>(RunnerStatus::kSucceeded));
  EXPECT_EQ(static_cast<std::underlying_type_t<JobState>>(JobState::kFailed),
            static_cast<std::underlying_type_t<RunnerStatus>>(RunnerStatus::kFailed));
  EXPECT_EQ(static_cast<std::underlying_type_t<JobState>>(JobState::kHalted),
            static_cast<std::underlying_type_t<RunnerStatus>>(RunnerStatus::kHalted));

  EXPECT_EQ(static_cast<RunnerStatus>(JobState::kInitial), RunnerStatus::kInitial);
  EXPECT_EQ(static_cast<RunnerStatus>(JobState::kPaused), RunnerStatus::kPaused);
  EXPECT_EQ(static_cast<RunnerStatus>(JobState::kStepping), RunnerStatus::kStepping);
  EXPECT_EQ(static_cast<RunnerStatus>(JobState::kRunning), RunnerStatus::kRunning);
  EXPECT_EQ(static_cast<RunnerStatus>(JobState::kSucceeded), RunnerStatus::kSucceeded);
  EXPECT_EQ(static_cast<RunnerStatus>(JobState::kFailed), RunnerStatus::kFailed);
  EXPECT_EQ(static_cast<RunnerStatus>(JobState::kHalted), RunnerStatus::kHalted);
}

}  // namespace oac_tree_gui::test
