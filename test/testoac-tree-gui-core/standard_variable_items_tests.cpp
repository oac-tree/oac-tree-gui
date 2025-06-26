/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "oac_tree_gui/model/standard_variable_items.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>
#include <oac_tree_gui/transform/transform_from_domain.h>

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/property_item.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for items in standard_variable_items.h
 */
class StandardVariableItemsTest : public ::testing::Test
{
};

TEST_F(StandardVariableItemsTest, ChannelAccessVariableItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  ChannelAccessVariableItem item;
  EXPECT_FALSE(item.GetName().empty());
  EXPECT_TRUE(item.GetChannel().empty());
  EXPECT_FALSE(item.IsAvailable());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  item.SetChannel("def");
  EXPECT_EQ(item.GetChannel(), std::string("def"));

  item.SetIsAvailable(true);
  EXPECT_TRUE(item.IsAvailable());
}

TEST_F(StandardVariableItemsTest, ChannelAccessVariableItemPropertyAppearance)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const ChannelAccessVariableItem item;
  auto children = item.GetAllItems();

  ASSERT_EQ(children.size(), 2);

  auto available = dynamic_cast<mvvm::PropertyItem*>(children.at(0));
  ASSERT_NE(available, nullptr);
  EXPECT_EQ(available->GetDisplayName(), itemconstants::kIsAvailable);

  auto channel = dynamic_cast<sup::gui::AnyValueScalarItem*>(children.at(1));
  ASSERT_NE(channel, nullptr);
  EXPECT_EQ(channel->GetDisplayName(), domainconstants::kChannelAttribute);
}

TEST_F(StandardVariableItemsTest, ChannelAccessVariableFromDomain)
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

  ChannelAccessVariableItem item;
  item.InitFromDomain(ca_variable.get());

  EXPECT_EQ(item.GetName(), expected_name);
  EXPECT_EQ(item.GetChannel(), expected_channel);
  EXPECT_FALSE(item.IsAvailable());

  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 0);
  EXPECT_EQ(GetAnyValue(item), expected_anyvalue);
}

TEST_F(StandardVariableItemsTest, ChannelAccessVariableToDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("expected_channel");
  const std::string expected_datatype(R"RAW({"type":"uint32"})RAW");

  {  // when AnyValueItem is set
    ChannelAccessVariableItem item;
    item.SetName(expected_name);
    item.SetChannel(expected_channel);

    const sup::dto::AnyValue anyvalue(sup::dto::UnsignedInteger32Type, 42);
    SetAnyValue(anyvalue, item);

    auto domain_item = item.CreateDomainVariable();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kChannelAccessVariableType);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute),
              expected_channel);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_datatype);

    const workspace_t workspace;
    EXPECT_NO_THROW(domain_item->Setup(workspace));
  }
}

TEST_F(StandardVariableItemsTest, FileVariableItem)
{
  FileVariableItem item;
  EXPECT_FALSE(item.GetName().empty());
  EXPECT_TRUE(item.GetFileName().empty());
  EXPECT_FALSE(item.IsAvailable());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  item.SetFileName("edf");
  EXPECT_EQ(item.GetFileName(), std::string("edf"));
}

TEST_F(StandardVariableItemsTest, FileVariableItemPropertyAppearance)
{
  const FileVariableItem item;
  auto children = item.GetAllItems();

  ASSERT_EQ(children.size(), 3);

  auto available_name = dynamic_cast<mvvm::PropertyItem*>(children.at(0));
  ASSERT_NE(available_name, nullptr);

  auto file_name = dynamic_cast<sup::gui::AnyValueScalarItem*>(children.at(1));
  ASSERT_NE(file_name, nullptr);
  EXPECT_EQ(file_name->GetDisplayName(), domainconstants::kFileNameAttribute);

  auto json_attr = dynamic_cast<sup::gui::AnyValueScalarItem*>(children.at(2));
  ASSERT_NE(json_attr, nullptr);
  EXPECT_EQ(json_attr->GetDisplayName(), domainconstants::kPrettyJsonAttribute);
}

