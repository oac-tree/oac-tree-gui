/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/composer/attribute_editor_action_handler.h"

#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_item.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

using namespace sequencergui;

Q_DECLARE_METATYPE(mvvm::SessionItem*)

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
    result.selected_item_callback = [selected_item]() { return selected_item; };
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

//! No items or their attributes selected in the property editor.

TEST_F(AttributeEditorActionHandlerTest, NothingIsSelected)
{
  // no items are selected in the property editor
  auto handler = CreateActionHandler(nullptr);

  EXPECT_FALSE(handler->CanToggleEnabledFlag());
  EXPECT_FALSE(handler->CanSetDefaultType());
  EXPECT_FALSE(handler->CanSetPlaceholderType());
  EXPECT_FALSE(handler->CanEditAnyValue());
}

//! Name attribute is selected.

TEST_F(AttributeEditorActionHandlerTest, NameAttributeSelected)
{
  WaitItem item;
  auto name_attribute = GetNameItem(item);

  // no items are selected in the property editor
  auto handler = CreateActionHandler(name_attribute);

  EXPECT_TRUE(handler->CanToggleEnabledFlag());
  EXPECT_TRUE(handler->CanSetDefaultType());
  EXPECT_TRUE(handler->CanSetPlaceholderType());

  // it is not possible to call external editor
  EXPECT_FALSE(handler->CanEditAnyValue());
}

//! AnyValue on board of LocalVariableItem is selected.

TEST_F(AttributeEditorActionHandlerTest, AnyValueSelected)
{
  LocalVariableItem item;
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, item);

  // no items are selected in the property editor
  auto handler = CreateActionHandler(item.GetAnyValueItem());

  EXPECT_TRUE(handler->CanToggleEnabledFlag());

  // it is not possible to replace types with placeholders
  EXPECT_FALSE(handler->CanSetDefaultType());
  EXPECT_FALSE(handler->CanSetPlaceholderType());

  // it is possible to call external editor
  EXPECT_TRUE(handler->CanEditAnyValue());
}
