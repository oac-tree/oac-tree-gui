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

#include "oac_tree_gui/composer/workspace_editor_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

namespace oac_tree_gui::test
{

//! Tests for methods from workspace_editor_helper.h.

class WorkspaceEditorHelperTest : public ::testing::Test
{
};

TEST_F(WorkspaceEditorHelperTest, UpdateVariableEditableProperty)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  WorkspaceItem workspace_item;

  auto var_item0 = InsertVariable(domainconstants::kLocalVariableType, &workspace_item);
  auto var_item1 = InsertVariable(domainconstants::kPvAccessServerVariableType, &workspace_item);

  EXPECT_TRUE(GetChannelItem(*var_item1)->IsEditable());

  UpdateVariableEditableProperty(/*is_running*/ true, workspace_item);
  EXPECT_FALSE(GetChannelItem(*var_item1)->IsEditable());

  UpdateVariableEditableProperty(/*is_running*/ false, workspace_item);
  EXPECT_TRUE(GetChannelItem(*var_item1)->IsEditable());
}

//! Validating helper method SetupNewVariable.
TEST_F(WorkspaceEditorHelperTest, SetupNewVariable)
{
  WorkspaceItem workspace_item;
  auto var_item0 = InsertVariable(domainconstants::kLocalVariableType, &workspace_item);
  SetupNewVariable(var_item0, workspace_item.GetVariableCount());

  EXPECT_EQ(var_item0->GetAnyValueItem()->GetDisplayName(),
            itemconstants::kAnyValueDefaultDisplayName);

  auto var_item1 = InsertVariable(domainconstants::kLocalVariableType, &workspace_item);
  SetupNewVariable(var_item1, workspace_item.GetVariableCount());

  EXPECT_EQ(var_item0->GetName(), "var1");
  EXPECT_EQ(var_item1->GetName(), "var2");

  const sup::dto::AnyValue expected_value(sup::dto::SignedInteger32Type, 0);

  ASSERT_TRUE(var_item0->GetAnyValueItem());
  EXPECT_EQ(sup::gui::CreateAnyValue(*var_item0->GetAnyValueItem()), expected_value);

  ASSERT_TRUE(var_item1->GetAnyValueItem());
  EXPECT_EQ(sup::gui::CreateAnyValue(*var_item1->GetAnyValueItem()), expected_value);

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  auto var_item2 = InsertVariable(domainconstants::kPvAccessClientVariableType, &workspace_item);
  EXPECT_FALSE(GetIsAvailableItem(*var_item2)->IsVisible());
  SetupNewVariable(var_item2, workspace_item.GetVariableCount());

  // "connected" property should be invisible, when variable is constructed via editor
  EXPECT_FALSE(GetIsAvailableItem(*var_item2)->IsVisible());

  // PvAccessClient variable is different, it shouldn't have just empty AnyValue
  EXPECT_EQ(var_item2->GetName(), "var3");
  ASSERT_NE(var_item2->GetAnyValueItem(), nullptr);
  EXPECT_TRUE(sup::dto::IsEmptyValue(sup::gui::CreateAnyValue(*var_item2->GetAnyValueItem())));
  EXPECT_EQ(var_item2->GetAnyValueItem()->GetDisplayName(),
            itemconstants::kAnyValueDefaultDisplayName);
}

}  // namespace oac_tree_gui::test
