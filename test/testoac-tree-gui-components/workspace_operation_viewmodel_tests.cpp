/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "oac_tree_gui/viewmodel/workspace_operation_viewmodel.h"

#include <oac_tree_gui/components/component_helper.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/application_model.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

#include <QSignalSpy>

namespace oac_tree_gui::test
{

//! Tests for WorkspaceOperationViewModel class.
class WorkspaceOperationViewModelTest : public ::testing::Test
{
public:
  //! Returns a struct with two int32 fields.
  static sup::dto::AnyValue CreateStruct(std::int32_t a, std::int32_t b)
  {
    return {{{"a", {sup::dto::SignedInteger32Type, a}}, {"b", {sup::dto::SignedInteger32Type, b}}}};
  }

  //! Returns the display value of the cell.
  static QVariant GetData(const WorkspaceOperationViewModel& viewmodel, int row, int column,
                          const QModelIndex& parent = QModelIndex())
  {
    return viewmodel.data(viewmodel.index(row, column, parent), Qt::DisplayRole);
  }
};

//! Testing model layout when there is LocalVariable without AnyValueItem on board.
TEST_F(WorkspaceOperationViewModelTest, LocalVariableWithoutAnyValue)
{
  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();

  auto variable_item = InsertVariable(domainconstants::kLocalVariableType, workspace_item);
  variable_item->SetName("abc");

  WorkspaceOperationViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 4);

