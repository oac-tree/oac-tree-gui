/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/operation/instruction_tree_expand_controller.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/viewmodel/instruction_operation_viewmodel.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

#include <QTreeView>

using namespace sequencergui;

class InstructionTreeExpandControllerTest : public ::testing::Test
{
public:
  InstructionTreeExpandControllerTest() : m_viewmodel(&m_model) {}

  class TestModel : public mvvm::ApplicationModel
  {
  public:
    TestModel() : mvvm::ApplicationModel("TestModel") {}
  };

  TestModel m_model;
  InstructionOperationViewModel m_viewmodel;
};

TEST_F(InstructionTreeExpandControllerTest, SelectWaitInFullyExpandedTree)
{
  auto sequence0 = m_model.InsertItem<SequenceItem>();
  auto sequence1 = m_model.InsertItem<SequenceItem>(sequence0);
  auto wait = m_model.InsertItem<SequenceItem>(sequence1);

  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 3);

  QTreeView tree;
  tree.setModel(&m_viewmodel);
  auto selection_model = tree.selectionModel();
  InstructionTreeExpandController controller(&tree);

  // tree is fully expanded, no item selected
  tree.expandAll();
  EXPECT_TRUE(selection_model->selectedIndexes().empty());

  // selected Wait instruction, it should be selected in the reality
  controller.SaveSelectionRequest({wait});
  EXPECT_EQ(controller.GetInstructionsToSelect(), std::vector<mvvm::SessionItem*>({wait}));

  EXPECT_EQ(controller.FindVisibleInstruction(wait), wait);

  EXPECT_TRUE(selection_model->selectedIndexes().empty());
}

TEST_F(InstructionTreeExpandControllerTest, SelectWaitInCollapsedBranch)
{
  auto sequence0 = m_model.InsertItem<SequenceItem>();
  auto sequence1 = m_model.InsertItem<SequenceItem>(sequence0);
  auto wait = m_model.InsertItem<SequenceItem>(sequence1);

  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 3);

  QTreeView tree;
  tree.setModel(&m_viewmodel);
  auto selection_model = tree.selectionModel();
  InstructionTreeExpandController controller(&tree);

  // collapsing sequence1 branch
  tree.expandAll();
  tree.setExpanded(m_viewmodel.GetIndexOfSessionItem(sequence1).at(0), false);

  EXPECT_TRUE(selection_model->selectedIndexes().empty());

  // selected Wait instruction, sequence1 should be selected in the reality
  controller.SaveSelectionRequest({wait});
  EXPECT_EQ(controller.FindVisibleInstruction(wait), sequence1);

  EXPECT_EQ(controller.GetInstructionsToSelect(), std::vector<mvvm::SessionItem*>({sequence1}));
}

//! Validating SetDefaultExpandState method. We create a procedure with nestes sequence and mark it
//! as collapsed. Tree view should have corresponding branches reflecting this state.

TEST_F(InstructionTreeExpandControllerTest, SetDefaultExpandState)
{
  auto container = m_model.InsertItem<InstructionContainerItem>();
  auto sequence0 = m_model.InsertItem<SequenceItem>(container);
  auto sequence1 = m_model.InsertItem<SequenceItem>(sequence0);
  sequence1->SetProperty(domainconstants::kShowCollapsedAttribute, true);
  auto wait = m_model.InsertItem<SequenceItem>(sequence1);

  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 3);

  QTreeView tree;
  tree.setModel(&m_viewmodel);
  auto selection_model = tree.selectionModel();

  InstructionTreeExpandController controller(&tree);

  // expanding all
  tree.expandAll();

  // container should be set first
  EXPECT_THROW(controller.SetDefaultExpandState(), RuntimeException);

  EXPECT_TRUE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(sequence0).at(0)));
  EXPECT_TRUE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(sequence1).at(0)));
  EXPECT_TRUE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(wait).at(0)));

  controller.SetInstructionContainer(container);

  // checking expand state when initially everything was expanding
  EXPECT_NO_THROW(controller.SetDefaultExpandState());
  EXPECT_TRUE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(sequence0).at(0)));
  EXPECT_FALSE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(sequence1).at(0)));
  EXPECT_TRUE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(wait).at(0)));

  // checking expand state when initially everything was collapsed
  tree.collapseAll();
  controller.SetDefaultExpandState();
  EXPECT_TRUE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(sequence0).at(0)));
  EXPECT_FALSE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(sequence1).at(0)));
  EXPECT_TRUE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(wait).at(0)));
}
