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

#include "oac_tree_gui/composer/attribute_editor_action_handler.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/attribute_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/instruction_item_transform_helper.h>
#include <oac_tree_gui/transform/variable_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/session_item.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Tests for AttributeEditorActionHandler class.

class AttributeEditorActionHandlerTest : public ::testing::Test
{
public:
  /**
   * @brief Test helper to create a context reporting currently selected item.
   *
   * @param selected_item Item pretending it is currently selected in the attribute editor.
   */
  AttributeEditorContext CreateContext(mvvm::SessionItem* selected_item)
  {
    AttributeEditorContext result;
    result.selected_items_callback = [selected_item]() -> std::vector<mvvm::SessionItem*>
    { return {selected_item}; };
    return result;
  }

  /**
   * @brief Test helper to create an action handler.
   *
   * @param selected_item Item pretending it is currently selected in the attribute editor.
   */
  std::unique_ptr<AttributeEditorActionHandler> CreateActionHandler(
      mvvm::SessionItem* selected_item)
  {
    return std::make_unique<AttributeEditorActionHandler>(CreateContext(selected_item));
  }
};

TEST_F(AttributeEditorActionHandlerTest, WhenNothingIsSelectedHandlerCanNotPerformAnyAction)
{
  // no items are selected in the property editor
  auto handler = CreateActionHandler(nullptr);

  EXPECT_FALSE(handler->CanSetPlaceholderType());
  EXPECT_FALSE(handler->CanEditAnyValue());
}

TEST_F(AttributeEditorActionHandlerTest, TimeoutAttributeSelected)
{
  auto item = CreateInstructionItem(domainconstants::kWaitInstructionType);
  auto timeout_attribute = item->GetAttributes()->FindItem(domainconstants::kTimeoutAttribute);

  auto handler = CreateActionHandler(timeout_attribute);

  EXPECT_TRUE(handler->CanSetPlaceholderType());

  // it is not possible to call external editor
  EXPECT_FALSE(handler->CanEditAnyValue());
}

TEST_F(AttributeEditorActionHandlerTest, StringAttributeSelected)
{
  auto item = CreateInstructionItem(domainconstants::kIncludeInstructionType);
  auto path_attribute = item->GetAttributes()->FindItem(domainconstants::kPathAttribute);

  auto handler = CreateActionHandler(path_attribute);

  // a string attribute takes `$par` as its value
  EXPECT_FALSE(handler->CanSetPlaceholderType());
}

TEST_F(AttributeEditorActionHandlerTest, ShowCollapsedAttributeSelected)
{
  auto item = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  auto show_collapsed_attribute =
      item->GetAttributes()->FindItem(domainconstants::kShowCollapsedAttribute);

  auto handler = CreateActionHandler(show_collapsed_attribute);

  // GUI attribute takes no text
  EXPECT_FALSE(handler->CanSetPlaceholderType());
  handler->OnSetPlaceholderType();
  EXPECT_FALSE(show_collapsed_attribute->HasData());
}

//! Variable attribute takes no text.
TEST_F(AttributeEditorActionHandlerTest, VariableAttributeSelected)
{
  auto item = CreateVariableItem(domainconstants::kLocalVariableType);
  auto dynamic_type_attribute =
      item->GetAttributes()->FindItem(domainconstants::kDynamicTypeAttribute);

  auto handler = CreateActionHandler(dynamic_type_attribute);

  EXPECT_FALSE(handler->CanSetPlaceholderType());
  EXPECT_FALSE(handler->CanEditAnyValue());

  handler->OnSetPlaceholderType();
  EXPECT_FALSE(dynamic_type_attribute->HasData());
}

TEST_F(AttributeEditorActionHandlerTest, CallingEditorWhenAnyValueSelected)
{
  auto item = CreateVariableItem(domainconstants::kLocalVariableType);
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *item);

  auto handler = CreateActionHandler(item->GetAnyValueItem());

  // it is not possible to replace AnyValue with a placeholder
  EXPECT_FALSE(handler->CanSetPlaceholderType());

  // it is possible to call external editor
  EXPECT_TRUE(handler->CanEditAnyValue());
}

TEST_F(AttributeEditorActionHandlerTest, InstructionAnyValueSelected)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  auto item = CreateInstructionItem(domainconstants::kPvAccessWriteInstructionType);
  auto anyvalue_item = GetAnyValueItem(*item);
  ASSERT_NE(anyvalue_item, nullptr);

  auto handler = CreateActionHandler(anyvalue_item);


  EXPECT_TRUE(handler->CanEditAnyValue());
}

TEST_F(AttributeEditorActionHandlerTest, TurningInstructionAttributeIntoPlaceholder)
{
  auto item = CreateInstructionItem(domainconstants::kWaitInstructionType);
  auto timeout_attribute = dynamic_cast<AttributeItem*>(
      item->GetAttributes()->FindItem(domainconstants::kTimeoutAttribute));
  ASSERT_NE(timeout_attribute, nullptr);

  auto handler = CreateActionHandler(timeout_attribute);

  handler->OnSetPlaceholderType();
  EXPECT_EQ(timeout_attribute->GetState(), AttributeState::kText);
  EXPECT_EQ(timeout_attribute->GetText(), itemconstants::kDefaultPlaceholderAttributeValue);

  // clearing the text returns to not set
  EXPECT_TRUE(timeout_attribute->SetText(""));
  EXPECT_EQ(timeout_attribute->GetState(), AttributeState::kNotSet);
}

//! A field of a structured AnyValue is not an attribute: no actions.
TEST_F(AttributeEditorActionHandlerTest, AnyValueFieldSelected)
{
  auto item = CreateVariableItem(domainconstants::kLocalVariableType);
  const sup::dto::AnyValue anyvalue = {{{"signed", {sup::dto::SignedInteger32Type, 42}}}};
  SetAnyValue(anyvalue, *item);
  auto field = item->GetAnyValueItem()->GetChildren().at(0);

  auto handler = CreateActionHandler(field);

  EXPECT_FALSE(handler->CanSetPlaceholderType());
  EXPECT_FALSE(handler->CanEditAnyValue());

  handler->OnSetPlaceholderType();
  EXPECT_EQ(field->Data<mvvm::int32>(), 42);
}

}  // namespace oac_tree_gui::test