  // checking viewmodel layout

  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 4);

  auto variable_name_index = viewmodel.index(0, 0, QModelIndex());
  auto variable_value_index = viewmodel.index(0, 1, QModelIndex());
  auto variable_type_index = viewmodel.index(0, 2, QModelIndex());
  auto variable_channel_index = viewmodel.index(0, 3, QModelIndex());

  // reading data from cell
  EXPECT_EQ(viewmodel.data(variable_name_index, Qt::DisplayRole).toString().toStdString(),
            std::string("abc"));
  // placeholder instead on AnyValue
  EXPECT_EQ(viewmodel.data(variable_value_index, Qt::DisplayRole).toString(), QString("empty"));
  EXPECT_EQ(viewmodel.data(variable_type_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Local"));
}

//! Testing model layout when there is LocalVariable with scalar AnyValueItem on board.
TEST_F(WorkspaceOperationViewModelTest, LocalVariableWithScalarAnyValue)
{
  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();

  auto variable_item = InsertVariable(domainconstants::kLocalVariableType, workspace_item);
  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(anyvalue, *variable_item);
  variable_item->SetName("abc");

  WorkspaceOperationViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 4);

  // checking viewmodel layout

  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 4);

  auto variable_name_index = viewmodel.index(0, 0, QModelIndex());
  auto variable_value_index = viewmodel.index(0, 1, QModelIndex());
  auto variable_type_index = viewmodel.index(0, 2, QModelIndex());
  auto variable_channel_index = viewmodel.index(0, 3, QModelIndex());

  // reading data from cell
  EXPECT_EQ(viewmodel.data(variable_name_index, Qt::DisplayRole).toString().toStdString(),
            std::string("abc"));
  // the scalar value
  EXPECT_EQ(viewmodel.data(variable_value_index, Qt::DisplayRole).toInt(), 42);
  EXPECT_EQ(viewmodel.data(variable_type_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Local"));
}

TEST_F(WorkspaceOperationViewModelTest, LocalVariableWithScalarAnyValueConstructedAsInfoItem)
{
  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();

  auto variable_item = CreateJobVariableItem(domainconstants::kLocalVariableType);
  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(anyvalue, *variable_item);
  variable_item->SetName("abc");

  model.InsertItem(std::move(variable_item), workspace_item, mvvm::TagIndex::Append());

  WorkspaceOperationViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 4);

  // checking viewmodel layout

  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 4);

  auto variable_name_index = viewmodel.index(0, 0, QModelIndex());
  auto variable_value_index = viewmodel.index(0, 1, QModelIndex());
  auto variable_type_index = viewmodel.index(0, 2, QModelIndex());
  auto variable_channel_index = viewmodel.index(0, 3, QModelIndex());

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

  auto variable_item = InsertVariable(domainconstants::kLocalVariableType, workspace_item);
  variable_item->SetName("abc");

  WorkspaceOperationViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 4);

  // setting AnyValue
  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(anyvalue, *variable_item);

  // checking viewmodel layout

  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 4);

  auto variable_name_index = viewmodel.index(0, 0, QModelIndex());
  auto variable_value_index = viewmodel.index(0, 1, QModelIndex());
  auto variable_type_index = viewmodel.index(0, 2, QModelIndex());
  auto variable_channel_index = viewmodel.index(0, 3, QModelIndex());

  // reading data from cell
  EXPECT_EQ(viewmodel.data(variable_name_index, Qt::DisplayRole).toString().toStdString(),
            std::string("abc"));

  EXPECT_EQ(viewmodel.data(variable_value_index, Qt::DisplayRole).toInt(), 42);

  EXPECT_EQ(viewmodel.data(variable_type_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Local"));
}

//! Struct replaced by a scalar, and back. The variable row and its branch follow.
TEST_F(WorkspaceOperationViewModelTest, StructReplacedByScalar)
{
  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();
  auto variable_item = InsertVariable(domainconstants::kLocalVariableType, workspace_item);
  SetAnyValue(CreateStruct(1, 2), *variable_item);

  WorkspaceOperationViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);

  // variable with struct beneath
  auto variable_index = viewmodel.index(0, 0, QModelIndex());
  ASSERT_EQ(viewmodel.rowCount(variable_index), 1);
  auto struct_index = viewmodel.index(0, 0, variable_index);
  EXPECT_EQ(viewmodel.rowCount(struct_index), 2);
  EXPECT_EQ(GetData(viewmodel, 0, 1), QVariant(QString("---")));
  EXPECT_EQ(GetData(viewmodel, 1, 1, struct_index).toInt(), 2);

  // scalar in the variable row
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, *variable_item);
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  variable_index = viewmodel.index(0, 0, QModelIndex());
  EXPECT_EQ(viewmodel.rowCount(variable_index), 0);
  EXPECT_EQ(GetData(viewmodel, 0, 1).toInt(), 42);

  // struct again
  SetAnyValue(CreateStruct(3, 4), *variable_item);
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  variable_index = viewmodel.index(0, 0, QModelIndex());
  ASSERT_EQ(viewmodel.rowCount(variable_index), 1);
  struct_index = viewmodel.index(0, 0, variable_index);
  EXPECT_EQ(viewmodel.rowCount(struct_index), 2);
  EXPECT_EQ(GetData(viewmodel, 0, 1), QVariant(QString("---")));
  EXPECT_EQ(GetData(viewmodel, 0, 1, struct_index).toInt(), 3);
}

//! AnyValueItem removed with no insert after. The variable row stays, without a branch.
TEST_F(WorkspaceOperationViewModelTest, AnyValueRemoved)
{
  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();
  auto variable_item = InsertVariable(domainconstants::kLocalVariableType, workspace_item);
  variable_item->SetName("abc");
  SetAnyValue(CreateStruct(1, 2), *variable_item);

  WorkspaceOperationViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);
  ASSERT_EQ(viewmodel.rowCount(viewmodel.index(0, 0, QModelIndex())), 1);

  model.RemoveItem(variable_item->GetAnyValueItem());

  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.rowCount(viewmodel.index(0, 0, QModelIndex())), 0);
  EXPECT_EQ(GetData(viewmodel, 0, 0).toString(), QString("abc"));
  EXPECT_TRUE(GetData(viewmodel, 0, 1).toString().isEmpty());
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(viewmodel.index(0, 0, QModelIndex())), variable_item);
}

