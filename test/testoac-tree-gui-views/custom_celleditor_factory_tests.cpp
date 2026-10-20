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

#include "oac_tree_gui/views/composer/custom_celleditor_factory.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/viewmodel/attribute_editor_viewmodel.h>

#include <mvvm/editors/nullable_bool_editor.h>
#include <mvvm/editors/scientific_spinbox.h>
#include <mvvm/editors/string_completer_combo_editor.h>
#include <mvvm/model/application_model.h>
#include <mvvm/utils/i_limited_float.h>

#include <gtest/gtest.h>

namespace oac_tree_gui
{

class CustomCellEditorFactoryTest : public ::testing::Test
{
public:
  CustomCellEditorFactoryTest() : m_view_model(&m_model) {}

  mvvm::ApplicationModel m_model;
  AttributeEditorViewModel m_view_model;
};

TEST_F(CustomCellEditorFactoryTest, IsVariableNameRelatedDisplayName)
{
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
  EXPECT_TRUE(IsVariableNameRelatedProperty(
      *copy_item->GetAttributes()->FindItem(domainconstants::kInputVariableNameAttribute)));
  EXPECT_TRUE(IsVariableNameRelatedProperty(
      *copy_item->GetAttributes()->FindItem(domainconstants::kOutputVariableNameAttribute)));
}

TEST_F(CustomCellEditorFactoryTest, IsVariableNameRelatedCell)
{
  // properties of copy instruction: name, isRoot, inputVar, outputVar, Behavior
  auto copy_item = InsertInstruction(domainconstants::kCopyInstructionType, m_model.GetRootItem());
  m_view_model.SetRootSessionItem(copy_item);

  EXPECT_FALSE(IsVariableNameRelatedCell(QModelIndex()));

  // attribute editor rows of copy instruction: inputVar, outputVar, Behavior
  EXPECT_EQ(m_view_model.rowCount(QModelIndex()), 3);

  {
    auto input_var_property =
        copy_item->GetAttributes()->FindItem(domainconstants::kInputVariableNameAttribute);
    ASSERT_NE(input_var_property, nullptr);
    auto indexes = m_view_model.GetIndexOfSessionItem(input_var_property);
    ASSERT_EQ(indexes.size(), 3);
    ASSERT_TRUE(IsVariableNameRelatedCell(indexes.at(0)));
    ASSERT_TRUE(IsVariableNameRelatedCell(indexes.at(1)));
  }

  {
    auto behavior_property = copy_item->FindItem(itemconstants::kBehaviorTag);
    ASSERT_NE(behavior_property, nullptr);
    auto indexes = m_view_model.GetIndexOfSessionItem(behavior_property);
    ASSERT_EQ(indexes.size(), 3);
    ASSERT_FALSE(IsVariableNameRelatedCell(indexes.at(0)));
    ASSERT_FALSE(IsVariableNameRelatedCell(indexes.at(1)));
  }
}

TEST_F(CustomCellEditorFactoryTest, CreateEditor)
{
  auto copy_item = InsertInstruction(domainconstants::kCopyInstructionType, m_model.GetRootItem());
  m_view_model.SetRootSessionItem(copy_item);

  {  // property related to variable
    auto input_var_property =
        copy_item->GetAttributes()->FindItem(domainconstants::kInputVariableNameAttribute);
    ASSERT_NE(input_var_property, nullptr);

    auto indexes = m_view_model.GetIndexOfSessionItem(input_var_property);
    ASSERT_EQ(indexes.size(), 3);

    const QStringList completer_list({"ABC", "DEF"});
    CustomCellEditorFactory factory([completer_list]() { return completer_list; });
    auto editor = factory.CreateEditor(indexes.at(1));
    auto string_completer_editor = dynamic_cast<mvvm::StringCompleterComboEditor*>(editor.get());

    ASSERT_NE(string_completer_editor, nullptr);
    EXPECT_EQ(string_completer_editor->GetStringList(), completer_list);
  }
}

//! Instruction attributes without data get the nullable editors of sup-mvvm.
TEST_F(CustomCellEditorFactoryTest, CreateEditorForNullableAttributes)
{
  auto wait_item = InsertInstruction(domainconstants::kWaitInstructionType, m_model.GetRootItem());
  m_view_model.SetRootSessionItem(wait_item);
  const CustomCellEditorFactory factory([]() { return QStringList(); });

  auto create_editor = [this, &factory, wait_item](const std::string& name)
  {
    auto property = wait_item->GetAttributes()->FindItem(name);
    EXPECT_FALSE(property->HasData());
    auto indexes = m_view_model.GetIndexOfSessionItem(property);
    return factory.CreateEditor(indexes.at(1));
  };

  {  // float64
    auto editor = create_editor(domainconstants::kTimeoutAttribute);
    auto float_editor = dynamic_cast<mvvm::ScientificSpinBox*>(editor.get());
    ASSERT_NE(float_editor, nullptr);
    EXPECT_TRUE(float_editor->GetLimitedFloat()->IsNullable());
    EXPECT_EQ(float_editor->GetPlaceholderText(), QString("default"));
  }

  {  // bool
    auto editor = create_editor(domainconstants::kBlockingAttribute);
    EXPECT_NE(dynamic_cast<mvvm::NullableBoolEditor*>(editor.get()), nullptr);
  }
}

}  // namespace oac_tree_gui
