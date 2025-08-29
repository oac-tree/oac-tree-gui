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

#include "oac_tree_gui/components/custom_children_strategies.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Testing custom strategies from custom_children_strategies.h
 */
class CustomChildrenStrategiesTest : public ::testing::Test
{
public:
};

//! Testing VariableTableChildrenStrategy.
TEST_F(CustomChildrenStrategiesTest, VariableTableChildrenStrategy)
{
  {  // workspace with single variable
    WorkspaceItem workspace;
    auto variable0 = workspace.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
    auto variable1 = workspace.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());

    const VariableTableChildrenStrategy strategy;
    auto children = strategy.GetChildren(&workspace);
    EXPECT_EQ(children, std::vector<mvvm::SessionItem*>({variable0, variable1}));
  }

  {  // single local variable
    LocalVariableItem item;

    const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
    SetAnyValue(anyvalue, item);
    item.SetName("abc");

    const VariableTableChildrenStrategy strategy;
    auto children = strategy.GetChildren(&item);

    // row of children is empty
    ASSERT_TRUE(children.empty());
  }

  {  // single local variable with structure
    LocalVariableItem item;

    const sup::dto::AnyValue anyvalue = {{{"signed", {sup::dto::SignedInteger32Type, 42}}}};
    SetAnyValue(anyvalue, item);
    item.SetName("abc");

    const VariableTableChildrenStrategy strategy;
    auto children = strategy.GetChildren(&item);

    // row of should contain our struct item
    ASSERT_EQ(children.size(), 1);
    EXPECT_EQ(children.at(0)->GetType(), mvvm::GetTypeName<sup::gui::AnyValueStructItem>());

    // struct has to contain only one child (no additional items representing a type
    children = strategy.GetChildren(item.GetAnyValueItem());
    ASSERT_EQ(children.size(), 1);
  }
}

}  // namespace oac_tree_gui::test
