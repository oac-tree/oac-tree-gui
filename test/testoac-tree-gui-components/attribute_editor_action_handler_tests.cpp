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
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>

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
  AttributeEditorContext CreateContext(const mvvm::SessionItem* selected_item)
  {
    AttributeEditorContext result;
    result.selected_items_callback = [selected_item]() -> std::vector<const mvvm::SessionItem*>
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

  EXPECT_FALSE(handler->CanToggleExposedFlag());
  EXPECT_FALSE(handler->CanSetDefaultType());
  EXPECT_FALSE(handler->CanSetPlaceholderType());
  EXPECT_FALSE(handler->CanEditAnyValue());
}

TEST_F(AttributeEditorActionHandlerTest, TimeoutAttributeSelectedCallingEditorIsNotPossible)
{
  const WaitItem item;
  auto timeout_attribute = GetPropertyItem(item, domainconstants::kTimeoutAttribute);

  auto handler = CreateActionHandler(timeout_attribute);

  EXPECT_TRUE(handler->CanToggleExposedFlag());
  EXPECT_TRUE(handler->CanSetDefaultType());
  EXPECT_TRUE(handler->CanSetPlaceholderType());

  // it is not possible to call external editor
  EXPECT_FALSE(handler->CanEditAnyValue());
}

TEST_F(AttributeEditorActionHandlerTest, CallingEditorWhenAnyValueSelected)
{
  LocalVariableItem item;
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, item);

  auto handler = CreateActionHandler(item.GetAnyValueItem());

  EXPECT_TRUE(handler->CanToggleExposedFlag());

  // it is not possible to replace types with placeholders
  EXPECT_FALSE(handler->CanSetDefaultType());
  EXPECT_FALSE(handler->CanSetPlaceholderType());

  // it is possible to call external editor
  EXPECT_TRUE(handler->CanEditAnyValue());
}

TEST_F(AttributeEditorActionHandlerTest, ToggleExposedFlagForTimeoutAttribute)
{
  const WaitItem item;
  auto timeout_attribute = dynamic_cast<sup::gui::AnyValueItem*>(
      GetPropertyItem(item, domainconstants::kTimeoutAttribute));

  auto handler = CreateActionHandler(timeout_attribute);

  EXPECT_TRUE(handler->GetExposedFlag());
  EXPECT_TRUE(GetAttributeExposedFlag(*timeout_attribute));

  handler->OnToggleExposedFlag();
  EXPECT_FALSE(handler->GetExposedFlag());
  EXPECT_FALSE(GetAttributeExposedFlag(*timeout_attribute));
}

TEST_F(AttributeEditorActionHandlerTest, TuringScalarAttributeIntoPlaceholder)
{
  sup::gui::AnyValueScalarItem item;
  item.SetAnyTypeName(sup::dto::kInt8TypeName);
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);

  auto handler = CreateActionHandler(&item);

  handler->OnSetPlaceholderType();
  EXPECT_EQ(item.Data<std::string>(), itemconstants::kDefaultPlaceholderAttributeValue);

  handler->OnSetAsDefaultType();
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
}

}  // namespace oac_tree_gui::test
