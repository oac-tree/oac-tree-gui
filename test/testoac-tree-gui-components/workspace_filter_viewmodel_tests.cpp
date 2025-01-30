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

#include <oac-tree-gui/domain/domain_helper.h>
#include <oac-tree-gui/model/standard_variable_items.h>
#include <oac-tree-gui/model/workspace_item.h>

#include <mvvm/model/application_model.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <gtest/gtest.h>

#include "oac-tree-gui/viewmodel/workspace_filter_viewmodel.h"

namespace oac_tree_gui::test
{

//! Tests for WorkspaceFilterViewModel class.

class WorkspaceFilterViewModelTest : public ::testing::Test
{
};

//! Testing method IsValidName.
TEST_F(WorkspaceFilterViewModelTest, IsValidName)
{
  WorkspaceFilterViewModel viewmodel;
  viewmodel.SetPattern("ABC");

  EXPECT_TRUE(viewmodel.IsValidName("ABC"));
  EXPECT_TRUE(viewmodel.IsValidName("AbC"));
  EXPECT_FALSE(viewmodel.IsValidName("AB"));
  EXPECT_FALSE(viewmodel.IsValidName(""));
}

//! Testing method IsValidItem for item name match.
TEST_F(WorkspaceFilterViewModelTest, IsItemAccepted)
{
  WorkspaceItem workspace;
  auto local_item0 = workspace.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  auto local_item1 = workspace.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());

  local_item0->SetDisplayName("ABC");
  local_item1->SetDisplayName("AB");

  WorkspaceFilterViewModel viewmodel;
  viewmodel.SetPattern("ABC");

  EXPECT_FALSE(viewmodel.IsItemAccepted(nullptr));
  EXPECT_TRUE(viewmodel.IsItemAccepted(local_item0));
  EXPECT_FALSE(viewmodel.IsItemAccepted(local_item1));
}

//! Testing method IsValidItem for channel name case.
TEST_F(WorkspaceFilterViewModelTest, IsItemAcceptedForChannelName)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  WorkspaceItem workspace;
  auto local_item = workspace.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  auto pv_access_item = workspace.InsertItem<PvAccessClientVariableItem>(mvvm::TagIndex::Append());

  local_item->SetDisplayName("A");
  pv_access_item->SetChannel("ABC");

  WorkspaceFilterViewModel viewmodel;
  viewmodel.SetPattern("ABC");

  EXPECT_FALSE(viewmodel.IsItemAccepted(nullptr));
  EXPECT_FALSE(viewmodel.IsItemAccepted(local_item));
  EXPECT_TRUE(viewmodel.IsItemAccepted(pv_access_item));  // channel name matches

  pv_access_item->SetChannel("AB");
  pv_access_item->SetDisplayName("ABC");
  EXPECT_TRUE(viewmodel.IsItemAccepted(pv_access_item));  // name matches
}

//! Full test of the proxy model looking to the viewmodel with the workspace.
TEST_F(WorkspaceFilterViewModelTest, FilterViewmodel)
{
  // workspace with two variables
  mvvm::ApplicationModel model;
  auto workspace_item = model.InsertItem<WorkspaceItem>();
  auto local_item0 = model.InsertItem<LocalVariableItem>(workspace_item, mvvm::TagIndex::Append());
  auto local_item1 = model.InsertItem<LocalVariableItem>(workspace_item, mvvm::TagIndex::Append());
  local_item0->SetDisplayName("ABC");
  local_item1->SetDisplayName("AB");

  // viewmodel
  mvvm::AllItemsViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);

  auto index_local_item0 = viewmodel.index(0, 0, QModelIndex());
  EXPECT_EQ(index_local_item0.data().toString(), QString("ABC"));
  auto index_local_item1 = viewmodel.index(1, 0, QModelIndex());
  EXPECT_EQ(index_local_item1.data().toString(), QString("AB"));

  // filtering proxy
  WorkspaceFilterViewModel viewmodel_proxy;
  viewmodel_proxy.SetPattern("ABC");
  viewmodel_proxy.setSourceModel(&viewmodel);

  // second item was filtered out
  EXPECT_EQ(viewmodel_proxy.columnCount(), 2);
  EXPECT_EQ(viewmodel_proxy.rowCount(), 1);
  EXPECT_TRUE(viewmodel_proxy.filterAcceptsRow(0, QModelIndex()));
  EXPECT_FALSE(viewmodel_proxy.filterAcceptsRow(1, QModelIndex()));
  EXPECT_EQ(viewmodel_proxy.index(0, 0, QModelIndex()).data().toString(), QString("ABC"));

  // properties of first item should remain
  EXPECT_TRUE(viewmodel_proxy.filterAcceptsRow(0, index_local_item0));  // dynamicType property
  EXPECT_FALSE(viewmodel_proxy.filterAcceptsRow(0, index_local_item1));
}

}  // namespace oac_tree_gui::test
