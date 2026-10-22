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

#include "oac_tree_gui/transform/variable_descriptor_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/transform/attribute_descriptor_helper.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_info.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

namespace
{

//! Creates a typed descriptor for the given domain type.
VariableDescriptor CreateTypedDescriptor(const std::string& domain_type)
{
  return CreateVariableDescriptor(*CreateDomainVariable(domain_type));
}

}  // namespace

//! Tests for functions from variable_descriptor_helper.h.
class VariableDescriptorHelperTest : public ::testing::Test
{
};

TEST_F(VariableDescriptorHelperTest, CreateVariableDescriptorFromLocal)
{
  const auto descriptor = CreateTypedDescriptor(domainconstants::kLocalVariableType);

  EXPECT_EQ(descriptor.domain_type, domainconstants::kLocalVariableType);
  const std::vector<std::string> expected_names{
      domainconstants::kNameAttribute, domainconstants::kTypeAttribute,
      domainconstants::kValueAttribute, domainconstants::kDynamicTypeAttribute};
  EXPECT_EQ(GetAttributeNames(descriptor), expected_names);

  const auto* type = FindAttributeDescriptor(descriptor, domainconstants::kTypeAttribute);
  EXPECT_EQ(type->default_value, mvvm::variant_t{std::string{}});
  EXPECT_FALSE(type->nullable);

  const auto* dynamic_type =
      FindAttributeDescriptor(descriptor, domainconstants::kDynamicTypeAttribute);
  EXPECT_EQ(dynamic_type->default_value, mvvm::variant_t{false});
  EXPECT_FALSE(dynamic_type->mandatory);
  EXPECT_FALSE(dynamic_type->has_preset);
  EXPECT_TRUE(dynamic_type->nullable);
  EXPECT_TRUE(dynamic_type->visible);
  EXPECT_FALSE(dynamic_type->allow_text);
}

TEST_F(VariableDescriptorHelperTest, CreateVariableDescriptorFromFile)
{
  const auto descriptor = CreateTypedDescriptor(domainconstants::kFileVariableType);

  const auto* file = FindAttributeDescriptor(descriptor, domainconstants::kFileNameAttribute);
  ASSERT_NE(file, nullptr);
  EXPECT_EQ(file->default_value, mvvm::variant_t{std::string{}});
  EXPECT_TRUE(file->mandatory);
  EXPECT_FALSE(file->nullable);
  EXPECT_FALSE(file->allow_text);

  const auto* pretty = FindAttributeDescriptor(descriptor, domainconstants::kPrettyJsonAttribute);
  ASSERT_NE(pretty, nullptr);
  EXPECT_EQ(pretty->default_value, mvvm::variant_t{false});
  EXPECT_TRUE(pretty->nullable);
  EXPECT_FALSE(pretty->allow_text);
}

TEST_F(VariableDescriptorHelperTest, CreateVariableDescriptorFromChannelAccess)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const auto descriptor = CreateTypedDescriptor(domainconstants::kChannelAccessVariableType);

  const auto* channel = FindAttributeDescriptor(descriptor, domainconstants::kChannelAttribute);
  ASSERT_NE(channel, nullptr);
  EXPECT_EQ(channel->default_value, mvvm::variant_t{std::string{}});
  EXPECT_TRUE(channel->mandatory);
  EXPECT_FALSE(channel->nullable);
  EXPECT_FALSE(channel->allow_text);
}

//! All registered variable types have scalar attribute definitions and take no text.
TEST_F(VariableDescriptorHelperTest, AllRegisteredVariableTypes)
{
  for (const auto& domain_type : GetDomainVariableNames())
  {
    VariableDescriptor descriptor;
    EXPECT_NO_THROW(descriptor = CreateTypedDescriptor(domain_type)) << domain_type;
    EXPECT_EQ(descriptor.domain_type, domain_type);
    for (const auto& attribute : descriptor.attributes)
    {
      EXPECT_FALSE(attribute.allow_text) << domain_type << "/" << attribute.name;
    }
  }
}

