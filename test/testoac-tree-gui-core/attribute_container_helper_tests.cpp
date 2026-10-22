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

#include "oac_tree_gui/model/attribute_container_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/attribute_item.h>
#include <oac_tree_gui/model/item_constants.h>

#include <mvvm/model/compound_item.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Tests for functions from attribute_container_helper.h.
class AttributeContainerHelperTest : public ::testing::Test
{
public:
  AttributeContainerHelperTest()
  {
    (void)m_item.SetData(std::string("TestType"), itemconstants::kDomainTypeNameRole);
    auto& attributes = m_item.AddProperty<mvvm::CompoundItem>(itemconstants::kAttributes);

    auto& timeout = attributes.AddProperty<AttributeItem>("timeout");
    timeout.SetAttributeType(0.0, /*is_nullable*/ true, /*allow_text*/ true);

    auto& channel = attributes.AddProperty<AttributeItem>("channel");
    channel.SetAttributeType(std::string(), /*is_nullable*/ false, /*allow_text*/ false);
  }

  //! Item with an attribute container: nullable double "timeout" that takes text, string "channel".
  mvvm::CompoundItem m_item;
};

TEST_F(AttributeContainerHelperTest, GetAttributeItem)
{
  const auto& const_item = m_item;
  auto* attributes = m_item.FindItem(itemconstants::kAttributes);

  EXPECT_EQ(&GetAttributeItem(m_item, "timeout"), attributes->FindItem("timeout"));
  EXPECT_EQ(&GetAttributeItem(const_item, "channel"), attributes->FindItem("channel"));

  EXPECT_THROW(GetAttributeItem(m_item, "unknown"), RuntimeException);

  // item without attribute container
  const mvvm::CompoundItem bare_item;
  EXPECT_THROW(GetAttributeItem(bare_item, "timeout"), RuntimeException);
}

TEST_F(AttributeContainerHelperTest, SetAndGetAttributeValue)
{
  // not set
  EXPECT_THROW(GetAttributeValue(m_item, "timeout"), RuntimeException);

  SetAttributeValue(m_item, "timeout", 42.0);
  EXPECT_EQ(GetAttributeValue(m_item, "timeout"), mvvm::variant_t(42.0));

  // a value removes the text
  (void)GetAttributeItem(m_item, "timeout").SetText("$par");
  EXPECT_THROW(GetAttributeValue(m_item, "timeout"), RuntimeException);
  SetAttributeValue(m_item, "timeout", 43.0);
  EXPECT_TRUE(GetAttributeItem(m_item, "timeout").GetText().empty());
  EXPECT_EQ(GetAttributeValue(m_item, "timeout"), mvvm::variant_t(43.0));

  // value type must match the attribute type
  EXPECT_THROW(SetAttributeValue(m_item, "timeout", 42), RuntimeException);
  EXPECT_THROW(SetAttributeValue(m_item, "unknown", 42.0), RuntimeException);

  // string attribute always has a value
  EXPECT_EQ(GetAttributeValue(m_item, "channel"), mvvm::variant_t(std::string()));
}

TEST_F(AttributeContainerHelperTest, UnsetAttributeValue)
{
  SetAttributeValue(m_item, "timeout", 42.0);
  UnsetAttributeValue(m_item, "timeout");
  EXPECT_EQ(GetAttributeItem(m_item, "timeout").GetState(), AttributeState::kNotSet);

  // not nullable
  EXPECT_THROW(UnsetAttributeValue(m_item, "channel"), RuntimeException);
  EXPECT_THROW(UnsetAttributeValue(m_item, "unknown"), RuntimeException);
}

}  // namespace oac_tree_gui::test
