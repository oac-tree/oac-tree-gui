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

#include "oac_tree_gui/transform/variable_item_transform_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/attribute_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/variable_descriptor_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_info.h>
#include <sup/oac-tree/workspace_info.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>
#include <testutils/universal_items.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests of helper methods from variable_item_transform_helper.h
 */
class VariableItemTransformHelperTest : public ::testing::Test
{
};

//! Item from variable info, as for jobs.
TEST_F(VariableItemTransformHelperTest, CreateVariableItemFromInfo)
{
  const sup::dto::uint32 variable_id{0};
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  const std::vector<sup::oac_tree::AttributeInfo> attributes(
      {{domainconstants::kNameAttribute, expected_name},
       {domainconstants::kTypeAttribute, expected_type},
       {domainconstants::kValueAttribute, expected_value}});
  const sup::oac_tree::VariableInfo info(domainconstants::kLocalVariableType, variable_id,
                                         attributes);

  auto variable_item = CreateVariableItem(CreateVariableNode(info));

  EXPECT_EQ(variable_item->GetType(), mvvm::GetTypeName<VariableItem>());
  EXPECT_EQ(variable_item->GetDomainType(), domainconstants::kLocalVariableType);
  EXPECT_EQ(variable_item->GetDisplayName(), expected_name);
  EXPECT_FALSE(variable_item->IsAvailable());

  const std::vector<std::string> expected_tags(
      {itemconstants::kIsAvailable, itemconstants::kAttributes, itemconstants::kAnyValueTag});
  EXPECT_EQ(mvvm::utils::RegisteredTags(*variable_item), expected_tags);

  // JSON type and value are not properties
  EXPECT_TRUE(mvvm::utils::RegisteredTags(*variable_item->GetAttributes()).empty());

  ASSERT_NE(variable_item->GetAnyValueItem(), nullptr);
  EXPECT_EQ(variable_item->GetAnyValueItem()->GetType(),
            mvvm::GetTypeName<sup::gui::AnyValueEmptyItem>());
}

//! Item from variable info: attributes are strings. No plugin is needed.
TEST_F(VariableItemTransformHelperTest, CreateVariableItemFromInfoWithAttributes)
{
  const std::vector<sup::oac_tree::AttributeInfo> attributes(
      {{domainconstants::kNameAttribute, "abc"},
       {domainconstants::kChannelAttribute, "CHANNEL"},
       {"custom", "42"}});
  const sup::oac_tree::VariableInfo info("UnknownVariable", 0, attributes);

  auto variable_item = CreateVariableItem(CreateVariableNode(info));

  EXPECT_EQ(variable_item->GetDomainType(), "UnknownVariable");
  EXPECT_EQ(variable_item->GetName(), "abc");

  const std::vector<std::string> expected_tags({domainconstants::kChannelAttribute, "custom"});
  EXPECT_EQ(mvvm::utils::RegisteredTags(*variable_item->GetAttributes()), expected_tags);

  auto* channel = GetChannelItem(*variable_item);
  ASSERT_NE(channel, nullptr);
  EXPECT_EQ(channel->Data<std::string>(), "CHANNEL");

  EXPECT_EQ(variable_item->GetAttribute<std::string>("custom"), "42");
}

//! One item type for all domain variable types.
TEST_F(VariableItemTransformHelperTest, CreateVariableItem)
{
  std::vector<std::string> domain_types = {domainconstants::kLocalVariableType,
                                           domainconstants::kFileVariableType};
  if (IsSequencerPluginEpicsAvailable())
  {
    domain_types.insert(domain_types.end(), {domainconstants::kChannelAccessVariableType,
                                             domainconstants::kPvAccessClientVariableType,
                                             domainconstants::kPvAccessServerVariableType});
  }

  for (const auto& domain_type : domain_types)
  {
    auto item = CreateVariableItem(domain_type);
    EXPECT_EQ(item->GetType(), mvvm::GetTypeName<VariableItem>()) << domain_type;
    EXPECT_EQ(item->GetDomainType(), domain_type);
    EXPECT_EQ(item->GetDisplayName(), domain_type);
  }
}

//! Domain variable unknown to the GUI.
TEST_F(VariableItemTransformHelperTest, CreateVariableItemOfUnknownType)
{
  test::UnknownDomainVariable::RegisterUnknownDomainVariable();

  auto item = CreateVariableItem(test::UnknownDomainVariable::Type);
  EXPECT_EQ(item->GetType(), mvvm::GetTypeName<VariableItem>());
  EXPECT_EQ(item->GetDomainType(), test::UnknownDomainVariable::Type);
}

