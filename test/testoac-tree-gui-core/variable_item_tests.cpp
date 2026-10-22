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

#include "oac_tree_gui/model/variable_item.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/attribute_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/variable_descriptor_helper.h>
#include <oac_tree_gui/transform/variable_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/property_item.h>

#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for VariableItem class.
 */
class VariableItemTest : public ::testing::Test
{
};

TEST_F(VariableItemTest, InitialState)
{
  const VariableItem item;

  EXPECT_EQ(item.GetType(), mvvm::GetTypeName<VariableItem>());
  EXPECT_EQ(item.GetDisplayName(), mvvm::GetTypeName<VariableItem>());

  const std::vector<std::string> expected_tags({itemconstants::kIsAvailable});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);
  EXPECT_TRUE(item.GetDomainType().empty());
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);
  EXPECT_FALSE(item.IsAvailable());
}

//! SetDomainType sets the domain type role only.
TEST_F(VariableItemTest, SetDomainType)
{
  VariableItem item;
  item.SetDomainType(domainconstants::kLocalVariableType);

  EXPECT_EQ(item.GetType(), mvvm::GetTypeName<VariableItem>());
  EXPECT_EQ(item.GetDomainType(), domainconstants::kLocalVariableType);
  EXPECT_EQ(item.GetDisplayName(), mvvm::GetTypeName<VariableItem>());

  const std::vector<std::string> expected_tags({itemconstants::kIsAvailable});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);
}

//! Clone keeps the domain type, the attributes and the AnyValue.
TEST_F(VariableItemTest, Clone)
{
  auto item = CreateVariableItem(domainconstants::kFileVariableType);
  item->SetAttribute(domainconstants::kFileNameAttribute, std::string("abc"));
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, *item);

  auto clone = item->Clone();
  auto* variable_clone = dynamic_cast<VariableItem*>(clone.get());
  ASSERT_NE(variable_clone, nullptr);
  EXPECT_EQ(variable_clone->GetDomainType(), domainconstants::kFileVariableType);
  EXPECT_EQ(variable_clone->GetAttribute<std::string>(domainconstants::kFileNameAttribute), "abc");
  EXPECT_EQ(GetAnyValue(*variable_clone), (sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}));
}

TEST_F(VariableItemTest, ChannelAccessVariable)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  auto item = CreateVariableItem(domainconstants::kChannelAccessVariableType);
  EXPECT_FALSE(item->GetName().empty());
  EXPECT_TRUE(item->GetAttribute<std::string>(domainconstants::kChannelAttribute).empty());
  EXPECT_FALSE(item->IsAvailable());

  item->SetName("abc");
  EXPECT_EQ(item->GetName(), std::string("abc"));

  item->SetAttribute(domainconstants::kChannelAttribute, std::string("def"));
  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kChannelAttribute),
            std::string("def"));

  item->SetIsAvailable(true);
  EXPECT_TRUE(item->IsAvailable());
}

TEST_F(VariableItemTest, ChannelAccessVariablePropertyAppearance)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  auto item = CreateVariableItem(domainconstants::kChannelAccessVariableType);
  auto children = item->GetAllItems();

  // connected flag, attribute container, AnyValueItem
  ASSERT_EQ(children.size(), 3);

  auto available = dynamic_cast<mvvm::PropertyItem*>(children.at(0));
  ASSERT_NE(available, nullptr);
  EXPECT_EQ(available->GetDisplayName(), itemconstants::kIsAvailable);
  EXPECT_EQ(children.at(1), item->GetAttributes());

  auto attributes = item->GetAttributes()->GetAllItems();
  ASSERT_EQ(attributes.size(), 1);

  auto channel = dynamic_cast<AttributeItem*>(attributes.at(0));
  ASSERT_NE(channel, nullptr);
  EXPECT_EQ(channel->GetDisplayName(), domainconstants::kChannelAttribute);
}

