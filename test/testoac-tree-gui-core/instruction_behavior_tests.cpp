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

#include "oac_tree_gui/model/instruction_behavior.h"

#include <oac_tree_gui/core/exceptions.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for InstructionBehavior.
 */
class InstructionBehaviorTest : public ::testing::Test
{
};

TEST_F(InstructionBehaviorTest, ToString)
{
  EXPECT_EQ(ToString(InstructionBehavior::kNative), "Native");
  EXPECT_EQ(ToString(InstructionBehavior::kHidden), "Hidden");
  EXPECT_EQ(ToString(InstructionBehavior::kSucceed), "Succeed");
  EXPECT_EQ(ToString(InstructionBehavior::kFail), "Fail");
}

TEST_F(InstructionBehaviorTest, GetInstructionBehavior)
{
  EXPECT_EQ(GetInstructionBehavior("Native"), InstructionBehavior::kNative);
  EXPECT_EQ(GetInstructionBehavior("Hidden"), InstructionBehavior::kHidden);
  EXPECT_EQ(GetInstructionBehavior("Succeed"), InstructionBehavior::kSucceed);
  EXPECT_EQ(GetInstructionBehavior("Fail"), InstructionBehavior::kFail);
  EXPECT_THROW(GetInstructionBehavior(""), RuntimeException);
  EXPECT_THROW(GetInstructionBehavior("abc"), RuntimeException);
}

}  // namespace oac_tree_gui::test