TEST_F(StandardVariableItemsTest, FileVariableItemFromDomain)
{
  const std::string expected_name("abc");
  const std::string expected_file_name("edf");

  auto local_variable = CreateDomainVariable(domainconstants::kFileVariableType);
  local_variable->AddAttribute(domainconstants::kNameAttribute, expected_name);
  local_variable->AddAttribute(domainconstants::kFileNameAttribute, expected_file_name);

  const workspace_t workspace;
  local_variable->Setup(workspace);  // to propagate string attributes to AnyValues

  oac_tree_gui::FileVariableItem item;
  item.InitFromDomain(local_variable.get());

  EXPECT_EQ(item.GetName(), expected_name);
  EXPECT_EQ(item.GetFileName(), expected_file_name);
}

TEST_F(StandardVariableItemsTest, FileVariableItemToDomain)
{
  const std::string expected_name("abc");
  const std::string expected_file_name("edf");

  oac_tree_gui::FileVariableItem item;
  item.SetName(expected_name);
  item.SetFileName(expected_file_name);

  auto domain_item = item.CreateDomainVariable();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kFileVariableType);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kFileNameAttribute),
            expected_file_name);

  const workspace_t workspace;
  EXPECT_NO_THROW(domain_item->Setup(workspace));
}

TEST_F(StandardVariableItemsTest, LocalVariableItem)
{
  oac_tree_gui::LocalVariableItem item;
  EXPECT_FALSE(item.GetName().empty());
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);
  EXPECT_FALSE(item.IsAvailable());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  auto anyvalue_item = item.InsertItem<sup::gui::AnyValueScalarItem>({});
  EXPECT_EQ(item.GetAnyValueItem(), anyvalue_item);

  EXPECT_NO_THROW(item.SetIsAvailable(true));
  EXPECT_TRUE(item.IsAvailable());
}

TEST_F(StandardVariableItemsTest, LocalVariableItemPropertyAppearance)
{
  const LocalVariableItem item;
  auto children = item.GetAllItems();

  ASSERT_EQ(children.size(), 2);

  auto dynamic_type_item = dynamic_cast<sup::gui::AnyValueScalarItem*>(children.at(1));
  ASSERT_NE(dynamic_type_item, nullptr);
  EXPECT_EQ(dynamic_type_item->GetDisplayName(), domainconstants::kDynamicTypeAttribute);
  EXPECT_FALSE(GetAttributeExposedFlag(*dynamic_type_item));
}

TEST_F(StandardVariableItemsTest, LocalVariableItemFromDomain)
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

  oac_tree_gui::LocalVariableItem item;
  item.InitFromDomain(local_variable.get());

  EXPECT_EQ(item.GetName(), expected_name);

  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  EXPECT_EQ(GetAnyValue(item), expected_anyvalue);
}

TEST_F(StandardVariableItemsTest, LocalVariableItemToDomain)
{
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  {  // case when AnyValueItem is set
    oac_tree_gui::LocalVariableItem item;
    item.SetName(expected_name);

    const sup::dto::AnyValue anyvalue(sup::dto::UnsignedInteger32Type, 42);
    SetAnyValue(anyvalue, item);

    auto domain_item = item.CreateDomainVariable();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kLocalVariableType);

    EXPECT_EQ(domain_item->GetStringAttributes().size(), 3);

    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_type);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kValueAttribute), expected_value);

    const workspace_t workspace;
    EXPECT_NO_THROW(domain_item->Setup(workspace));
  }

  {  // case when AnyValueItem is not set
    oac_tree_gui::LocalVariableItem item;
    item.SetName(expected_name);
    EXPECT_EQ(item.GetAnyValueItem(), nullptr);

    auto domain_item = item.CreateDomainVariable();
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

TEST_F(StandardVariableItemsTest, PvAccessClientVariableItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  PvAccessClientVariableItem item;
  EXPECT_FALSE(item.GetName().empty());
  EXPECT_TRUE(item.GetChannel().empty());
  EXPECT_FALSE(item.IsAvailable());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  item.SetChannel("def");
  EXPECT_EQ(item.GetChannel(), std::string("def"));
}

TEST_F(StandardVariableItemsTest, PvAccessClientVariableItemFromDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("TEST-CHANNEL-PvAccessClientVariableItemFromDomain");
  const std::string expected_datatype(R"RAW({"type":"int32"})RAW");

  auto pv_variable = CreateDomainVariable(domainconstants::kPvAccessClientVariableType);
  pv_variable->AddAttribute(domainconstants::kNameAttribute, expected_name);
  pv_variable->AddAttribute(domainconstants::kChannelAttribute, expected_channel);
  pv_variable->AddAttribute(domainconstants::kTypeAttribute, expected_datatype);

  const workspace_t workspace;
  pv_variable->Setup(workspace);

  PvAccessClientVariableItem item;
  item.InitFromDomain(pv_variable.get());

  EXPECT_EQ(item.GetName(), expected_name);
  EXPECT_EQ(item.GetChannel(), expected_channel);

  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 0);
  EXPECT_EQ(GetAnyValue(item), expected_anyvalue);
}

