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

#include "oac_tree_gui/model/instruction_status.h"

#include <oac_tree_gui/core/exceptions.h>

#include <sup/oac-tree/job_states.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for InstructionStatus.
 */
class InstructionStatusTest : public ::testing::Test
{
};

TEST_F(InstructionStatusTest, ToString)
{
  EXPECT_EQ(ToString(InstructionStatus::kNotStarted), "Not started");
  EXPECT_EQ(ToString(InstructionStatus::kNotFinished), "Not finished");
  EXPECT_EQ(ToString(InstructionStatus::kRunning), "Running");
  EXPECT_EQ(ToString(InstructionStatus::kSuccess), "Success");
  EXPECT_EQ(ToString(InstructionStatus::kFailure), "Failure");
  EXPECT_EQ(ToString(InstructionStatus::kUndefined), "");
}

TEST_F(InstructionStatusTest, GetRunnerStatus)
{
  EXPECT_EQ(GetInstructionStatus("Not started"), InstructionStatus::kNotStarted);
  EXPECT_EQ(GetInstructionStatus("Not finished"), InstructionStatus::kNotFinished);
  EXPECT_EQ(GetInstructionStatus("Running"), InstructionStatus::kRunning);
  EXPECT_EQ(GetInstructionStatus("Success"), InstructionStatus::kSuccess);
  EXPECT_EQ(GetInstructionStatus("Failure"), InstructionStatus::kFailure);
  EXPECT_EQ(GetInstructionStatus(""), InstructionStatus::kUndefined);
  EXPECT_THROW(GetInstructionStatus("abc"), RuntimeException);
}

}  // namespace oac_tree_gui::test
