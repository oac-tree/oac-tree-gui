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

#include "oac_tree_gui/views/operation/task_widget.h"

#include <gtest/gtest.h>

#include <QGroupBox>
#include <QLabel>

namespace oac_tree_gui::test
{

class TaskWidgetTest : public ::testing::Test
{
};

TEST_F(TaskWidgetTest, InitialState)
{
  TaskWidget widget("abc");
  widget.show();

  EXPECT_EQ(widget.GetLabelText(), QString("abc"));

  // children group should be invisible
  auto group_box = widget.findChild<QGroupBox*>();
  ASSERT_NE(group_box, nullptr);

  // there is no reliable way to check visibility
  EXPECT_FALSE(group_box->isVisible());
}

TEST_F(TaskWidgetTest, GetSetInstructionStatus)
{
  TaskWidget widget("abc");

  EXPECT_EQ(widget.GetInstructionStatus(), InstructionStatus::kUndefined);
  widget.SetInstructionStatus(InstructionStatus::kRunning);
  EXPECT_EQ(widget.GetInstructionStatus(), InstructionStatus::kRunning);
}

TEST_F(TaskWidgetTest, CreateAndAddChild)
{
  TaskWidget widget("abc");
  widget.show();

  auto child_widget = widget.CreateAndAddChild("child");
  EXPECT_EQ(child_widget->GetLabelText(), QString("child"));
  // children group should be visible after adding child
  auto group_box = widget.findChild<QGroupBox*>();
  ASSERT_NE(group_box, nullptr);
  EXPECT_TRUE(group_box->isVisible());
}

}  // namespace oac_tree_gui::test