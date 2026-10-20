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

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/instruction_descriptor.h>
#include <oac_tree_gui/model/attribute_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/transform/instruction_item_transform_helper.h>

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/item_utils.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

namespace
{

//! Creates an item from a hand-written node with the given attributes. No plugin needed.
std::unique_ptr<InstructionItem> CreateTestItem(const std::vector<AttributeDescriptor>& attributes)
{
  const InstructionNode node{
      {"SomeDomainInstruction", sup::oac_tree::Instruction::kAction, attributes}, {}, 0, {}};
  return CreateInstructionItemTree(node).root;
}

}  // namespace

/**
 * @brief Tests for InstructionItem class.
 */
class InstructionItemTest : public ::testing::Test
{
};

TEST_F(InstructionItemTest, InitialState)
{
  InstructionItem item;

  EXPECT_EQ(item.GetType(), "Instruction");
  EXPECT_EQ(item.GetType(), mvvm::GetTypeName<InstructionItem>());
  EXPECT_EQ(item.GetDisplayName(), "Instruction");
  EXPECT_TRUE(item.GetDomainType().empty());
  EXPECT_TRUE(item.GetInstructions().empty());
  EXPECT_TRUE(mvvm::utils::RegisteredTags(item).empty());

  const std::string expected_domain_type = "SomeDomainInstruction";
  item.SetDomainType(expected_domain_type);
  EXPECT_EQ(item.GetDomainType(), expected_domain_type);
}

TEST_F(InstructionItemTest, GuiProperties)
{
  auto item = CreateTestItem({});

  EXPECT_EQ(item->GetDomainType(), "SomeDomainInstruction");

  // these attributes are coming from the sequencer domain
  EXPECT_FALSE(mvvm::utils::HasTag(*item, domainconstants::kNameAttribute));
  EXPECT_FALSE(mvvm::utils::HasTag(*item, domainconstants::kIsRootAttribute));

  EXPECT_EQ(item->GetX(), 0);
  EXPECT_EQ(item->GetY(), 0);

  item->SetX(1.1);
  item->SetY(1.2);
  EXPECT_EQ(item->GetX(), 1.1);
  EXPECT_EQ(item->GetY(), 1.2);

  EXPECT_EQ(item->GetStatus(), InstructionStatus::kUndefined);
  EXPECT_EQ(item->GetBehavior(), itemconstants::kNativeBehavior);
  EXPECT_TRUE(item->GetInstructions().empty());
}

//! Testing SetAttribute and GetAttribute members.
TEST_F(InstructionItemTest, SetAndGetAttribute)
{
  auto item = CreateTestItem({{domainconstants::kTimeoutAttribute,
                               0.0,
                               {},
                               false,
                               false,
                               sup::oac_tree::AttributeCategory::kLiteral}});
  auto* property =
      item->GetAttributes()->FindItem<AttributeItem>(domainconstants::kTimeoutAttribute);
  ASSERT_NE(property, nullptr);

  // not mandatory, not set: no data
  EXPECT_TRUE(property->HasFlag(mvvm::Appearance::kNullable));
  EXPECT_FALSE(property->HasData());

  item->SetAttribute(domainconstants::kTimeoutAttribute, 42.0);
  EXPECT_EQ(item->GetAttribute<double>(domainconstants::kTimeoutAttribute), 42.0);

  // value type must match the attribute type
  EXPECT_THROW(item->SetAttribute(domainconstants::kTimeoutAttribute, 1), RuntimeException);

  item->SetAttributeText(domainconstants::kTimeoutAttribute, "$par");
  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kTimeoutAttribute), "$par");
  EXPECT_TRUE(property->HasText());

  item->UnsetAttribute(domainconstants::kTimeoutAttribute);
  EXPECT_FALSE(property->HasData());

  // unknown attribute
  EXPECT_THROW(item->SetAttribute("unknown", 42.0), RuntimeException);
}

//! Mandatory attribute always has data and can't be unset.
TEST_F(InstructionItemTest, MandatoryAttribute)
{
  auto item = CreateTestItem({{domainconstants::kTimeoutAttribute,
                               0.0,
                               {},
                               true,
                               true,
                               sup::oac_tree::AttributeCategory::kLiteral}});
  auto* property =
      item->GetAttributes()->FindItem<AttributeItem>(domainconstants::kTimeoutAttribute);
  ASSERT_NE(property, nullptr);
  EXPECT_FALSE(property->HasFlag(mvvm::Appearance::kNullable));
  EXPECT_EQ(item->GetAttribute<double>(domainconstants::kTimeoutAttribute), 0.0);

  EXPECT_THROW(item->UnsetAttribute(domainconstants::kTimeoutAttribute), RuntimeException);
}

//! Clone keeps domain type, attributes and GUI properties.
TEST_F(InstructionItemTest, Clone)
{
  auto item = CreateTestItem({{domainconstants::kTimeoutAttribute,
                               0.0,
                               {},
                               false,
                               false,
                               sup::oac_tree::AttributeCategory::kLiteral}});
  item->SetAttribute(domainconstants::kTimeoutAttribute, 42.0);
  item->SetX(1.1);

  auto clone = item->Clone();
  auto* instruction = dynamic_cast<InstructionItem*>(clone.get());
  ASSERT_NE(instruction, nullptr);
  EXPECT_EQ(instruction->GetType(), mvvm::GetTypeName<InstructionItem>());
  EXPECT_EQ(instruction->GetDomainType(), "SomeDomainInstruction");
  EXPECT_EQ(instruction->GetAttribute<double>(domainconstants::kTimeoutAttribute), 42.0);
  EXPECT_EQ(instruction->GetX(), 1.1);
}

}  // namespace oac_tree_gui::test
