/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "oac_tree_gui/transform/transform_from_domain.h"

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

class TransformFromDomainTest : public ::testing::Test
{
};

TEST_F(TransformFromDomainTest, GetRunnerStatusFromDomain)
{
  using sup::oac_tree::JobState;

  // to trace the mapping between domain JobState and GUI RunnerStatus
  EXPECT_EQ(static_cast<RunnerStatus>(JobState::kInitial), RunnerStatus::kInitial);
  EXPECT_EQ(static_cast<RunnerStatus>(JobState::kPaused), RunnerStatus::kPaused);
  EXPECT_EQ(static_cast<RunnerStatus>(JobState::kStepping), RunnerStatus::kStepping);
  EXPECT_EQ(static_cast<RunnerStatus>(JobState::kRunning), RunnerStatus::kRunning);
  EXPECT_EQ(static_cast<RunnerStatus>(JobState::kSucceeded), RunnerStatus::kSucceeded);
  EXPECT_EQ(static_cast<RunnerStatus>(JobState::kFailed), RunnerStatus::kFailed);
  EXPECT_EQ(static_cast<RunnerStatus>(JobState::kHalted), RunnerStatus::kHalted);

  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kInitial), RunnerStatus::kInitial);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kPaused), RunnerStatus::kPaused);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kStepping), RunnerStatus::kStepping);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kRunning), RunnerStatus::kRunning);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kSucceeded), RunnerStatus::kSucceeded);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kFailed), RunnerStatus::kFailed);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kHalted), RunnerStatus::kHalted);
}

TEST_F(TransformFromDomainTest, GetInstructionStatusFromDomain)
{
  using sup::oac_tree::ExecutionStatus;

  EXPECT_EQ(static_cast<InstructionStatus>(ExecutionStatus::NOT_STARTED),
            InstructionStatus::kNotStarted);
  EXPECT_EQ(static_cast<InstructionStatus>(ExecutionStatus::NOT_FINISHED),
            InstructionStatus::kNotFinished);
  EXPECT_EQ(static_cast<InstructionStatus>(ExecutionStatus::RUNNING), InstructionStatus::kRunning);
  EXPECT_EQ(static_cast<InstructionStatus>(ExecutionStatus::SUCCESS), InstructionStatus::kSuccess);
  EXPECT_EQ(static_cast<InstructionStatus>(ExecutionStatus::FAILURE), InstructionStatus::kFailure);
  EXPECT_EQ(static_cast<InstructionStatus>(ExecutionStatus::HALTED), InstructionStatus::kHalted);

  EXPECT_EQ(GetInstructionStatusFromDomain(ExecutionStatus::NOT_STARTED),
            InstructionStatus::kNotStarted);
  EXPECT_EQ(GetInstructionStatusFromDomain(ExecutionStatus::NOT_FINISHED),
            InstructionStatus::kNotFinished);
  EXPECT_EQ(GetInstructionStatusFromDomain(ExecutionStatus::RUNNING), InstructionStatus::kRunning);
  EXPECT_EQ(GetInstructionStatusFromDomain(ExecutionStatus::SUCCESS), InstructionStatus::kSuccess);
  EXPECT_EQ(GetInstructionStatusFromDomain(ExecutionStatus::FAILURE), InstructionStatus::kFailure);
  EXPECT_EQ(GetInstructionStatusFromDomain(ExecutionStatus::HALTED), InstructionStatus::kHalted);
}

}  // namespace oac_tree_gui::test