//! AnyValueItem replaced as in WorkspaceEditorActionHandler::EditAnyValue, then undo and redo.
TEST_F(WorkspaceOperationViewModelTest, AnyValueReplacedWithUndo)
{
  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();
  auto variable_item = InsertVariable(domainconstants::kLocalVariableType, workspace_item);
  SetAnyValue(CreateStruct(1, 2), *variable_item);

  WorkspaceOperationViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);

  model.SetUndoEnabled(true);

  // replacing struct with a scalar
  model.RemoveItem(variable_item->GetAnyValueItem());
  auto scalar_item = std::make_unique<sup::gui::AnyValueScalarItem>();
  scalar_item->SetAnyTypeName(sup::dto::kInt32TypeName);
  (void)scalar_item->SetData(std::int32_t{42});
  (void)model.InsertItem(std::move(scalar_item), variable_item, {});

  EXPECT_EQ(viewmodel.rowCount(viewmodel.index(0, 0, QModelIndex())), 0);
  EXPECT_EQ(GetData(viewmodel, 0, 1).toInt(), 42);

  // undo the insert: no AnyValueItem
  model.GetCommandStack()->Undo();
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.rowCount(viewmodel.index(0, 0, QModelIndex())), 0);
  EXPECT_TRUE(GetData(viewmodel, 0, 1).toString().isEmpty());

  // undo the remove: struct is back
  model.GetCommandStack()->Undo();
  auto variable_index = viewmodel.index(0, 0, QModelIndex());
  ASSERT_EQ(viewmodel.rowCount(variable_index), 1);
  EXPECT_EQ(viewmodel.rowCount(viewmodel.index(0, 0, variable_index)), 2);
  EXPECT_EQ(GetData(viewmodel, 0, 1), QVariant(QString("---")));

  // redo both
  model.GetCommandStack()->Redo();
  model.GetCommandStack()->Redo();
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.rowCount(viewmodel.index(0, 0, QModelIndex())), 0);
  EXPECT_EQ(GetData(viewmodel, 0, 1).toInt(), 42);
}

//! Fields inserted into and removed from a struct in place. The branch follows.
TEST_F(WorkspaceOperationViewModelTest, StructFieldInsertedAndRemoved)
{
  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();
  auto variable_item = InsertVariable(domainconstants::kLocalVariableType, workspace_item);
  SetAnyValue(CreateStruct(1, 2), *variable_item);
  auto struct_item = dynamic_cast<sup::gui::AnyValueStructItem*>(variable_item->GetAnyValueItem());
  ASSERT_NE(struct_item, nullptr);

  WorkspaceOperationViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);

  auto field_item = struct_item->AddScalarField("c", std::int32_t{3});

  auto variable_index = viewmodel.index(0, 0, QModelIndex());
  ASSERT_EQ(viewmodel.rowCount(variable_index), 1);
  auto struct_index = viewmodel.index(0, 0, variable_index);
  ASSERT_EQ(viewmodel.rowCount(struct_index), 3);
  EXPECT_EQ(GetData(viewmodel, 2, 0, struct_index).toString(), QString("c"));
  EXPECT_EQ(GetData(viewmodel, 2, 1, struct_index).toInt(), 3);

  model.RemoveItem(field_item);

  variable_index = viewmodel.index(0, 0, QModelIndex());
  ASSERT_EQ(viewmodel.rowCount(variable_index), 1);
  struct_index = viewmodel.index(0, 0, variable_index);
  EXPECT_EQ(viewmodel.rowCount(struct_index), 2);
}

