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

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/nodeeditor/connectable_view.h>
#include <oac_tree_gui/nodeeditor/connectable_view_factory.h>

#include <gtest/gtest.h>

#include "oac_tree_gui/nodeeditor/connectable_view_map.h"

namespace oac_tree_gui::test
{

//! Tests for InstructionViewItemMap class.

class ConnectableViewMapTest : public ::testing::Test
{
};

//! Testing Insert method.
TEST_F(ConnectableViewMapTest, InitialState)
{
  ConnectableViewMap map;
  EXPECT_EQ(map.GetSize(), 0);
}

//! Testing Insert method.
TEST_F(ConnectableViewMapTest, InsertThenFind)
{
  WaitItem instruction;
  auto view = ConnectableViewFactory().CreateView(&instruction);
  ConnectableViewMap map;

  // map is empty at the beginning
  EXPECT_FALSE(map.FindView(&instruction));

  // inserting to the map
  map.Insert(&instruction, view.get());

  EXPECT_EQ(map.GetSize(), 1);

  // now we can find the view
  EXPECT_EQ(map.FindView(&instruction), view.get());

  // it is not possible to insert another view for given instruction
  auto another_view = ConnectableViewFactory().CreateView(&instruction);
  EXPECT_THROW(map.Insert(&instruction, another_view.get()), RuntimeException);

  map.Clear();
  EXPECT_FALSE(map.FindView(&instruction));
  EXPECT_EQ(map.GetSize(), 0);
}

//! Testing Remove method.

TEST_F(ConnectableViewMapTest, RemoveUsingItem)
{
  WaitItem instruction;
  auto view = ConnectableViewFactory().CreateView(&instruction);
  ConnectableViewMap map;

  // inserting to the map
  map.Insert(&instruction, view.get());

  // removing
  map.Remove(&instruction);

  EXPECT_FALSE(map.FindView(&instruction));

  // it is not possible to remove what was already removed
  EXPECT_THROW(map.Remove(&instruction), RuntimeException);
}

//! Testing Remove method intended for views.

TEST_F(ConnectableViewMapTest, RemoveUsingView)
{
  WaitItem instruction;
  auto view = ConnectableViewFactory().CreateView(&instruction);
  ConnectableViewMap map;

  // inserting to the map
  map.Insert(&instruction, view.get());

  // removing
  map.Remove(view.get());

  EXPECT_FALSE(map.FindView(&instruction));

  // it is not possible to remove what was already removed
  EXPECT_THROW(map.Remove(view.get()), RuntimeException);
}

//! Testing InsertOrAssign method.
TEST_F(ConnectableViewMapTest, InsertOrAssignThenFind)
{
  WaitItem instruction;
  auto view = ConnectableViewFactory().CreateView(&instruction);
  ConnectableViewMap map;

  // map is empty at the beginning
  EXPECT_FALSE(map.FindView(&instruction));

  // updating the map
  map.InsertOrAssign(&instruction, view.get());

  // now we can find the view
  EXPECT_EQ(map.FindView(&instruction), view.get());

  map.Clear();
  EXPECT_FALSE(map.FindView(&instruction));
}

//! Testing TakeView method.
TEST_F(ConnectableViewMapTest, TakeView)
{
  WaitItem instruction0;
  auto view0 = ConnectableViewFactory().CreateView(&instruction0);
  WaitItem instruction1;
  auto view1 = ConnectableViewFactory().CreateView(&instruction1);
  ConnectableViewMap map;

  // updating the map
  map.InsertOrAssign(&instruction0, view0.get());
  map.InsertOrAssign(&instruction1, view1.get());

  auto taken = map.TakeView(&instruction0);
  EXPECT_EQ(taken, view0.get());

  // the view is not in the map anymore
  EXPECT_EQ(map.FindView(&instruction0), nullptr);

  // it is not possible to take a view again
  EXPECT_THROW(map.TakeView(&instruction0), RuntimeException);

  // another view is in the map
  EXPECT_EQ(map.FindView(&instruction1), view1.get());
}

//! Testing InsertOrAssign method when one view replaces another.
TEST_F(ConnectableViewMapTest, InsertOrAssignTwice)
{
  WaitItem instruction;
  auto view0 = ConnectableViewFactory().CreateView(&instruction);
  auto view1 = ConnectableViewFactory().CreateView(&instruction);
  ConnectableViewMap map;

  // updating the map
  map.InsertOrAssign(&instruction, view0.get());

  // now we can find the view
  EXPECT_EQ(map.FindView(&instruction), view0.get());

  // updating the map
  map.InsertOrAssign(&instruction, view1.get());

  // another view is in the map
  EXPECT_EQ(map.FindView(&instruction), view1.get());
}

//! Testing FindRelatedView method.
TEST_F(ConnectableViewMapTest, FindRelatedView)
{
  ConnectableViewMap map;
  WaitItem instruction0;
  auto view0 = ConnectableViewFactory().CreateView(&instruction0);

  // Empty list of related views for non-registered instructions
  EXPECT_TRUE(map.FindRelatedViews(&instruction0).empty());

  // List of related views should contain single view
  map.Insert(&instruction0, view0.get());
  EXPECT_EQ(map.FindRelatedViews(&instruction0), std::vector<ConnectableView*>({view0.get()}));
}

//! Testing FindRelatedView method for simple instruction hierarchy.
TEST_F(ConnectableViewMapTest, FindRelatedViewForChildrenTree)
{
  SequenceItem parent;
  ConnectableViewMap map;

  auto child0 = parent.InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  auto child1 = parent.InsertItem<SequenceItem>(mvvm::TagIndex::Append());

  auto grandchild0 = child0->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  auto grandchild1 = child0->InsertItem<WaitItem>(mvvm::TagIndex::Append());

  std::vector<InstructionItem*> instructions{&parent, child0, child1, grandchild0, grandchild1};

  std::vector<std::unique_ptr<ConnectableView>> views;
  for (const auto item : instructions)
  {
    auto view = ConnectableViewFactory().CreateView(item);
    map.Insert(item, view.get());
    views.emplace_back(std::move(view));
  }

  // expected related views of child0: will be views corresponding child0, grandchild0, grandchild1
  std::vector<ConnectableView*> expected({views.at(1).get(), views.at(3).get(), views.at(4).get()});
  EXPECT_EQ(map.FindRelatedViews(child0), expected);

  // expected related views of parent: views corresponding to parent, child0, grandchild0,
  // grandchild1, child1
  expected = {views.at(0).get(), views.at(1).get(), views.at(3).get(), views.at(4).get(),
              views.at(2).get()};
  EXPECT_EQ(map.FindRelatedViews(&parent), expected);
}

}  // namespace oac_tree_gui::test