TEST_F(VariableItemTest, ChannelAccessVariableFromDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("expected_channel");
  const std::string expected_datatype(R"RAW({"type":"int32"})RAW");

  auto ca_variable = CreateDomainVariable(domainconstants::kChannelAccessVariableType);
  ca_variable->AddAttribute(domainconstants::kNameAttribute, expected_name);
  ca_variable->AddAttribute(domainconstants::kChannelAttribute, expected_channel);
  ca_variable->AddAttribute(domainconstants::kTypeAttribute, expected_datatype);

  const workspace_t workspace;
  ca_variable->Setup(workspace);

  auto item = CreateVariableItem(CreateVariableNode(*ca_variable));

  EXPECT_EQ(item->GetName(), expected_name);
  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kChannelAttribute), expected_channel);
  EXPECT_FALSE(item->IsAvailable());

  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 0);
  EXPECT_EQ(GetAnyValue(*item), expected_anyvalue);
}

TEST_F(VariableItemTest, ChannelAccessVariableToDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("expected_channel");
  const std::string expected_datatype(R"RAW({"type":"uint32"})RAW");

  {  // when AnyValueItem is set
    auto item = CreateVariableItem(domainconstants::kChannelAccessVariableType);
    item->SetName(expected_name);
    item->SetAttribute(domainconstants::kChannelAttribute, expected_channel);

    const sup::dto::AnyValue anyvalue(sup::dto::UnsignedInteger32Type, 42);
    SetAnyValue(anyvalue, *item);

    auto domain_item = CreateDomainVariable(*item);
    EXPECT_EQ(domain_item->GetType(), domainconstants::kChannelAccessVariableType);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute),
              expected_channel);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_datatype);

    const workspace_t workspace;
    EXPECT_NO_THROW(domain_item->Setup(workspace));
  }
}

TEST_F(VariableItemTest, FileVariable)
{
  auto item = CreateVariableItem(domainconstants::kFileVariableType);
  EXPECT_FALSE(item->GetName().empty());
  EXPECT_TRUE(item->GetAttribute<std::string>(domainconstants::kFileNameAttribute).empty());
  EXPECT_FALSE(item->IsAvailable());

  item->SetName("abc");
  EXPECT_EQ(item->GetName(), std::string("abc"));

  item->SetAttribute(domainconstants::kFileNameAttribute, std::string("edf"));
  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kFileNameAttribute),
            std::string("edf"));
}

TEST_F(VariableItemTest, FileVariablePropertyAppearance)
{
  auto item = CreateVariableItem(domainconstants::kFileVariableType);
  auto children = item->GetAllItems();

  // connected flag, attribute container, AnyValueItem
  ASSERT_EQ(children.size(), 3);

  auto available_name = dynamic_cast<mvvm::PropertyItem*>(children.at(0));
  ASSERT_NE(available_name, nullptr);
  EXPECT_EQ(children.at(1), item->GetAttributes());

  auto attributes = item->GetAttributes()->GetAllItems();
  ASSERT_EQ(attributes.size(), 2);

  auto file_name = dynamic_cast<AttributeItem*>(attributes.at(0));
  ASSERT_NE(file_name, nullptr);
  EXPECT_EQ(file_name->GetDisplayName(), domainconstants::kFileNameAttribute);

  auto json_attr = dynamic_cast<AttributeItem*>(attributes.at(1));
  ASSERT_NE(json_attr, nullptr);
  EXPECT_EQ(json_attr->GetDisplayName(), domainconstants::kPrettyJsonAttribute);
}

TEST_F(VariableItemTest, FileVariableFromDomain)
{
  const std::string expected_name("abc");
  const std::string expected_file_name("edf");

  auto local_variable = CreateDomainVariable(domainconstants::kFileVariableType);
  local_variable->AddAttribute(domainconstants::kNameAttribute, expected_name);
  local_variable->AddAttribute(domainconstants::kFileNameAttribute, expected_file_name);

  const workspace_t workspace;
  local_variable->Setup(workspace);  // to propagate string attributes to AnyValues

  auto item = CreateVariableItem(CreateVariableNode(*local_variable));

  EXPECT_EQ(item->GetName(), expected_name);
  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kFileNameAttribute),
            expected_file_name);
}

