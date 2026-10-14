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

#include "oac_tree_gui/viewmodel/instruction_operation_viewmodel.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/operation/breakpoint_helper.h>
#include <oac_tree_gui/transform/instruction_descriptor_helper.h>
#include <oac_tree_gui/transform/instruction_item_transform_helper.h>

#include <mvvm/model/application_model.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_info.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

namespace oac_tree_gui::test
{

//! Tests for InstructionOperationViewModel class.

class InstructionOperationViewModelTest : public ::testing::Test
{
public:
  class TestModel : public mvvm::ApplicationModel
  {
  public:
    TestModel() : mvvm::ApplicationModel("TestModel") {}
  };
};

TEST_F(InstructionOperationViewModelTest, SingleInstruction)
{
  TestModel model;

  auto sequence = InsertInstruction(domainconstants::kSequenceInstructionType, model.GetRootItem());
  sequence->SetStatus(InstructionStatus::kRunning);

  SetBreakpointStatus(*sequence, BreakpointStatus::kSet);

  InstructionOperationViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 4);

  auto sequence_displayname_index = viewmodel.index(0, 0, QModelIndex());
  auto sequence_type_index = viewmodel.index(0, 1, QModelIndex());
  auto sequence_status_index = viewmodel.index(0, 2, QModelIndex());
  auto sequence_breakpoint_index = viewmodel.index(0, 3, QModelIndex());

  auto views = viewmodel.FindViews(GetStatusItem(*sequence));
  EXPECT_EQ(views.size(), 1);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), sequence_status_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(sequence_displayname_index), sequence);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(sequence_status_index), GetStatusItem(*sequence));
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(sequence_breakpoint_index),
            GetBreakpointItem(*sequence));

  EXPECT_EQ(viewmodel.data(sequence_displayname_index, Qt::DisplayRole).toString(), "Sequence");
  EXPECT_EQ(viewmodel.data(sequence_type_index, Qt::DisplayRole).toString(), "Sequence");
  EXPECT_EQ(viewmodel.data(sequence_status_index, Qt::DisplayRole).toString(), "Running");
  // returns int corresponding to BreakpointStatus::kSet
  EXPECT_EQ(viewmodel.data(sequence_breakpoint_index, Qt::DisplayRole).toInt(), 1);
}

TEST_F(InstructionOperationViewModelTest, SequenceWithChild)
{
  TestModel model;

  auto sequence = InsertInstruction(domainconstants::kSequenceInstructionType, model.GetRootItem());
  auto wait0 = InsertInstruction(domainconstants::kWaitInstructionType, sequence);
  auto wait1 = InsertInstruction(domainconstants::kWaitInstructionType, sequence);

  InstructionOperationViewModel viewmodel(&model);
  auto sequence_ndex = viewmodel.index(0, 0, QModelIndex());
  EXPECT_EQ(viewmodel.rowCount(sequence_ndex), 2);
  EXPECT_EQ(viewmodel.columnCount(sequence_ndex), 4);

  auto wait0_displayname_index = viewmodel.index(0, 0, sequence_ndex);
  auto wait1_displayname_index = viewmodel.index(1, 0, sequence_ndex);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(wait0_displayname_index), wait0);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(wait1_displayname_index), wait1);

  EXPECT_EQ(viewmodel.data(wait0_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Wait"));
  EXPECT_EQ(viewmodel.data(wait1_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Wait"));
}

TEST_F(InstructionOperationViewModelTest, NotificationOnStatusChange)
{
  TestModel model;

  auto sequence = InsertInstruction(domainconstants::kSequenceInstructionType, model.GetRootItem());

  InstructionOperationViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 4);

  const QSignalSpy spy_data_changed(&viewmodel, &InstructionOperationViewModel::dataChanged);

  sequence->SetStatus(InstructionStatus::kRunning);
  EXPECT_EQ(spy_data_changed.count(), 1);
}

//! Validating how view model depicts a tree built from instruction info, as for jobs.
TEST_F(InstructionOperationViewModelTest, ItemTreeFromInfo)
{
  TestModel model;

  sup::oac_tree::InstructionInfo sequence_info(domainconstants::kSequenceInstructionType,
                                               sup::oac_tree::Instruction::kCompound, 0, {});
  (void)sequence_info.AppendChild(std::make_unique<sup::oac_tree::InstructionInfo>(
      domainconstants::kWaitInstructionType, sup::oac_tree::Instruction::kAction, 1,
      std::vector<sup::oac_tree::AttributeInfo>{{domainconstants::kNameAttribute, "First"}}));
  (void)sequence_info.AppendChild(std::make_unique<sup::oac_tree::InstructionInfo>(
      domainconstants::kWaitInstructionType, sup::oac_tree::Instruction::kAction, 2,
      std::vector<sup::oac_tree::AttributeInfo>{}));

  auto tree = CreateInstructionItemTree(CreateInstructionNodeTree(sequence_info));
  auto* wait0 = tree.indexes.at(1);
  auto* wait1 = tree.indexes.at(2);
  (void)model.InsertItem(std::move(tree.root), model.GetRootItem(), mvvm::TagIndex::Append());

  InstructionOperationViewModel viewmodel(&model);
  auto sequence_index = viewmodel.index(0, 0, QModelIndex());
  EXPECT_EQ(viewmodel.rowCount(sequence_index), 2);
  EXPECT_EQ(viewmodel.columnCount(sequence_index), 4);

  auto wait0_displayname_index = viewmodel.index(0, 0, sequence_index);
  auto wait1_displayname_index = viewmodel.index(1, 0, sequence_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(wait0_displayname_index), wait0);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(wait1_displayname_index), wait1);

  EXPECT_EQ(viewmodel.data(wait0_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("First"));
  // without a name, the domain type is shown
  EXPECT_EQ(viewmodel.data(wait1_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Wait"));
}

}  // namespace oac_tree_gui::test
