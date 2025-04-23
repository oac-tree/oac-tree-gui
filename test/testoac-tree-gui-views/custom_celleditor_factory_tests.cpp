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

#include "oac_tree_gui/views/composer/custom_celleditor_factory.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/universal_item_helper.h>

#include <mvvm/editors/string_completer_combo_editor.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/viewmodel/property_viewmodel.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QDebug>
#include <QLineEdit>

namespace oac_tree_gui
{

class CustomCellEditorFactoryTest : public ::testing::Test
{
public:
  CustomCellEditorFactoryTest() : m_view_model(&m_model) {}

  mvvm::ApplicationModel m_model;
  mvvm::PropertyViewModel m_view_model;
};

TEST_F(CustomCellEditorFactoryTest, IsVariableNameRelatedDisplayName)
{
  EXPECT_FALSE(IsVariableNameRelatedDisplayName(itemconstants::kName));
  EXPECT_TRUE(IsVariableNameRelatedDisplayName(domainconstants::kEqualVariableAttributeName));
  EXPECT_TRUE(IsVariableNameRelatedDisplayName(domainconstants::kFromVariableAttributeName));
  EXPECT_TRUE(IsVariableNameRelatedDisplayName(domainconstants::kGenericVariableNameAttribute));
  EXPECT_TRUE(IsVariableNameRelatedDisplayName(domainconstants::kInputVariableNameAttribute));
  EXPECT_TRUE(IsVariableNameRelatedDisplayName(domainconstants::kLeftVariableNameAttribute));
  EXPECT_TRUE(IsVariableNameRelatedDisplayName(domainconstants::kOutputVariableNameAttribute));
  EXPECT_TRUE(IsVariableNameRelatedDisplayName(domainconstants::kRequestAttribute));
  EXPECT_TRUE(IsVariableNameRelatedDisplayName(domainconstants::kRightVariableNameAttribute));
}

TEST_F(CustomCellEditorFactoryTest, IsVariableNameRelatedProperty)
{
  // properties of copy instruction: name, isRoot, inputVar, outputVar, Behavior
  auto copy_item = InsertInstruction(domainconstants::kCopyInstructionType, m_model.GetRootItem());

  EXPECT_FALSE(IsVariableNameRelatedProperty(*copy_item));
  EXPECT_FALSE(IsVariableNameRelatedProperty(*copy_item->GetItem(itemconstants::kName)));
  EXPECT_TRUE(IsVariableNameRelatedProperty(
      *copy_item->GetItem(domainconstants::kInputVariableNameAttribute)));
  EXPECT_TRUE(IsVariableNameRelatedProperty(
      *copy_item->GetItem(domainconstants::kOutputVariableNameAttribute)));
}

TEST_F(CustomCellEditorFactoryTest, IsVariableNameRelatedCell)
{
  // properties of copy instruction: name, isRoot, inputVar, outputVar, Behavior
  auto copy_item = InsertInstruction(domainconstants::kCopyInstructionType, m_model.GetRootItem());
  m_view_model.SetRootSessionItem(copy_item);

  EXPECT_FALSE(IsVariableNameRelatedCell(QModelIndex()));

  // properties of copy instruction: name, isRoot, inputVar, outputVar, Behavior
  EXPECT_EQ(m_view_model.rowCount(), 5);

  {
    auto input_var_property = copy_item->GetItem(domainconstants::kInputVariableNameAttribute);
    ASSERT_NE(input_var_property, nullptr);
    auto indexes = m_view_model.GetIndexOfSessionItem(input_var_property);
    ASSERT_EQ(indexes.size(), 2);
    ASSERT_TRUE(IsVariableNameRelatedCell(indexes.at(0)));
    ASSERT_TRUE(IsVariableNameRelatedCell(indexes.at(1)));
  }

  {
    auto name_property = copy_item->GetItem(itemconstants::kName);
    ASSERT_NE(name_property, nullptr);
    auto indexes = m_view_model.GetIndexOfSessionItem(name_property);
    ASSERT_EQ(indexes.size(), 2);
    ASSERT_FALSE(IsVariableNameRelatedCell(indexes.at(0)));
    ASSERT_FALSE(IsVariableNameRelatedCell(indexes.at(1)));
  }
}

TEST_F(CustomCellEditorFactoryTest, CreateEditor)
{
  auto copy_item = InsertInstruction(domainconstants::kCopyInstructionType, m_model.GetRootItem());
  m_view_model.SetRootSessionItem(copy_item);

  {  // property related to variable
    auto input_var_property = copy_item->GetItem(domainconstants::kInputVariableNameAttribute);
    ASSERT_NE(input_var_property, nullptr);

    auto indexes = m_view_model.GetIndexOfSessionItem(input_var_property);
    ASSERT_EQ(indexes.size(), 2);

    const QStringList completer_list({"ABC", "DEF"});
    CustomCellEditorFactory factory([completer_list]() { return completer_list; });
    auto editor = factory.CreateEditor(indexes.at(1));
    auto string_completer_editor = dynamic_cast<mvvm::StringCompleterComboEditor*>(editor.get());

    ASSERT_NE(string_completer_editor, nullptr);
    EXPECT_EQ(string_completer_editor->GetStringList(), completer_list);
  }

  {  // property related to simple name
    auto name_property = copy_item->GetItem(itemconstants::kName);
    ASSERT_NE(name_property, nullptr);

    auto indexes = m_view_model.GetIndexOfSessionItem(name_property);
    ASSERT_EQ(indexes.size(), 2);

    // custom factory do not create default editors
    CustomCellEditorFactory factory({});
    auto editor = factory.CreateEditor(indexes.at(1));
    EXPECT_EQ(editor.get(), nullptr);
  }
}

}  // namespace oac_tree_gui
