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

#include <mvvm/model/compound_item.h>
#include <mvvm/model/item_utils.h>

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/sequencer/attribute_handler.h>

#include <gtest/gtest.h>

/**
 * @brief Add property to item using sequencer attribute definition.
 * @param attr Attribute definition to use for property construction.
 * @param item Compound item which will get a property on board.
 */

void AddPropertyFromDefinition(const sup::sequencer::AttributeDefinition& attr,
                               mvvm::CompoundItem& item)
{
  auto type_name = attr.GetType().GetTypeName();

  // In the absence of other sources of the information we can only use attribute name
  // for both, display name and tag name of the new property item.
  item.AddProperty(attr.GetName(), sup::gui::GetVariantForAnyValueTypeName(type_name));
}

class NewAttributeHandlerTest : public ::testing::Test
{
};

//! Add property to empty CompoundItem using AttributeDefinition.

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
}
