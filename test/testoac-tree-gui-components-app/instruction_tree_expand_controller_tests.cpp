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

#include "oac_tree_gui/operation/instruction_tree_expand_controller.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/viewmodel/instruction_operation_viewmodel.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

#include <QTreeView>

namespace oac_tree_gui::test
{

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

TEST_F(InstructionTreeExpandControllerTest, SetTreeViewToInstructionExpandState)
{
  auto container = m_model.InsertItem<InstructionContainerItem>();
  auto sequence0 = m_model.InsertItem<SequenceItem>(container);
  auto sequence1 = m_model.InsertItem<SequenceItem>(sequence0);
  (void) sequence1->SetProperty(domainconstants::kShowCollapsedAttribute, true);
  auto wait = m_model.InsertItem<SequenceItem>(sequence1);

  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 3);

  QTreeView tree;
  tree.setModel(&m_viewmodel);
  auto selection_model = tree.selectionModel();

  InstructionTreeExpandController controller(&tree);

  // container should be set first
  EXPECT_THROW(controller.SetTreeViewToInstructionExpandState(), RuntimeException);

  // initial expand state of tree (everything is collapsed)
  EXPECT_FALSE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(sequence0).at(0)));
  EXPECT_FALSE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(sequence1).at(0)));
  EXPECT_FALSE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(wait).at(0)));

  controller.SetInstructionContainer(container);

  // checking tree expand state corresponding to instruction state
  EXPECT_NO_THROW(controller.SetTreeViewToInstructionExpandState());
  EXPECT_TRUE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(sequence0).at(0)));
  EXPECT_FALSE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(sequence1).at(0)));
  EXPECT_TRUE(tree.isExpanded(m_viewmodel.GetIndexOfSessionItem(wait).at(0)));
}

TEST_F(InstructionTreeExpandControllerTest, ExpandStateFromTreeViewToInstructions)
{
  auto container = m_model.InsertItem<InstructionContainerItem>();
  auto sequence0 = m_model.InsertItem<SequenceItem>(container);
  auto sequence1 = m_model.InsertItem<SequenceItem>(sequence0);
  (void) sequence1->SetProperty(domainconstants::kShowCollapsedAttribute, true);
  auto wait = m_model.InsertItem<SequenceItem>(sequence1);

  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 3);

  auto container_index = m_viewmodel.index(0, 0);
  auto sequence0_index = m_viewmodel.index(0, 0, container_index);

  QTreeView tree;
  tree.setModel(&m_viewmodel);
  auto selection_model = tree.selectionModel();

  InstructionTreeExpandController controller(&tree);
  controller.SetInstructionContainer(container);
  controller.SetTreeViewToInstructionExpandState();
  EXPECT_FALSE(IsCollapsed(*sequence0));
  EXPECT_TRUE(IsCollapsed(*sequence1));

  tree.expandAll();

  EXPECT_FALSE(IsCollapsed(*sequence0));
  EXPECT_FALSE(IsCollapsed(*sequence1));

  tree.collapseAll();
  EXPECT_TRUE(IsCollapsed(*sequence0));
  EXPECT_TRUE(IsCollapsed(*sequence1));

  tree.expand(sequence0_index);
  EXPECT_FALSE(IsCollapsed(*sequence0));
  EXPECT_TRUE(IsCollapsed(*sequence1));
}

}  // namespace oac_tree_gui::test
