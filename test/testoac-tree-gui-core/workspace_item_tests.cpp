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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

class WorkspaceItemTest : public ::testing::Test
{
};

TEST_F(WorkspaceItemTest, WorkspaceItem)
{
  WorkspaceItem item;
  EXPECT_TRUE(item.GetVariables().empty());
  EXPECT_EQ(item.GetVariableCount(), 0);

  auto var0 = item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  auto var1 = item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  EXPECT_EQ(item.GetVariables(), std::vector<VariableItem*>({var0, var1}));
  EXPECT_EQ(item.GetVariableCount(), 2);
}

TEST_F(WorkspaceItemTest, GetVariableNames)
{
  WorkspaceItem item;
  EXPECT_TRUE(item.GetVariableNames().empty());

  auto var0 = item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var0->SetName("var0");
  auto var1 = item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var1->SetName("var1");

  EXPECT_EQ(item.GetVariableNames(), std::vector<std::string>({"var0", "var1"}));
}

}  // namespace oac_tree_gui::test