//! Validating how IsAvailable status change is notified. 4th column in a model is a tricky one, it
//! contains the channel name and green/gray color box, denoting IsAvailable status. This test
//! checks, that the model emits dataChanged event on corresponding changes.
TEST_F(WorkspaceOperationViewModelTest, ChannelAccessIsAvailableStatus)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string channel_name("CHANNEL");
  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();

  auto variable_item = InsertVariable(domainconstants::kChannelAccessVariableType, workspace_item);
  variable_item->SetName("abc");
  variable_item->SetAttribute(domainconstants::kChannelAttribute, channel_name);
  variable_item->SetIsAvailable(true);

  WorkspaceOperationViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 4);

  // setting AnyValue
  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(anyvalue, *variable_item);

  // checking viewmodel layout
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 4);

  // reading data from cell
  auto variable_channel_index = viewmodel.index(0, 3, QModelIndex());
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
  const QList<QVariant> arguments = spy_data_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex left, QModelIndex right, QVector<int> roles
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), variable_channel_index);
  EXPECT_EQ(arguments.at(1).value<QModelIndex>(), variable_channel_index);
  const QVector<int> expected_roles = {Qt::DecorationRole};
  EXPECT_EQ(arguments.at(2).value<QVector<int>>(), expected_roles);

  EXPECT_EQ(viewmodel.data(variable_channel_index, Qt::DecorationRole).value<QColor>(),
            GetDisonnectedVariableColor());
}

TEST_F(WorkspaceOperationViewModelTest, ChannelAccessIsAvailableStatusInfoItemCase)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  // Creating domain Variable, then VariableInfo, then VariableItem
  const std::string expected_name("abc");
  const std::string expected_channel("CHANNEL");
  const std::string expected_datatype(R"RAW({"type":"int32"})RAW");
  const std::string expected_value("42");

  const attribute_list_t attributes = {{domainconstants::kNameAttribute, expected_name},
                                       {domainconstants::kChannelAttribute, expected_channel},
                                       {domainconstants::kTypeAttribute, expected_datatype},
                                       {domainconstants::kValueAttribute, expected_value}};

  auto variable_item =
      CreateJobVariableItem(domainconstants::kPvAccessServerVariableType, 0, attributes);
  auto variable_item_ptr = variable_item.get();

  mvvm::ApplicationModel model;

  auto workspace_item = model.InsertItem<WorkspaceItem>();

  (void)model.InsertItem(std::move(variable_item), workspace_item, mvvm::TagIndex::Append());

  GetIsAvailableItem(*variable_item_ptr)->SetData(true);

  WorkspaceOperationViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(workspace_item);
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 4);

  // checking viewmodel layout
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 4);

  // reading data from cell
  auto variable_channel_index = viewmodel.index(0, 3, QModelIndex());
  EXPECT_FALSE(viewmodel.data(variable_channel_index, Qt::EditRole).isValid());
  EXPECT_TRUE(viewmodel.data(variable_channel_index, Qt::DisplayRole).isValid());
  EXPECT_EQ(viewmodel.data(variable_channel_index, Qt::DisplayRole).toString(),
            QString::fromStdString(expected_channel));
  EXPECT_TRUE(viewmodel.data(variable_channel_index, Qt::DecorationRole).isValid());
  EXPECT_EQ(viewmodel.data(variable_channel_index, Qt::DecorationRole).value<QColor>(),
            GetConnectedVariableColor());

  QSignalSpy spy_data_changed(&viewmodel, &WorkspaceOperationViewModel::dataChanged);
  variable_item_ptr->SetIsAvailable(false);

  ASSERT_EQ(spy_data_changed.count(), 1);
  const QList<QVariant> arguments = spy_data_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex left, QModelIndex right, QVector<int> roles
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), variable_channel_index);
  EXPECT_EQ(arguments.at(1).value<QModelIndex>(), variable_channel_index);
  const QVector<int> expected_roles = {Qt::DecorationRole};
  EXPECT_EQ(arguments.at(2).value<QVector<int>>(), expected_roles);

  EXPECT_EQ(viewmodel.data(variable_channel_index, Qt::DecorationRole).value<QColor>(),
            GetDisonnectedVariableColor());
}

}  // namespace oac_tree_gui::test
