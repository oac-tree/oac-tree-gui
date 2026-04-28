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

#include "oac_tree_gui/views/operation/instruction_task_widget_builder.h"

#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/views/operation/task_widget.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests of InstructionTaskWidgetBuilder class.
 */
class InstructionTaskWidgetBuilderTest : public ::testing::Test
{
};

TEST_F(InstructionTaskWidgetBuilderTest, SingleSequenceInstruction)
{
  SequenceItem item;
  item.SetDisplayName("abc");

  InstructionTaskWidgetBuilder builder;
  EXPECT_EQ(builder.GetInstructionCount(), 0U);

  auto task_widget = builder.CreateTaskWidget(item);
  EXPECT_EQ(task_widget->GetInstructionStatus(), item.GetStatus());
  EXPECT_EQ(task_widget->GetLabelText(), "abc");
  EXPECT_EQ(builder.GetInstructionCount(), 1U);

  EXPECT_EQ(builder.FindWidgetForInstruction(&item), task_widget.get());
  EXPECT_EQ(builder.FindWidgetForInstruction(nullptr), nullptr);
}

TEST_F(InstructionTaskWidgetBuilderTest, SequenceWithTwoChildren)
{
  SequenceItem sequence;
  sequence.SetDisplayName("seq");
  sequence.SetStatus(InstructionStatus::kRunning);
  auto wait0 = sequence.InsertItem<WaitItem>(mvvm::TagIndex::Append());
  wait0->SetDisplayName("wait0");
  auto wait1 = sequence.InsertItem<WaitItem>(mvvm::TagIndex::Append());
  wait1->SetDisplayName("wait1");
  wait1->SetStatus(InstructionStatus::kRunning);

  InstructionTaskWidgetBuilder builder;
  auto root_task_widget = builder.CreateTaskWidget(sequence);
  EXPECT_EQ(builder.FindWidgetForInstruction(&sequence), root_task_widget.get());

  EXPECT_EQ(root_task_widget->GetInstructionStatus(), sequence.GetStatus());
  EXPECT_EQ(root_task_widget->GetLabelText(), "seq");

  auto child_widgets = root_task_widget->GetChildWidgets();
  ASSERT_EQ(child_widgets.size(), 2);
  EXPECT_EQ(child_widgets.at(0)->GetInstructionStatus(), wait0->GetStatus());
  EXPECT_EQ(child_widgets.at(0)->GetLabelText(), "wait0");
  EXPECT_EQ(child_widgets.at(1)->GetInstructionStatus(), wait1->GetStatus());
  EXPECT_EQ(child_widgets.at(1)->GetLabelText(), "wait1");

  EXPECT_EQ(builder.FindWidgetForInstruction(wait0), child_widgets.at(0));
  EXPECT_EQ(builder.FindWidgetForInstruction(wait1), child_widgets.at(1));
}

TEST_F(InstructionTaskWidgetBuilderTest, SequenceWithChildrenAndGrandchildren)
{
  SequenceItem sequence;
  sequence.SetDisplayName("sequence");
  auto child0 = sequence.InsertItem<WaitItem>(mvvm::TagIndex::Append());
  child0->SetDisplayName("child0");
  auto child1 = sequence.InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  child1->SetDisplayName("child1");
  auto grandchild0 = child1->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  grandchild0->SetDisplayName("grandchild0");

  InstructionTaskWidgetBuilder builder;
  auto root_task_widget = builder.CreateTaskWidget(sequence);
  EXPECT_EQ(builder.FindWidgetForInstruction(&sequence), root_task_widget.get());

  EXPECT_EQ(root_task_widget->GetInstructionStatus(), sequence.GetStatus());
  EXPECT_EQ(root_task_widget->GetLabelText(), "sequence");

  auto child_widgets = root_task_widget->GetChildWidgets();
  ASSERT_EQ(child_widgets.size(), 2);
  EXPECT_EQ(child_widgets.at(0)->GetInstructionStatus(), child0->GetStatus());
  EXPECT_EQ(child_widgets.at(0)->GetLabelText(), "child0");
  EXPECT_EQ(child_widgets.at(1)->GetInstructionStatus(), child1->GetStatus());
  EXPECT_EQ(child_widgets.at(1)->GetLabelText(), "child1");

  auto grandchild_widgets = child_widgets.at(1)->GetChildWidgets();
  ASSERT_EQ(grandchild_widgets.size(), 1);
  EXPECT_EQ(grandchild_widgets.at(0)->GetInstructionStatus(), grandchild0->GetStatus());
  EXPECT_EQ(grandchild_widgets.at(0)->GetLabelText(), "grandchild0");

  EXPECT_EQ(builder.FindWidgetForInstruction(child0), child_widgets.at(0));
  EXPECT_EQ(builder.FindWidgetForInstruction(child1), child_widgets.at(1));
  EXPECT_EQ(builder.FindWidgetForInstruction(grandchild0), grandchild_widgets.at(0));
}

}  // namespace oac_tree_gui::test
