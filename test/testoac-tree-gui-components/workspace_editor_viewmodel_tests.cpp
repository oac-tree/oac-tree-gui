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
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

#include "oac_tree_gui/viewmodel/workspace_editor_viewmodel.h"

//! Tests for WorkspaceEditorViewModel class.

namespace oac_tree_gui::test
{

class WorkspaceEditorViewModelTest : public ::testing::Test
{
};

//! Single local variable in a workspace.
//! ViewModel should see two rows and 3 columns for VariableItem, and its properties beneath (name
//! and dynamicType).

TEST_F(WorkspaceEditorViewModelTest, LocalVariable)
{
  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();

  WorkspaceEditorViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);
  EXPECT_EQ(viewmodel.rowCount(), 0);
  EXPECT_EQ(viewmodel.columnCount(), 3);

  // adding single variable and populating with values
  auto variable_item = model.InsertItem<LocalVariableItem>(workspace_item);
  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(anyvalue, *variable_item);
  variable_item->SetName("abc");

  // checking viewmodel layout

  // one parent item with type, and editable name
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 3);

  auto variable_displayname_index = viewmodel.index(0, 0);
  auto variable_emptylabel_index = viewmodel.index(0, 1);
  auto variable_typename_index = viewmodel.index(0, 2);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(variable_displayname_index), variable_item);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(variable_emptylabel_index), variable_item);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(variable_typename_index), variable_item);

  // reading data from cell
  EXPECT_EQ(viewmodel.data(variable_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("abc"));
  EXPECT_EQ(viewmodel.data(variable_emptylabel_index, Qt::DisplayRole).toString().toStdString(),
            std::string(""));
  EXPECT_EQ(viewmodel.data(variable_typename_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Local"));

  // writing data to cells
  EXPECT_TRUE(viewmodel.setData(variable_displayname_index, "new_name", Qt::EditRole));
  EXPECT_FALSE(viewmodel.setData(variable_emptylabel_index, "cant_change", Qt::EditRole));
  EXPECT_FALSE(viewmodel.setData(variable_typename_index, "cant_change", Qt::EditRole));
  EXPECT_EQ(variable_item->GetName(), std::string("new_name"));

  // Access to properties beneath. There are only AnyValueItem and dynamicType, name property was
  // filtered out.
  EXPECT_EQ(viewmodel.rowCount(variable_displayname_index), 2);
  EXPECT_EQ(viewmodel.columnCount(variable_displayname_index), 3);

  // dynamicProperty
  auto dynamictype_name_index = viewmodel.index(0, 0, variable_displayname_index);
  auto dynamictype_value_index = viewmodel.index(0, 1, variable_displayname_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(dynamictype_name_index),
            variable_item->GetItem(domainconstants::kDynamicTypeAttribute));

  // AnyValueItem
  auto anyvalue_name_index = viewmodel.index(1, 0, variable_displayname_index);
  auto anyvalue_value_index = viewmodel.index(1, 1, variable_displayname_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(anyvalue_name_index),
            variable_item->GetAnyValueItem());

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(anyvalue_value_index),
            variable_item->GetAnyValueItem());
}

//! Single ChannelAccessVariable in a workspace.
//! ViewModel should see single row and 3 columns for VariableItem, and its properties beneath.

TEST_F(WorkspaceEditorViewModelTest, ChannelAccessVariable)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();

  WorkspaceEditorViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);
  EXPECT_EQ(viewmodel.rowCount(), 0);
  EXPECT_EQ(viewmodel.columnCount(), 3);

  // adding single variable and populating with values
  auto variable_item = model.InsertItem<ChannelAccessVariableItem>(workspace_item);
  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(anyvalue, *variable_item);
  variable_item->SetName("abc");

  // checking viewmodel layout

  // one parent item with type, and editable name
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 3);

  auto variable_displayname_index = viewmodel.index(0, 0);
  auto variable_emptylabel_index = viewmodel.index(0, 1);
  auto variable_typename_index = viewmodel.index(0, 2);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(variable_displayname_index), variable_item);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(variable_emptylabel_index), variable_item);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(variable_typename_index), variable_item);

  // reading data from cell
  EXPECT_EQ(viewmodel.data(variable_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("abc"));
  EXPECT_EQ(viewmodel.data(variable_emptylabel_index, Qt::DisplayRole).toString().toStdString(),
            std::string(""));
  EXPECT_EQ(viewmodel.data(variable_typename_index, Qt::DisplayRole).toString().toStdString(),
            std::string("ChannelAccessClient"));

  // writing data to cells
  EXPECT_TRUE(viewmodel.setData(variable_displayname_index, "new_name", Qt::EditRole));
  EXPECT_FALSE(viewmodel.setData(variable_emptylabel_index, "cant_change", Qt::EditRole));
  EXPECT_EQ(variable_item->GetName(), std::string("new_name"));

  EXPECT_EQ(viewmodel.rowCount(variable_displayname_index), 2);
  EXPECT_EQ(viewmodel.columnCount(variable_displayname_index), 3);

  auto channel_name_index = viewmodel.index(0, 0, variable_displayname_index);
  auto channel_value_index = viewmodel.index(0, 1, variable_displayname_index);
  auto anyvalue_name_index = viewmodel.index(1, 0, variable_displayname_index);
  auto anyvalue_value_index = viewmodel.index(1, 1, variable_displayname_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(anyvalue_name_index),
            variable_item->GetAnyValueItem());
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(anyvalue_value_index),
            variable_item->GetAnyValueItem());

  EXPECT_EQ(viewmodel.data(channel_name_index, Qt::DisplayRole).toString().toStdString(),
            std::string("channel"));
  EXPECT_EQ(viewmodel.data(anyvalue_name_index, Qt::DisplayRole).toString().toStdString(),
            std::string("scalar"));
}

}  // namespace oac_tree_gui::test
