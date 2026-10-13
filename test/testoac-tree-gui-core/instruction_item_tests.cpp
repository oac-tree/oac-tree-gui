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

#include "oac_tree_gui/model/instruction_item.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/item_utils.h>

#include <sup/dto/anytype.h>
#include <sup/oac-tree/attribute_definition.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for InstructionItem base class.
 */
class InstructionItemTest : public ::testing::Test
{
public:
  class TestItem : public InstructionItem
  {
  public:
    TestItem() : InstructionItem("test") { RegisterCommonProperties(); }

  private:
    void InitFromDomainImpl(const instruction_t* instruction) override {};
    void SetupDomainImpl(instruction_t* instruction) const override {};
  };
};

TEST_F(InstructionItemTest, TestItem)
{
  // Correctly initialised item
  TestItem item;

  EXPECT_EQ(item.GetType(), "test");

  EXPECT_TRUE(item.GetDomainType().empty());
  const std::string expected_domain_type = "SomeDomainInstruction";
  item.SetDomainType(expected_domain_type);
  EXPECT_EQ(item.GetDomainType(), expected_domain_type);

  // these attributes are coming from the sequencer domain
  EXPECT_FALSE(mvvm::utils::HasTag(item, domainconstants::kNameAttribute));
  EXPECT_FALSE(mvvm::utils::HasTag(item, domainconstants::kIsRootAttribute));

  EXPECT_EQ(item.GetX(), 0);
  EXPECT_EQ(item.GetY(), 0);

  item.SetX(1.1);
  item.SetY(1.2);
  EXPECT_EQ(item.GetX(), 1.1);
  EXPECT_EQ(item.GetY(), 1.2);

  EXPECT_EQ(item.GetStatus(), InstructionStatus::kUndefined);
  EXPECT_TRUE(item.GetInstructions().empty());
}

//! Testing SetAttribute and GetAttribute members.

TEST_F(InstructionItemTest, SetAndGetAttribute)
{
  TestItem item;
  const sup::oac_tree::AttributeDefinition attr(domainconstants::kTimeoutAttribute,
                                                sup::dto::Float64Type);
  auto property = AddPropertyFromDefinition(attr, item);
  ASSERT_NE(property, nullptr);
  EXPECT_FALSE(GetAttributeExposedFlag(*property));

  // value of not exposed attribute is still available
  EXPECT_EQ(item.GetAttribute<double>(domainconstants::kTimeoutAttribute), 0.0);

  item.SetAttribute(domainconstants::kTimeoutAttribute, 42.0);
  EXPECT_TRUE(GetAttributeExposedFlag(*property));
  EXPECT_EQ(item.GetAttribute<double>(domainconstants::kTimeoutAttribute), 42.0);

  // value type must match the attribute type
  EXPECT_THROW(item.SetAttribute(domainconstants::kTimeoutAttribute, 1), mvvm::RuntimeException);
}

}  // namespace oac_tree_gui::test
