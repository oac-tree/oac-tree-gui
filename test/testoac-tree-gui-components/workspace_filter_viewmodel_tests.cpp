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

#include "oac_tree_gui/viewmodel/workspace_filter_viewmodel.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/variable_info_item.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <mvvm/model/application_model.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for WorkspaceFilterViewModel class.
 */
class WorkspaceFilterViewModelTest : public ::testing::Test
{
public:
};

TEST_F(WorkspaceFilterViewModelTest, IsValidName)
{
  WorkspaceFilterViewModel viewmodel;
  viewmodel.SetPattern("ABC");

  EXPECT_TRUE(viewmodel.IsValidName("ABC"));
  EXPECT_TRUE(viewmodel.IsValidName("AbC"));
  EXPECT_FALSE(viewmodel.IsValidName("AB"));
  EXPECT_FALSE(viewmodel.IsValidName(""));
}

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

TEST_F(WorkspaceFilterViewModelTest, IsItemAcceptedForVariableInfoCase)
{
  WorkspaceItem workspace;

  auto local_item0 = CreateVariableInfoItem(domainconstants::kLocalVariableType);
  local_item0->SetDisplayName("ABC");
  auto local_item1 = CreateVariableInfoItem(domainconstants::kLocalVariableType);
  local_item1->SetDisplayName("AB");

  auto local_item0_ptr = workspace.InsertItem(std::move(local_item0), mvvm::TagIndex::Append());
  auto local_item1_ptr = workspace.InsertItem(std::move(local_item1), mvvm::TagIndex::Append());

  WorkspaceFilterViewModel viewmodel;
  viewmodel.SetPattern("ABC");

  EXPECT_FALSE(viewmodel.IsItemAccepted(nullptr));
  EXPECT_TRUE(viewmodel.IsItemAccepted(local_item0_ptr));
  EXPECT_FALSE(viewmodel.IsItemAccepted(local_item1_ptr));
}

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

TEST_F(WorkspaceFilterViewModelTest, IsItemAcceptedForChannelNameInfoCase)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  WorkspaceItem workspace;
  auto local_item = CreateVariableInfoItem(domainconstants::kLocalVariableType);
  auto pv_access_item = CreateVariableInfoItem(domainconstants::kPvAccessServerVariableType);

  // we have to create "channel" property manually, since domain Variable doesn't have channel
  // attribute by default
  pv_access_item->AddProperty(domainconstants::kChannelAttribute, std::string());

  local_item->SetDisplayName("A");
  // info item doesn't have API to set a channel name
  GetChannelItem(*pv_access_item)->SetData("ABC");

  auto local_item_ptr = workspace.InsertItem(std::move(local_item), mvvm::TagIndex::Append());
  auto pv_access_item_ptr =
      workspace.InsertItem(std::move(pv_access_item), mvvm::TagIndex::Append());

  WorkspaceFilterViewModel viewmodel;
  viewmodel.SetPattern("ABC");

  EXPECT_FALSE(viewmodel.IsItemAccepted(nullptr));
  EXPECT_FALSE(viewmodel.IsItemAccepted(local_item_ptr));
  EXPECT_TRUE(viewmodel.IsItemAccepted(pv_access_item_ptr));  // channel name matches

  GetChannelItem(*pv_access_item_ptr)->SetData("AB");
  pv_access_item_ptr->SetDisplayName("ABC");

  EXPECT_TRUE(viewmodel.IsItemAccepted(pv_access_item_ptr));  // name matches
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
