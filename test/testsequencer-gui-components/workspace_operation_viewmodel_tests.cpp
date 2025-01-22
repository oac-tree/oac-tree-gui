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

#include "sequencergui/viewmodel/workspace_operation_viewmodel.h"

#include <sequencergui/components/component_helper.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/anyvalue_item_transform_helper.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

namespace sequencergui::test
{

//! Tests for WorkspaceOperationViewModel class.
class WorkspaceOperationViewModelTest : public ::testing::Test
{
};

//! Testing model layout when there is LocalVariable without AnyValueItem on board.
TEST_F(WorkspaceOperationViewModelTest, LocalVariableWithoutAnyValue)
{
  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();

  auto variable_item = model.InsertItem<LocalVariableItem>(workspace_item);
  variable_item->SetName("abc");

  WorkspaceOperationViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 4);

  // checking viewmodel layout

  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 4);

  auto variable_name_index = viewmodel.index(0, 0);
  auto variable_value_index = viewmodel.index(0, 1);
  auto variable_type_index = viewmodel.index(0, 2);
  auto variable_channel_index = viewmodel.index(0, 3);

  // reading data from cell
  EXPECT_EQ(viewmodel.data(variable_name_index, Qt::DisplayRole).toString().toStdString(),
            std::string("abc"));
  // placeholder instead on AnyValue
  EXPECT_EQ(viewmodel.data(variable_value_index, Qt::DisplayRole).toString(), QString());
  EXPECT_EQ(viewmodel.data(variable_type_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Local"));
}

//! Testing model layout when there is LocalVariable with scalar AnyValueItem on board.
TEST_F(WorkspaceOperationViewModelTest, LocalVariableWithScalarAnyValue)
{
  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();

  auto variable_item = model.InsertItem<LocalVariableItem>(workspace_item);
  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(anyvalue, *variable_item);
  variable_item->SetName("abc");

  WorkspaceOperationViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 4);

  // checking viewmodel layout

  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 4);

  auto variable_name_index = viewmodel.index(0, 0);
  auto variable_value_index = viewmodel.index(0, 1);
  auto variable_type_index = viewmodel.index(0, 2);
  auto variable_channel_index = viewmodel.index(0, 3);

  // reading data from cell
  EXPECT_EQ(viewmodel.data(variable_name_index, Qt::DisplayRole).toString().toStdString(),
            std::string("abc"));
  // the scalar value
  EXPECT_EQ(viewmodel.data(variable_value_index, Qt::DisplayRole).toInt(), 42);
  EXPECT_EQ(viewmodel.data(variable_type_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Local"));
}

//! Testing model layout when there is LocalVariable initially without any AnyValueItem on board.
//! Then we check if row is regenerated correctly.
TEST_F(WorkspaceOperationViewModelTest, LocalVariableWithScalarInserted)
{
  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();

  auto variable_item = model.InsertItem<LocalVariableItem>(workspace_item);
  variable_item->SetName("abc");

  WorkspaceOperationViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 4);

  // setting AnyValue
  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(anyvalue, *variable_item);

  // checking viewmodel layout

  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 4);

  auto variable_name_index = viewmodel.index(0, 0);
  auto variable_value_index = viewmodel.index(0, 1);
  auto variable_type_index = viewmodel.index(0, 2);
  auto variable_channel_index = viewmodel.index(0, 3);

  // reading data from cell
  EXPECT_EQ(viewmodel.data(variable_name_index, Qt::DisplayRole).toString().toStdString(),
            std::string("abc"));

  EXPECT_EQ(viewmodel.data(variable_value_index, Qt::DisplayRole).toInt(), 42);

  EXPECT_EQ(viewmodel.data(variable_type_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Local"));
}

//! Validating how IsAvailable status change is notified. 4th column in a model is a tricky one, it
//! contains the channel name and green/gray color box, denoting IsAvailable status. This test
//! checks, that the model emits dataChanged event on corresponding changes.
TEST_F(WorkspaceOperationViewModelTest, ChannelAccessIsAvailableStatus)
{
  const std::string channel_name("CHANNEL");
  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();

  auto variable_item = model.InsertItem<ChannelAccessVariableItem>(workspace_item);
  variable_item->SetName("abc");
  variable_item->SetChannel(channel_name);
  variable_item->SetIsAvailable(true);

  WorkspaceOperationViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 4);

  // setting AnyValue
  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(anyvalue, *variable_item);

  // checking viewmodel layout
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 4);

  // reading data from cell
  auto variable_channel_index = viewmodel.index(0, 3);
  EXPECT_FALSE(viewmodel.data(variable_channel_index, Qt::EditRole).isValid());
  EXPECT_TRUE(viewmodel.data(variable_channel_index, Qt::DisplayRole).isValid());
  EXPECT_EQ(viewmodel.data(variable_channel_index, Qt::DisplayRole).toString(),
            QString::fromStdString(channel_name));
  EXPECT_TRUE(viewmodel.data(variable_channel_index, Qt::DecorationRole).isValid());
  EXPECT_EQ(viewmodel.data(variable_channel_index, Qt::DecorationRole).value<QColor>(),
            GetConnectedVariableColor());

  QSignalSpy spy_data_changed(&viewmodel, &WorkspaceOperationViewModel::dataChanged);
  variable_item->SetIsAvailable(false);

  ASSERT_EQ(spy_data_changed.count(), 1);
  QList<QVariant> arguments = spy_data_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex left, QModelIndex right, QVector<int> roles
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), variable_channel_index);
  EXPECT_EQ(arguments.at(1).value<QModelIndex>(), variable_channel_index);
  QVector<int> expectedRoles = {Qt::DecorationRole};
  EXPECT_EQ(arguments.at(2).value<QVector<int>>(), expectedRoles);

  EXPECT_EQ(viewmodel.data(variable_channel_index, Qt::DecorationRole).value<QColor>(),
            GetDisonnectedVariableColor());
}

}  // namespace sequencergui::test