TEST_F(VariableItemTest, FileVariableToDomain)
{
  const std::string expected_name("abc");
  const std::string expected_file_name("edf");

  auto item = CreateVariableItem(domainconstants::kFileVariableType);
  item->SetName(expected_name);
  item->SetAttribute(domainconstants::kFileNameAttribute, expected_file_name);

  auto domain_item = CreateDomainVariable(*item);
  EXPECT_EQ(domain_item->GetType(), domainconstants::kFileVariableType);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kFileNameAttribute),
            expected_file_name);

  const workspace_t workspace;
  EXPECT_NO_THROW(domain_item->Setup(workspace));
}

TEST_F(VariableItemTest, LocalVariable)
{
  auto item = CreateVariableItem(domainconstants::kLocalVariableType);
  EXPECT_FALSE(item->GetName().empty());
  ASSERT_NE(item->GetAnyValueItem(), nullptr);
  EXPECT_EQ(item->GetAnyValueItem()->GetType(), mvvm::GetTypeName<sup::gui::AnyValueEmptyItem>());
  EXPECT_FALSE(item->IsAvailable());

  item->SetName("abc");
  EXPECT_EQ(item->GetName(), std::string("abc"));

  EXPECT_NO_THROW(item->SetIsAvailable(true));
  EXPECT_TRUE(item->IsAvailable());
}

TEST_F(VariableItemTest, LocalVariablePropertyAppearance)
{
  auto item = CreateVariableItem(domainconstants::kLocalVariableType);
  auto children = item->GetAllItems();

  // connected flag, attribute container, AnyValueItem
  ASSERT_EQ(children.size(), 3);
  EXPECT_EQ(children.at(1), item->GetAttributes());

  auto attributes = item->GetAttributes()->GetAllItems();
  ASSERT_EQ(attributes.size(), 1);

  auto dynamic_type_item = dynamic_cast<AttributeItem*>(attributes.at(0));
  ASSERT_NE(dynamic_type_item, nullptr);
  EXPECT_EQ(dynamic_type_item->GetDisplayName(), domainconstants::kDynamicTypeAttribute);
  EXPECT_EQ(dynamic_type_item->GetState(), AttributeState::kNotSet);
}

TEST_F(VariableItemTest, LocalVariableFromDomain)
{
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  auto local_variable = CreateDomainVariable(domainconstants::kLocalVariableType);
  local_variable->AddAttribute(domainconstants::kNameAttribute, expected_name);
  local_variable->AddAttribute(domainconstants::kTypeAttribute, expected_type);
  local_variable->AddAttribute(domainconstants::kValueAttribute, expected_value);

  const workspace_t workspace;
  local_variable->Setup(workspace);

  auto item = CreateVariableItem(CreateVariableNode(*local_variable));

  EXPECT_EQ(item->GetName(), expected_name);

  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  EXPECT_EQ(GetAnyValue(*item), expected_anyvalue);
}