//! Item from the node of a domain variable.
TEST_F(VariableItemTransformHelperTest, CreateVariableItemFromNode)
{
  auto domain_variable = CreateDomainVariable(domainconstants::kLocalVariableType);

  {  // default constructed domain variable
    auto item = CreateVariableItem(CreateVariableNode(*domain_variable));

    EXPECT_EQ(item->GetType(), mvvm::GetTypeName<VariableItem>());
    EXPECT_EQ(item->GetDomainType(), domainconstants::kLocalVariableType);
    EXPECT_EQ(item->GetDisplayName(), domainconstants::kLocalVariableType);

    const std::vector<std::string> expected_tags(
        {itemconstants::kIsAvailable, itemconstants::kAttributes, itemconstants::kAnyValueTag});
    EXPECT_EQ(mvvm::utils::RegisteredTags(*item), expected_tags);

    // JSON type and value are not properties
    const std::vector<std::string> expected_attribute_tags(
        {domainconstants::kDynamicTypeAttribute});
    EXPECT_EQ(mvvm::utils::RegisteredTags(*item->GetAttributes()), expected_attribute_tags);

    // nullable attribute without text
    auto* dynamic_type =
        item->GetAttributes()->FindItem<AttributeItem>(domainconstants::kDynamicTypeAttribute);
    ASSERT_NE(dynamic_type, nullptr);
    EXPECT_EQ(dynamic_type->GetState(), AttributeState::kNotSet);
    EXPECT_EQ(dynamic_type->GetDefaultValue(), mvvm::variant_t(false));
    EXPECT_FALSE(dynamic_type->IsTextAllowed());

    // no type attribute: empty AnyValueItem
    ASSERT_NE(item->GetAnyValueItem(), nullptr);
    EXPECT_EQ(item->GetAnyValueItem()->GetType(), mvvm::GetTypeName<sup::gui::AnyValueEmptyItem>());
  }

  (void)domain_variable->AddAttribute(domainconstants::kNameAttribute, "abc");
  (void)domain_variable->AddAttribute(domainconstants::kDynamicTypeAttribute, "true");
  (void)domain_variable->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"int32"})RAW");

  {  // domain variable with attributes
    auto item = CreateVariableItem(CreateVariableNode(*domain_variable));

    EXPECT_EQ(item->GetDisplayName(), "abc");
    EXPECT_TRUE(item->GetAttribute<bool>(domainconstants::kDynamicTypeAttribute));
    EXPECT_EQ(GetAnyValue(*item), sup::dto::AnyValue(sup::dto::SignedInteger32Type, 0));
  }
}

//! Mandatory string attribute always has a value, optional attribute starts not set.
TEST_F(VariableItemTransformHelperTest, CreateVariableItemWithMandatoryAttribute)
{
  auto item = CreateVariableItem(domainconstants::kFileVariableType);

  auto* file = item->GetAttributes()->FindItem<AttributeItem>(domainconstants::kFileNameAttribute);
  EXPECT_EQ(file->GetState(), AttributeState::kValue);
  EXPECT_FALSE(file->HasFlag(mvvm::Appearance::kNullable));

  auto* pretty =
      item->GetAttributes()->FindItem<AttributeItem>(domainconstants::kPrettyJsonAttribute);
  EXPECT_EQ(pretty->GetState(), AttributeState::kNotSet);
  EXPECT_TRUE(pretty->HasFlag(mvvm::Appearance::kNullable));

  // not set attribute and empty string are not sent to the domain
  item->SetName("abc");
  const std::vector<sup::oac_tree::AttributeInfo> expected_attributes{
      {domainconstants::kNameAttribute, "abc"}};
  EXPECT_EQ(CreateDomainVariable(*item)->GetStringAttributes(), expected_attributes);
}

//! Variable attributes take no text: a placeholder in a non-string attribute is not parsed, in a
//! string attribute it is the value.
TEST_F(VariableItemTransformHelperTest, CreateVariableItemWithPlaceholder)
{
  auto domain_variable = CreateDomainVariable(domainconstants::kFileVariableType);
  (void)domain_variable->AddAttribute(domainconstants::kFileNameAttribute, "$file");
  (void)domain_variable->AddAttribute(domainconstants::kPrettyJsonAttribute, "$pretty");

  auto item = CreateVariableItem(CreateVariableNode(*domain_variable));

  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kFileNameAttribute), "$file");
  auto* pretty =
      item->GetAttributes()->FindItem<AttributeItem>(domainconstants::kPrettyJsonAttribute);
  EXPECT_EQ(pretty->GetState(), AttributeState::kNotSet);
}

//! An attribute unknown to the descriptor becomes a string attribute and goes back to the domain.
TEST_F(VariableItemTransformHelperTest, CreateVariableItemWithUnknownAttribute)
{
  auto node = CreateVariableNode(domainconstants::kLocalVariableType);
  node.attributes = {{"custom", "abc"}};

  auto item = CreateVariableItem(node);
  EXPECT_EQ(item->GetAttribute<std::string>("custom"), "abc");
  EXPECT_FALSE(item->GetAttributes()->FindItem<AttributeItem>("custom")->IsTextAllowed());

  auto domain_variable = CreateDomainVariable(*item);
  EXPECT_EQ(domain_variable->GetAttributeString("custom"), "abc");
}

