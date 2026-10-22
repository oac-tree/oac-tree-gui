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

#include "oac_tree_gui/transform/attribute_descriptor_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/instruction_descriptor.h>
#include <oac_tree_gui/domain/variable_descriptor.h>

#include <sup/dto/anytype.h>
#include <sup/oac-tree/attribute_definition.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Tests for functions from attribute_descriptor_helper.h.
class AttributeDescriptorHelperTest : public ::testing::Test
{
};

TEST_F(AttributeDescriptorHelperTest, OptionalScalar)
{
  const auto descriptor = CreateAttributeDescriptor(
      "UnknownType", "timeout", 0.0, /*mandatory*/ false, sup::oac_tree::AttributeCategory::kBoth);

  EXPECT_EQ(descriptor.name, "timeout");
  EXPECT_EQ(descriptor.default_value, mvvm::variant_t{0.0});
  EXPECT_TRUE(descriptor.tooltip.empty());
  EXPECT_FALSE(descriptor.mandatory);
  EXPECT_FALSE(descriptor.has_preset);
  EXPECT_EQ(descriptor.category, sup::oac_tree::AttributeCategory::kBoth);
  EXPECT_TRUE(descriptor.visible);
  EXPECT_TRUE(descriptor.allow_text);
  EXPECT_TRUE(descriptor.nullable);
}

TEST_F(AttributeDescriptorHelperTest, MandatoryScalar)
{
  const auto descriptor =
      CreateAttributeDescriptor("UnknownType", "timeout", 0.0, /*mandatory*/ true,
                                sup::oac_tree::AttributeCategory::kLiteral);

  EXPECT_TRUE(descriptor.mandatory);
  EXPECT_TRUE(descriptor.allow_text);
  EXPECT_FALSE(descriptor.nullable);
}

TEST_F(AttributeDescriptorHelperTest, String)
{
  for (const bool mandatory : {false, true})
  {
    const auto descriptor =
        CreateAttributeDescriptor("UnknownType", "channel", std::string{}, mandatory,
                                  sup::oac_tree::AttributeCategory::kLiteral);

    EXPECT_EQ(descriptor.default_value, mvvm::variant_t{std::string{}});
    EXPECT_FALSE(descriptor.allow_text);
    EXPECT_FALSE(descriptor.nullable);
  }
}

TEST_F(AttributeDescriptorHelperTest, GuiAttributes)
{
  const auto is_root =
      CreateAttributeDescriptor("UnknownType", domainconstants::kIsRootAttribute, false, false,
                                sup::oac_tree::AttributeCategory::kLiteral);
  EXPECT_FALSE(is_root.visible);
  EXPECT_FALSE(is_root.allow_text);
  EXPECT_FALSE(is_root.nullable);

  const auto show_collapsed =
      CreateAttributeDescriptor("UnknownType", domainconstants::kShowCollapsedAttribute, false,
                                false, sup::oac_tree::AttributeCategory::kLiteral);
  EXPECT_TRUE(show_collapsed.visible);
  EXPECT_FALSE(show_collapsed.allow_text);
  EXPECT_TRUE(show_collapsed.nullable);
}

TEST_F(AttributeDescriptorHelperTest, Preset)
{
  const auto descriptor = CreateAttributeDescriptor(
      domainconstants::kRepeatInstructionType, domainconstants::kMaxCountAttribute, mvvm::int32{0},
      false, sup::oac_tree::AttributeCategory::kBoth);

  EXPECT_EQ(descriptor.default_value, mvvm::variant_t{mvvm::int32{-1}});
  EXPECT_TRUE(descriptor.has_preset);
  EXPECT_TRUE(descriptor.nullable);
  EXPECT_TRUE(descriptor.allow_text);
}

TEST_F(AttributeDescriptorHelperTest, FromDefinition)
{
  sup::oac_tree::AttributeDefinition definition("timeout", sup::dto::Float64Type);
  (void)definition.SetMandatory().SetCategory(sup::oac_tree::AttributeCategory::kBoth);

  const auto descriptor = CreateAttributeDescriptor("UnknownType", definition);

  EXPECT_EQ(descriptor.name, "timeout");
  EXPECT_EQ(descriptor.default_value, mvvm::variant_t{0.0});
  EXPECT_TRUE(descriptor.mandatory);
  EXPECT_EQ(descriptor.category, sup::oac_tree::AttributeCategory::kBoth);
  EXPECT_FALSE(descriptor.nullable);
  EXPECT_TRUE(descriptor.allow_text);
}

TEST_F(AttributeDescriptorHelperTest, FromNonScalarDefinitionThrows)
{
  const sup::oac_tree::AttributeDefinition definition(
      "data", sup::dto::AnyType{{"field", sup::dto::SignedInteger32Type}});

  EXPECT_THROW(CreateAttributeDescriptor("UnknownType", definition), RuntimeException);
}

TEST_F(AttributeDescriptorHelperTest, FindAttributeDescriptor)
{
  InstructionDescriptor instruction_descriptor;
  instruction_descriptor.attributes = {{"timeout", 0.0}, {"blocking", false}};
  const auto* blocking = FindAttributeDescriptor(instruction_descriptor, "blocking");
  ASSERT_NE(blocking, nullptr);
  EXPECT_EQ(blocking, &instruction_descriptor.attributes.at(1));
  EXPECT_EQ(FindAttributeDescriptor(instruction_descriptor, "unknown"), nullptr);

  VariableDescriptor variable_descriptor;
  EXPECT_EQ(FindAttributeDescriptor(variable_descriptor, "channel"), nullptr);
  variable_descriptor.attributes = {{"channel", std::string{}}};
  EXPECT_EQ(FindAttributeDescriptor(variable_descriptor, "channel"),
            &variable_descriptor.attributes.at(0));
}

TEST_F(AttributeDescriptorHelperTest, GetAttributeNames)
{
  InstructionDescriptor instruction_descriptor;
  EXPECT_TRUE(GetAttributeNames(instruction_descriptor).empty());
  instruction_descriptor.attributes = {{"timeout", 0.0}, {"blocking", false}};
  const std::vector<std::string> expected_instruction_names{"timeout", "blocking"};
  EXPECT_EQ(GetAttributeNames(instruction_descriptor), expected_instruction_names);

  VariableDescriptor variable_descriptor;
  variable_descriptor.attributes = {{"channel", std::string{}}};
  const std::vector<std::string> expected_variable_names{"channel"};
  EXPECT_EQ(GetAttributeNames(variable_descriptor), expected_variable_names);
}

}  // namespace oac_tree_gui::test
