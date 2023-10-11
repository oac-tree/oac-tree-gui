/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/transform/attribute_item_transform_helper.h"

#include <sequencergui/model/attribute_item.h>
#include <sup/gui/model/scalar_conversion_utils.h>

#include <mvvm/model/compound_item.h>
#include <mvvm/model/item_utils.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/sequencer/attribute_definition.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for functions from variable_item_transform_utils.h

class AttributeItemTransformHelperTests : public ::testing::Test
{
};

TEST_F(AttributeItemTransformHelperTests, IsPlaceholderAttribute)
{
  EXPECT_TRUE(IsPlaceholderAttribute("$par"));
  EXPECT_FALSE(IsPlaceholderAttribute("@par"));
  EXPECT_FALSE(IsPlaceholderAttribute("par"));
  EXPECT_FALSE(IsPlaceholderAttribute(""));
}

TEST_F(AttributeItemTransformHelperTests, IsReferenceAttribute)
{
  EXPECT_FALSE(IsReferenceAttribute("$par"));
  EXPECT_TRUE(IsReferenceAttribute("@par"));
  EXPECT_FALSE(IsReferenceAttribute("par"));
  EXPECT_FALSE(IsReferenceAttribute(""));
}

//! Testing AddPropertyFromDefinition helper method.

TEST_F(AttributeItemTransformHelperTests, AddPropertyFromDefinition)
{
  const std::string attribute_name("attr");
  const sup::sequencer::AttributeDefinition attr(attribute_name, sup::dto::SignedInteger32Type);

  mvvm::CompoundItem item;
  auto property = AddPropertyFromDefinitionV2(attr, item);
  ASSERT_NE(property, nullptr);

  // validating that CompoundItem got new property item
  EXPECT_TRUE(mvvm::utils::HasTag(item, attribute_name));

  std::vector<std::string> expected_tags({attribute_name});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);

  auto property_item = item.GetItem({attribute_name});
  ASSERT_EQ(property_item, property);
  EXPECT_EQ(property_item->GetDisplayName(), attribute_name);
  EXPECT_NE(dynamic_cast<AttributeItem*>(property_item), nullptr);

  EXPECT_EQ(property_item->Data<int>(), 0);

  // Checking that we can convert AnyValueScalarItem property back to AnyValue
  sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type};

  auto anyvalue_item = item.GetItem<AttributeItem>(attr.GetName());
  auto any_value = sup::gui::GetAnyValueFromScalar(anyvalue_item->Data());

  EXPECT_EQ(expected_anyvalue, any_value);
}
