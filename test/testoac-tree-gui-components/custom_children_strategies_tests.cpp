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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

#include "oac-tree-gui/viewmodel/custom_children_strategies.h"
#include <oac-tree-gui/domain/domain_constants.h>
#include <oac-tree-gui/model/item_constants.h>
#include <oac-tree-gui/model/standard_variable_items.h>
#include <oac-tree-gui/model/workspace_item.h>
#include <oac-tree-gui/transform/anyvalue_item_transform_helper.h>

namespace sequencergui::test
{

//! Tests for strategies from standard_children_strategies.h

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

    VariableTableChildrenStrategy strategy;
    auto children = strategy.GetChildren(&workspace);
    EXPECT_EQ(children, std::vector<mvvm::SessionItem*>({variable0, variable1}));
  }

  {  // single local variable
    LocalVariableItem item;

    const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
    SetAnyValue(anyvalue, item);
    item.SetName("abc");

    VariableTableChildrenStrategy strategy;
    auto children = strategy.GetChildren(&item);

    // row of children is empty
    ASSERT_TRUE(children.empty());
  }

  {  // single local variable with structure
    LocalVariableItem item;

    const sup::dto::AnyValue anyvalue = {{{"signed", {sup::dto::SignedInteger32Type, 42}}}};
    SetAnyValue(anyvalue, item);
    item.SetName("abc");

    VariableTableChildrenStrategy strategy;
    auto children = strategy.GetChildren(&item);

    // row of should contain our struct item
    ASSERT_EQ(children.size(), 1);
    EXPECT_EQ(children.at(0)->GetType(),
              std::string(sup::gui::AnyValueStructItem::GetStaticType()));
  }
}

}  // namespace sequencergui::test
