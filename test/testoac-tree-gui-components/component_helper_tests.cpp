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

#include "oac_tree_gui/components/component_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/variable_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

namespace oac_tree_gui::test
{

//! Tests for helper functions from component_helper.h.
class ComponentHelperTest : public ::testing::Test
{
public:
  //! Returns a local variable with the given value.
  static std::unique_ptr<VariableItem> CreateVariable(const sup::dto::AnyValue& anyvalue)
  {
    auto result = CreateVariableItem(domainconstants::kLocalVariableType);
    SetAnyValue(anyvalue, *result);
    return result;
  }
};

TEST_F(ComponentHelperTest, GetInlineValueItemForScalar)
{
  auto item = CreateVariable(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  EXPECT_EQ(GetInlineValueItem(*item), item->GetAnyValueItem());
}

TEST_F(ComponentHelperTest, GetInlineValueItemForStructWithSingleScalar)
{
  {  // field "value"
    auto item = CreateVariable({{{"value", {sup::dto::SignedInteger32Type, 42}}}});
    EXPECT_EQ(GetInlineValueItem(*item), item->GetAnyValueItem()->GetChildren().at(0));
  }

  {  // field name doesn't matter
    auto item = CreateVariable({{{"x", {sup::dto::SignedInteger32Type, 42}}}});
    EXPECT_EQ(GetInlineValueItem(*item), item->GetAnyValueItem()->GetChildren().at(0));
  }
}

TEST_F(ComponentHelperTest, GetInlineValueItemForOtherValues)
{
  {  // absent
    auto item = CreateVariableItem(domainconstants::kLocalVariableType);
    if (auto* anyvalue_item = item->GetAnyValueItem(); anyvalue_item != nullptr)
    {
      mvvm::utils::RemoveItem(*anyvalue_item);
    }
    EXPECT_EQ(GetInlineValueItem(*item), nullptr);
  }

  {  // empty
    auto item = CreateVariable(sup::dto::AnyValue{});
    EXPECT_EQ(GetInlineValueItem(*item), nullptr);
  }

  {  // empty struct
    auto item = CreateVariable(sup::dto::EmptyStruct());
    EXPECT_EQ(GetInlineValueItem(*item), nullptr);
  }

  {  // struct with two fields
    auto item = CreateVariable(
        {{{"a", {sup::dto::SignedInteger32Type, 1}}, {"b", {sup::dto::SignedInteger32Type, 2}}}});
    EXPECT_EQ(GetInlineValueItem(*item), nullptr);
  }

  {  // struct with a single struct field
    const sup::dto::AnyValue inner = {{{"value", {sup::dto::SignedInteger32Type, 42}}}};
    auto item = CreateVariable({{{"value", inner}}});
    EXPECT_EQ(GetInlineValueItem(*item), nullptr);
  }

  {  // array with a single element
    auto item = CreateVariable(sup::dto::ArrayValue({{sup::dto::SignedInteger32Type, 42}}));
    EXPECT_EQ(GetInlineValueItem(*item), nullptr);
  }
}

}  // namespace oac_tree_gui::test