//! Domain variable from an item without domain type.
TEST_F(VariableItemTransformHelperTest, CreateDomainVariableFromBareItem)
{
  const VariableItem item;
  EXPECT_THROW(CreateDomainVariable(item), sup::oac_tree::InvalidOperationException);
}

//! Domain variable from an item: attributes with a value, name, JSON type and value.
TEST_F(VariableItemTransformHelperTest, CreateDomainVariable)
{
  auto item = CreateVariableItem(domainconstants::kFileVariableType);
  item->SetName("abc");
  item->SetAttribute(domainconstants::kFileNameAttribute, std::string("file.json"));

  {  // not set attributes and empty AnyValue are not sent
    auto domain_variable = CreateDomainVariable(*item);
    EXPECT_EQ(domain_variable->GetType(), domainconstants::kFileVariableType);
    const std::vector<sup::oac_tree::AttributeInfo> expected_attributes{
        {domainconstants::kFileNameAttribute, "file.json"},
        {domainconstants::kNameAttribute, "abc"}};
    EXPECT_EQ(domain_variable->GetStringAttributes(), expected_attributes);
  }

  item->SetAttribute(domainconstants::kPrettyJsonAttribute, true);

  {  // attribute with a value is sent
    auto domain_variable = CreateDomainVariable(*item);
    EXPECT_EQ(domain_variable->GetAttributeString(domainconstants::kPrettyJsonAttribute), "true");
  }
}

TEST_F(VariableItemTransformHelperTest, PopulateWorkspaceItem)
{
  {  // attempt to populate non-empty workspace
    WorkspaceItem workspace_item;
    InsertVariable(domainconstants::kLocalVariableType, &workspace_item);

    const sup::oac_tree::WorkspaceInfo info;
    EXPECT_THROW(PopulateWorkspaceItem(info, &workspace_item), RuntimeException);
  }

  const std::size_t variable_id{0};
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  const std::vector<sup::oac_tree::AttributeInfo> attributes(
      {{domainconstants::kNameAttribute, expected_name},
       {domainconstants::kTypeAttribute, expected_type},
       {domainconstants::kValueAttribute, expected_value}});

  const sup::oac_tree::VariableInfo info0(
      domainconstants::kLocalVariableType, 0,
      {{domainconstants::kTypeAttribute, expected_type}, {domainconstants::kValueAttribute, "42"}});

  const sup::oac_tree::VariableInfo info1(
      domainconstants::kLocalVariableType, 1,
      {{domainconstants::kTypeAttribute, expected_type}, {domainconstants::kValueAttribute, "43"}});

  sup::oac_tree::WorkspaceInfo workspace_info;
  workspace_info.AddVariableInfo("var0", info0);
  workspace_info.AddVariableInfo("var1", info1);

  WorkspaceItem workspace_item;
  auto index_to_variable_item = PopulateWorkspaceItem(workspace_info, &workspace_item);

  auto variable_items = workspace_item.GetVariables();
  ASSERT_EQ(index_to_variable_item.size(), 2);
  ASSERT_EQ(variable_items.size(), 2);
  EXPECT_EQ(variable_items[0], index_to_variable_item[0]);
  EXPECT_EQ(variable_items[1], index_to_variable_item[1]);

  EXPECT_TRUE(sup::dto::IsEmptyValue(GetAnyValue(*variable_items[0])));
  EXPECT_TRUE(sup::dto::IsEmptyValue(GetAnyValue(*variable_items[1])));
}

TEST_F(VariableItemTransformHelperTest, PopulateWorkspaceItemFromDomain)
{
  const std::string procedure_xml = R"RAW(
<Procedure>
  <Wait timeout="42"/>
  <Workspace>
    <Local name="counter" type='{"type":"int32"}' value="0"/>
    <Local name="one" type='{"type":"int32"}' value="1"/>
  </Workspace>
</Procedure>
)RAW";
  auto procedure = sup::oac_tree::ParseProcedureString(procedure_xml);
  procedure->Setup();

  WorkspaceItem workspace_item;
  auto variables = PopulateWorkspaceItem(procedure->GetWorkspace(), nullptr, &workspace_item);

  ASSERT_EQ(variables.size(), 2);
  ASSERT_EQ(workspace_item.GetTotalItemCount(), 2);
  // validating indices
  EXPECT_EQ(variables[0], workspace_item.GetVariables().at(0));
  EXPECT_EQ(variables[1], workspace_item.GetVariables().at(1));

  EXPECT_TRUE(test::IsEqual(*variables[0], sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}));
  EXPECT_TRUE(test::IsEqual(*variables[1], sup::dto::AnyValue{sup::dto::SignedInteger32Type, 1}));
}

}  // namespace oac_tree_gui::test