//! Info descriptor holds the domain type only. No plugin is needed.
TEST_F(VariableDescriptorHelperTest, InfoDescriptor)
{
  const sup::oac_tree::VariableInfo info("UnknownVariable", 0,
                                         {{domainconstants::kChannelAttribute, "CHANNEL"}});
  const auto descriptor = CreateVariableDescriptor(info);

  EXPECT_EQ(descriptor.domain_type, "UnknownVariable");
  EXPECT_TRUE(descriptor.attributes.empty());
}

TEST_F(VariableDescriptorHelperTest, NodeFromDomainType)
{
  const auto node = CreateVariableNode(domainconstants::kLocalVariableType);

  EXPECT_EQ(node.descriptor.domain_type, domainconstants::kLocalVariableType);
  EXPECT_EQ(GetAttributeNames(node.descriptor),
            GetAttributeNames(CreateTypedDescriptor(domainconstants::kLocalVariableType)));
  EXPECT_TRUE(node.attributes.empty());
  EXPECT_EQ(node.index, 0);
}

TEST_F(VariableDescriptorHelperTest, NodeFromUnknownDomainTypeThrows)
{
  EXPECT_THROW(CreateVariableNode("UnknownVariable"), sup::oac_tree::InvalidOperationException);
}

TEST_F(VariableDescriptorHelperTest, NodeFromDomainVariable)
{
  auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);
  (void)variable->AddAttribute(domainconstants::kNameAttribute, "var0");
  (void)variable->AddAttribute(domainconstants::kTypeAttribute, R"({"type":"uint32"})");
  (void)variable->AddAttribute(domainconstants::kValueAttribute, "42");

  const auto node = CreateVariableNode(*variable);

  EXPECT_EQ(node.descriptor.domain_type, domainconstants::kLocalVariableType);
  EXPECT_EQ(GetAttributeNames(node.descriptor),
            GetAttributeNames(CreateTypedDescriptor(domainconstants::kLocalVariableType)));
  const std::vector<sup::oac_tree::AttributeInfo> expected_attributes{
      {domainconstants::kNameAttribute, "var0"},
      {domainconstants::kTypeAttribute, R"({"type":"uint32"})"},
      {domainconstants::kValueAttribute, "42"}};
  EXPECT_EQ(node.attributes, expected_attributes);
  EXPECT_EQ(node.index, 0);
}

//! Node from variable info: info descriptor, attributes and index as given. No plugin.
TEST_F(VariableDescriptorHelperTest, NodeFromVariableInfo)
{
  const std::vector<sup::oac_tree::AttributeInfo> attributes{
      {domainconstants::kNameAttribute, "var0"}, {domainconstants::kChannelAttribute, "CHANNEL"}};
  const sup::oac_tree::VariableInfo info("UnknownVariable", 3, attributes);

  const auto node = CreateVariableNode(info);

  EXPECT_EQ(node.descriptor.domain_type, "UnknownVariable");
  EXPECT_TRUE(node.descriptor.attributes.empty());
  EXPECT_EQ(node.attributes, attributes);
  EXPECT_EQ(node.index, 3);
}

//! Node from variable info: JSON type and value are kept only if the initial value is provided.
TEST_F(VariableDescriptorHelperTest, NodeFromVariableInfoWithTypeAndValue)
{
  const std::vector<sup::oac_tree::AttributeInfo> attributes{
      {domainconstants::kNameAttribute, "var0"},
      {domainconstants::kTypeAttribute, R"({"type":"uint32"})"},
      {domainconstants::kValueAttribute, "42"}};
  const sup::oac_tree::VariableInfo info(domainconstants::kLocalVariableType, 0, attributes);

  const auto node = CreateVariableNode(info);

  if (itemconstants::kProvideVariableInfoInitialValue)
  {
    EXPECT_EQ(node.attributes, attributes);
  }
  else
  {
    const std::vector<sup::oac_tree::AttributeInfo> expected_attributes{
        {domainconstants::kNameAttribute, "var0"}};
    EXPECT_EQ(node.attributes, expected_attributes);
  }
}

}  // namespace oac_tree_gui::test