TEST_F(StandardVariableItemsTest, PvAccessClientVariableItemToDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("TEST-CHANNEL-PvAccessClientVariableItemToDomain");
  const std::string expected_datatype(R"RAW({"type":"uint32"})RAW");

  {  // case when AnyValueItem is set
    PvAccessClientVariableItem item;
    item.SetName(expected_name);
    item.SetChannel(expected_channel);

    const sup::dto::AnyValue anyvalue(sup::dto::UnsignedInteger32Type, 42);
    SetAnyValue(anyvalue, item);

    auto domain_item = item.CreateDomainVariable();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kPvAccessClientVariableType);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute),
              expected_channel);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_datatype);
  }
}

TEST_F(StandardVariableItemsTest, PvAccessServerVariableItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  PvAccessServerVariableItem item;
  EXPECT_FALSE(item.GetName().empty());
  EXPECT_TRUE(item.GetChannel().empty());
  EXPECT_FALSE(item.IsAvailable());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  item.SetChannel("def");
  EXPECT_EQ(item.GetChannel(), std::string("def"));
}

TEST_F(StandardVariableItemsTest, PvAccessServerVariableItemFromDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("TEST-CHANNEL-PvAccessServerVariableItemFromDomain");
  const std::string expected_datatype(R"RAW({"type":"int32"})RAW");
  const std::string expected_value("42");

  auto pvxs_variable = CreateDomainVariable(domainconstants::kPvAccessServerVariableType);
  pvxs_variable->AddAttribute(domainconstants::kNameAttribute, expected_name);
  pvxs_variable->AddAttribute(domainconstants::kChannelAttribute, expected_channel);
  pvxs_variable->AddAttribute(domainconstants::kTypeAttribute, expected_datatype);
  pvxs_variable->AddAttribute(domainconstants::kValueAttribute, expected_value);

  const workspace_t workspace;
  pvxs_variable->Setup(workspace);

  PvAccessServerVariableItem item;
  item.InitFromDomain(pvxs_variable.get());

  EXPECT_EQ(item.GetName(), expected_name);
  EXPECT_EQ(item.GetChannel(), expected_channel);
  EXPECT_FALSE(item.IsAvailable());

  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  EXPECT_EQ(GetAnyValue(item), expected_anyvalue);
}

TEST_F(StandardVariableItemsTest, PvAccessServerVariableItemToDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("TEST-CHANNEL-PvAccessServerVariableItemToDomain");
  const std::string expected_datatype(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  {  // case when AnyValueItem is set
    PvAccessServerVariableItem item;
    item.SetChannel(expected_channel);
    item.SetName(expected_name);

    const sup::dto::AnyValue anyvalue(sup::dto::UnsignedInteger32Type, 42);
    SetAnyValue(anyvalue, item);

    auto domain_item = item.CreateDomainVariable();
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

}  // namespace oac_tree_gui::test
