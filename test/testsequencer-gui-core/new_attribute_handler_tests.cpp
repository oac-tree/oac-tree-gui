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

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/scalar_conversion_utils.h>

#include <mvvm/model/compound_item.h>
#include <mvvm/model/item_utils.h>

#include <sup/sequencer/attribute_handler.h>

#include <gtest/gtest.h>

/**
 * @brief Add property to item using sequencer attribute definition.
 * @param attr Attribute definition to use for property construction.
 * @param item Compound item which will get a property on board.
 *
 * @details We add PropertyItem and set's it's variant to the closest type as given by AnyType.
 */

void AddPropertyFromDefinition(const sup::sequencer::AttributeDefinition& attr,
                               mvvm::CompoundItem& item)
{
  auto type_name = attr.GetType().GetTypeName();

  // In the absence of other sources of the information we can only use attribute name
  // for both, display name and tag name of the new property item.
  item.AddProperty(attr.GetName(), sup::gui::GetVariantForAnyValueTypeName(type_name));
}

/**
 * @brief Add property to item using sequencer attribute definition.
 * @param attr Attribute definition to use for property construction.
 * @param item Compound item which will get a property on board.
 *
 * @details It's an alternative method which relies on AnyValueItem.
 */

// FIXME move convenience function to setup AnyValueScalarItem from AnyType to sup-gui-core

void AddScalarPropertyFromDefinition(const sup::sequencer::AttributeDefinition& attr,
                                     mvvm::CompoundItem& item)
{
  // In the absence of other sources of the information we can only use attribute name
  // for both, display name and tag name of the new property item.
  auto property = item.AddProperty<sup::gui::AnyValueScalarItem>(attr.GetName());
  property->SetAnyTypeName(attr.GetType().GetTypeName());
  property->SetDisplayName(attr.GetName());

  auto default_anyvalue = sup::dto::AnyValue(attr.GetType());

  sup::gui::SetDataFromScalar(default_anyvalue, *property);
}

class NewAttributeHandlerTest : public ::testing::Test
{
};

//! Add property to empty CompoundItem using AttributeDefinition.
//! We are using method AddPropertyFromStringDefinition which relies on PropertyItem.

TEST_F(NewAttributeHandlerTest, AddPropertyFromStringDefinition)
{
  const std::string attribute_name("attr");
  const sup::sequencer::AttributeDefinition attr(attribute_name, sup::dto::StringType);

  mvvm::CompoundItem item;
  AddPropertyFromDefinition(attr, item);

  // validating that CompoundItem got new property item
  EXPECT_TRUE(mvvm::utils::HasTag(item, attribute_name));
  auto property_item = item.GetItem({attribute_name});
  ASSERT_NE(property_item, nullptr);
  EXPECT_EQ(property_item->GetDisplayName(), attribute_name);

  EXPECT_TRUE(std::holds_alternative<std::string>(property_item->Data()));

  EXPECT_EQ(property_item->Data<std::string>(), std::string());

  // We do not have a way to convert our variant_t to AnyValue yet.
  // It is not hard to implement it, but may be it is better to use AnyValueScalarItem as
  // an underlying property. It is already implemented. See test below.
}

//! Add property to empty CompoundItem using AttributeDefinition.
//! We are using method AddPropertyFromStringDefinition which relies on AnyValueScalarItem.

TEST_F(NewAttributeHandlerTest, AddScalarPropertyFromDefinition)
{
  const std::string attribute_name("attr");
  const sup::sequencer::AttributeDefinition attr(attribute_name, sup::dto::SignedInteger32Type);

  mvvm::CompoundItem item;
  AddScalarPropertyFromDefinition(attr, item);

  // validating that CompoundItem got new property item
  EXPECT_TRUE(mvvm::utils::HasTag(item, attribute_name));
  auto property_item = item.GetItem({attribute_name});
  ASSERT_NE(property_item, nullptr);
  EXPECT_EQ(property_item->GetDisplayName(), attribute_name);
  EXPECT_NE(dynamic_cast<sup::gui::AnyValueScalarItem*>(property_item), nullptr);

  EXPECT_EQ(property_item->Data<int>(), 0);

  // Checking that we can convert AnyValueScalarItem property back to AnyValue
  sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type};

  auto anyvalue_item = item.GetItem<sup::gui::AnyValueScalarItem>(attr.GetName());
  auto any_value = GetAnyValueFromScalar(*anyvalue_item);

  EXPECT_EQ(expected_anyvalue, any_value);
}
