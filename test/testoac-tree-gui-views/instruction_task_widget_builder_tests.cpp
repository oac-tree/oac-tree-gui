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

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_factory.h>
#include <oac_tree_gui/model/universal_item_helper.h>
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
  auto item = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  item->SetDisplayName("abc");

  InstructionTaskWidgetBuilder builder;
  EXPECT_EQ(builder.GetInstructionCount(), 0U);

  auto task_widget = builder.CreateTaskWidget(*item);
  EXPECT_EQ(task_widget->GetInstructionStatus(), item->GetStatus());
  EXPECT_EQ(task_widget->GetLabelText(), "abc");
  EXPECT_EQ(builder.GetInstructionCount(), 1U);

  EXPECT_EQ(builder.FindWidgetForInstruction(item.get()), task_widget.get());
  EXPECT_EQ(builder.FindWidgetForInstruction(nullptr), nullptr);
}

TEST_F(InstructionTaskWidgetBuilderTest, SequenceWithTwoChildren)
{
  auto sequence = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  sequence->SetDisplayName("seq");
  sequence->SetStatus(InstructionStatus::kRunning);
  auto wait0 = InsertInstruction(domainconstants::kWaitInstructionType, sequence.get());
  wait0->SetDisplayName("wait0");
  auto wait1 = InsertInstruction(domainconstants::kWaitInstructionType, sequence.get());
  wait1->SetDisplayName("wait1");
  wait1->SetStatus(InstructionStatus::kRunning);

  InstructionTaskWidgetBuilder builder;
  auto root_task_widget = builder.CreateTaskWidget(*sequence);
  EXPECT_EQ(builder.FindWidgetForInstruction(sequence.get()), root_task_widget.get());

  EXPECT_EQ(root_task_widget->GetInstructionStatus(), sequence->GetStatus());
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
  auto sequence = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  sequence->SetDisplayName("sequence");
  auto child0 = InsertInstruction(domainconstants::kWaitInstructionType, sequence.get());
  child0->SetDisplayName("child0");
  auto child1 = InsertInstruction(domainconstants::kSequenceInstructionType, sequence.get());
  child1->SetDisplayName("child1");
  auto grandchild0 = InsertInstruction(domainconstants::kWaitInstructionType, child1);
  grandchild0->SetDisplayName("grandchild0");

  InstructionTaskWidgetBuilder builder;
  auto root_task_widget = builder.CreateTaskWidget(*sequence);
  EXPECT_EQ(builder.FindWidgetForInstruction(sequence.get()), root_task_widget.get());

  EXPECT_EQ(root_task_widget->GetInstructionStatus(), sequence->GetStatus());
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