TEST_F(VariableItemTest, LocalVariableToDomain)
{
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  {  // case when AnyValueItem is set
    auto item = CreateVariableItem(domainconstants::kLocalVariableType);
    item->SetName(expected_name);

    const sup::dto::AnyValue anyvalue(sup::dto::UnsignedInteger32Type, 42);
    SetAnyValue(anyvalue, *item);

    auto domain_item = CreateDomainVariable(*item);
    EXPECT_EQ(domain_item->GetType(), domainconstants::kLocalVariableType);

    EXPECT_EQ(domain_item->GetStringAttributes().size(), 3);

    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_type);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kValueAttribute), expected_value);

    const workspace_t workspace;
    EXPECT_NO_THROW(domain_item->Setup(workspace));
  }

  {  // case when AnyValueItem is empty
    auto item = CreateVariableItem(domainconstants::kLocalVariableType);
    item->SetName(expected_name);
    EXPECT_EQ(item->GetAnyValueItem()->GetType(), mvvm::GetTypeName<sup::gui::AnyValueEmptyItem>());

    auto domain_item = CreateDomainVariable(*item);
    EXPECT_EQ(domain_item->GetType(), domainconstants::kLocalVariableType);

    EXPECT_EQ(domain_item->GetStringAttributes().size(), 1);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kTypeAttribute));
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kValueAttribute));

    workspace_t workspace;
    workspace.AddVariable(expected_name, std::move(domain_item));
    workspace.Setup();

    sup::dto::AnyValue anyvalue;
    EXPECT_FALSE(workspace.GetValue(expected_name, anyvalue));
  }
}

TEST_F(VariableItemTest, PvAccessClientVariable)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  auto item = CreateVariableItem(domainconstants::kPvAccessClientVariableType);
  EXPECT_FALSE(item->GetName().empty());
  EXPECT_TRUE(item->GetAttribute<std::string>(domainconstants::kChannelAttribute).empty());
  EXPECT_FALSE(item->IsAvailable());

  item->SetName("abc");
  EXPECT_EQ(item->GetName(), std::string("abc"));

  item->SetAttribute(domainconstants::kChannelAttribute, std::string("def"));
  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kChannelAttribute),
            std::string("def"));
}

TEST_F(VariableItemTest, PvAccessClientVariableFromDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("TEST-CHANNEL-PvAccessClientVariableFromDomain");
  const std::string expected_datatype(R"RAW({"type":"int32"})RAW");

  auto pv_variable = CreateDomainVariable(domainconstants::kPvAccessClientVariableType);
  pv_variable->AddAttribute(domainconstants::kNameAttribute, expected_name);
  pv_variable->AddAttribute(domainconstants::kChannelAttribute, expected_channel);
  pv_variable->AddAttribute(domainconstants::kTypeAttribute, expected_datatype);

  const workspace_t workspace;
  pv_variable->Setup(workspace);

  auto item = CreateVariableItem(CreateVariableNode(*pv_variable));

  EXPECT_EQ(item->GetName(), expected_name);
  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kChannelAttribute), expected_channel);

  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 0);
  EXPECT_EQ(GetAnyValue(*item), expected_anyvalue);
}

TEST_F(VariableItemTest, PvAccessClientVariableToDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("TEST-CHANNEL-PvAccessClientVariableToDomain");
  const std::string expected_datatype(R"RAW({"type":"uint32"})RAW");

  {  // case when AnyValueItem is set
    auto item = CreateVariableItem(domainconstants::kPvAccessClientVariableType);
    item->SetName(expected_name);
    item->SetAttribute(domainconstants::kChannelAttribute, expected_channel);

    const sup::dto::AnyValue anyvalue(sup::dto::UnsignedInteger32Type, 42);
    SetAnyValue(anyvalue, *item);

    auto domain_item = CreateDomainVariable(*item);
    EXPECT_EQ(domain_item->GetType(), domainconstants::kPvAccessClientVariableType);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute),
              expected_channel);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_datatype);
  }
}

TEST_F(VariableItemTest, PvAccessServerVariable)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  auto item = CreateVariableItem(domainconstants::kPvAccessServerVariableType);
  EXPECT_FALSE(item->GetName().empty());
  EXPECT_TRUE(item->GetAttribute<std::string>(domainconstants::kChannelAttribute).empty());
  EXPECT_FALSE(item->IsAvailable());

  item->SetName("abc");
  EXPECT_EQ(item->GetName(), std::string("abc"));

  item->SetAttribute(domainconstants::kChannelAttribute, std::string("def"));
  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kChannelAttribute),
            std::string("def"));
}

