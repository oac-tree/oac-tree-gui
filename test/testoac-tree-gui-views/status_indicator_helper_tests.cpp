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

#include "oac_tree_gui/views/operation/status_indicator_helper.h"

#include <gtest/gtest.h>

#include <QGroupBox>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for helper methods in status_indicator_helper.h
 */
class StatusIndicatorHelperTest : public ::testing::Test
{
};

TEST_F(StatusIndicatorHelperTest, GetBusySignTypeFromInstructionStatus)
{
  EXPECT_EQ(GetBusySignTypeForInstructionStatus(InstructionStatus::kNotStarted),
            BusySignType::kHidden);
  EXPECT_EQ(GetBusySignTypeForInstructionStatus(InstructionStatus::kNotFinished),
            BusySignType::kAnimated);
  EXPECT_EQ(GetBusySignTypeForInstructionStatus(InstructionStatus::kRunning),
            BusySignType::kAnimated);
  EXPECT_EQ(GetBusySignTypeForInstructionStatus(InstructionStatus::kSuccess),
            BusySignType::kHidden);
  EXPECT_EQ(GetBusySignTypeForInstructionStatus(InstructionStatus::kFailure),
            BusySignType::kHidden);
  EXPECT_EQ(GetBusySignTypeForInstructionStatus(InstructionStatus::kHalted), BusySignType::kHidden);
  EXPECT_EQ(GetBusySignTypeForInstructionStatus(InstructionStatus::kUndefined),
            BusySignType::kHidden);
}

TEST_F(StatusIndicatorHelperTest, GetColorForInstructionStatus)
{
  // validate that there is a color for each status and that it is not an invalid color
  for (size_t i = 0; i <= static_cast<size_t>(InstructionStatus::kUndefined); ++i)
  {
    const auto status = static_cast<InstructionStatus>(i);
    const QColor color = GetColorForInstructionStatus(status);
    EXPECT_TRUE(color.isValid());
  }
}

TEST_F(StatusIndicatorHelperTest, GetLabelTextForInstructionStatus)
{
  // validated that halted text is non-empty, the rest is empty
  for (size_t i = 0; i <= static_cast<size_t>(InstructionStatus::kUndefined); ++i)
  {
    const auto status = static_cast<InstructionStatus>(i);
    const QString text = GetLabelTextForInstructionStatus(status);
    if (status == InstructionStatus::kHalted)
    {
      EXPECT_FALSE(text.isEmpty());
    }
    else
    {
      EXPECT_TRUE(text.isEmpty());
    }
  }
}

}  // namespace oac_tree_gui::test
