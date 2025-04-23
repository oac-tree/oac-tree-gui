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

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <mvvm/model/application_model.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <gtest/gtest.h>

#include <QTreeView>

#include "oac_tree_gui/operation/workspace_view_component_provider.h"

namespace oac_tree_gui::test
{

//! Tests for WOrkspaceViewComponentProvider class.

class WorkspaceViewComponentProviderTest : public ::testing::Test
{
};

//! Viewmodel is looking at the workspace with two variables. Provider sets up the view and filters
//! one item out.
TEST_F(WorkspaceViewComponentProviderTest, FilterViewmodel)
{
  // workspace with two variables
  mvvm::ApplicationModel model;
  auto workspace_item = model.InsertItem<WorkspaceItem>();
  auto local_item0 = model.InsertItem<LocalVariableItem>(workspace_item, mvvm::TagIndex::Append());
  auto local_item1 = model.InsertItem<LocalVariableItem>(workspace_item, mvvm::TagIndex::Append());
  local_item0->SetDisplayName("ABC");
  local_item1->SetDisplayName("AB");

  // viewmodel
  auto viewmodel = std::make_unique<mvvm::AllItemsViewModel>(&model);
  auto viewmodel_ptr = viewmodel.get();
  viewmodel->SetRootSessionItem(workspace_item);

  QTreeView view;
  WorkspaceViewComponentProvider provider(std::move(viewmodel), &view);

  provider.SetFilterPattern("ABC");
  EXPECT_EQ(view.model()->rowCount(), 1);

  provider.SetFilterPattern("A");
  EXPECT_EQ(view.model()->rowCount(), 2);
}

}  // namespace oac_tree_gui::test