TEST_F(VariableItemTest, PvAccessServerVariableFromDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("TEST-CHANNEL-PvAccessServerVariableFromDomain");
  const std::string expected_datatype(R"RAW({"type":"int32"})RAW");
  const std::string expected_value("42");

  auto pvxs_variable = CreateDomainVariable(domainconstants::kPvAccessServerVariableType);
  pvxs_variable->AddAttribute(domainconstants::kNameAttribute, expected_name);
  pvxs_variable->AddAttribute(domainconstants::kChannelAttribute, expected_channel);
  pvxs_variable->AddAttribute(domainconstants::kTypeAttribute, expected_datatype);
  pvxs_variable->AddAttribute(domainconstants::kValueAttribute, expected_value);

  const workspace_t workspace;
  pvxs_variable->Setup(workspace);

  auto item = CreateVariableItem(CreateVariableNode(*pvxs_variable));

  EXPECT_EQ(item->GetName(), expected_name);
  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kChannelAttribute), expected_channel);
  EXPECT_FALSE(item->IsAvailable());

  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  EXPECT_EQ(GetAnyValue(*item), expected_anyvalue);
}

TEST_F(VariableItemTest, PvAccessServerVariableToDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("TEST-CHANNEL-PvAccessServerVariableToDomain");
  const std::string expected_datatype(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  {  // case when AnyValueItem is set
    auto item = CreateVariableItem(domainconstants::kPvAccessServerVariableType);
    item->SetAttribute(domainconstants::kChannelAttribute, expected_channel);
    item->SetName(expected_name);

    const sup::dto::AnyValue anyvalue(sup::dto::UnsignedInteger32Type, 42);
    SetAnyValue(anyvalue, *item);

    auto domain_item = CreateDomainVariable(*item);
    EXPECT_EQ(domain_item->GetType(), domainconstants::kPvAccessServerVariableType);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute),
              expected_channel);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_datatype);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kValueAttribute), expected_value);

    const workspace_t workspace;
    EXPECT_NO_THROW(domain_item->Setup(workspace));
  }
}

//! SetAttribute and GetAttribute members of VariableItem.
TEST_F(VariableItemTest, SetAndGetAttribute)
{
  auto item = CreateVariableItem(domainconstants::kFileVariableType);

  // nullable attribute is not set at the beginning
  EXPECT_THROW(item->GetAttribute<bool>(domainconstants::kPrettyJsonAttribute), RuntimeException);

  item->SetAttribute(domainconstants::kFileNameAttribute, std::string("abc"));
  EXPECT_EQ(item->GetAttribute<std::string>(domainconstants::kFileNameAttribute), "abc");

  item->SetAttribute(domainconstants::kPrettyJsonAttribute, true);
  EXPECT_TRUE(item->GetAttribute<bool>(domainconstants::kPrettyJsonAttribute));

  // value type must match the attribute type
  EXPECT_THROW(item->SetAttribute(domainconstants::kPrettyJsonAttribute, 1), RuntimeException);

  // unknown attribute
  EXPECT_THROW(item->SetAttribute("unknown", true), RuntimeException);

  // unset: nullable attribute only
  item->UnsetAttribute(domainconstants::kPrettyJsonAttribute);
  EXPECT_THROW(item->GetAttribute<bool>(domainconstants::kPrettyJsonAttribute), RuntimeException);
  EXPECT_THROW(item->UnsetAttribute(domainconstants::kFileNameAttribute), RuntimeException);
}

//! Test helper InsertVariable.
TEST_F(VariableItemTest, InsertVariable)
{
  mvvm::ApplicationModel model;

  auto* item = InsertVariable(domainconstants::kLocalVariableType, model.GetRootItem());
  ASSERT_NE(item, nullptr);
  EXPECT_EQ(item->GetDomainType(), domainconstants::kLocalVariableType);
  EXPECT_EQ(item->GetModel(), &model);
  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 1);
}

}  // namespace oac_tree_gui::test
