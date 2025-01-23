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

#include <oac-tree-gui/model/procedure_item.h>
#include <oac-tree-gui/model/standard_variable_items.h>
#include <oac-tree-gui/model/workspace_item.h>

#include <sup/oac-tree/instruction.h>

#include <gtest/gtest.h>

namespace sequencergui::test
{

//! Tests for items from instructionitems.h

class SequencerItemsTest : public ::testing::Test
{
};

//! Validate Workspace

TEST_F(SequencerItemsTest, WorkspaceItem)
{
  WorkspaceItem item;
  EXPECT_TRUE(item.GetVariables().empty());

  auto var0 = item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  auto var1 = item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  EXPECT_EQ(item.GetVariables(), std::vector<VariableItem*>({var0, var1}));
}

//! Validate ProcedureItem

TEST_F(SequencerItemsTest, ProcedureItem)
{
  ProcedureItem item;

  item.SetDescription("abc");
  EXPECT_EQ(item.GetDescription(), "abc");

  item.SetName("efg");
  EXPECT_EQ(item.GetName(), "efg");
}

}  // namespace sequencergui::test
