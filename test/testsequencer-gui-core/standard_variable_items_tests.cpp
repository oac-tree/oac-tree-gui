/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/standard_variable_items.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/transform/attribute_item_transform_helper.h>
#include <sequencergui/transform/transform_from_domain.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/property_item.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

using namespace sequencergui;

//! Tests for items in standard_variable_items.h

class StandardVariableItemsTest : public ::testing::Test
{
};

//! ---------------------------------------------------------------------------
//! ChannelAccessItem
//! ---------------------------------------------------------------------------

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

//! Testing appearance of properties.

TEST_F(StandardVariableItemsTest, ChannelAccessVariableItemPropertyAppearance)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  ChannelAccessVariableItem item;
  auto children = item.GetAllItems();

  ASSERT_EQ(children.size(), 2);

  auto channel = dynamic_cast<sup::gui::AnyValueScalarItem*>(children.at(0));
  ASSERT_NE(channel, nullptr);
  EXPECT_EQ(channel->GetDisplayName(), domainconstants::kChannelAttribute);

  auto available = dynamic_cast<mvvm::PropertyItem*>(children.at(1));
  ASSERT_NE(available, nullptr);
  EXPECT_EQ(available->GetDisplayName(), itemconstants::kIsAvailable);
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

  workspace_t ws;
  ca_variable->Setup(ws);

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

    sup::dto::AnyValue anyvalue(sup::dto::UnsignedInteger32Type, 42);
    SetAnyValue(anyvalue, item);

    auto domain_item = item.CreateDomainVariable();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kChannelAccessVariableType);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute),
              expected_channel);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_datatype);

    workspace_t ws;
    EXPECT_NO_THROW(domain_item->Setup(ws));
  }
}

//! ---------------------------------------------------------------------------
//! FileVariableItem
//! ---------------------------------------------------------------------------

TEST_F(StandardVariableItemsTest, FileVariableItem)
{
  FileVariableItem item;
  EXPECT_FALSE(item.GetName().empty());
  EXPECT_TRUE(item.GetFileName().empty());
  EXPECT_TRUE(item.IsAvailable());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  item.SetFileName("edf");
  EXPECT_EQ(item.GetFileName(), std::string("edf"));
}

//! Testing appearance of properties.

TEST_F(StandardVariableItemsTest, FileVariableItemPropertyAppearance)
{
  FileVariableItem item;
  auto children = item.GetAllItems();

  ASSERT_EQ(children.size(), 2);

  auto file_name = dynamic_cast<sup::gui::AnyValueScalarItem*>(children.at(0));
  ASSERT_NE(file_name, nullptr);
  EXPECT_EQ(file_name->GetDisplayName(), domainconstants::kFileNameAttribute);

  auto json_attr = dynamic_cast<sup::gui::AnyValueScalarItem*>(children.at(1));
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

  workspace_t ws;
  local_variable->Setup(ws);  // to propagate string attributes to AnyValues

  sequencergui::FileVariableItem item;
  item.InitFromDomain(local_variable.get());

  EXPECT_EQ(item.GetName(), expected_name);
  EXPECT_EQ(item.GetFileName(), expected_file_name);
}

TEST_F(StandardVariableItemsTest, FileVariableItemToDomain)
{
  const std::string expected_name("abc");
  const std::string expected_file_name("edf");

  sequencergui::FileVariableItem item;
  item.SetName(expected_name);
  item.SetFileName(expected_file_name);

  auto domain_item = item.CreateDomainVariable();
  EXPECT_EQ(domain_item->GetType(), domainconstants::kFileVariableType);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
  EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kFileNameAttribute),
            expected_file_name);

  workspace_t ws;
  EXPECT_NO_THROW(domain_item->Setup(ws));
}

//! ---------------------------------------------------------------------------
//! LocalVariableItem
//! ---------------------------------------------------------------------------

TEST_F(StandardVariableItemsTest, LocalVariableItem)
{
  sequencergui::LocalVariableItem item;
  EXPECT_FALSE(item.GetName().empty());
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);
  EXPECT_TRUE(item.IsAvailable());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  auto anyvalue_item = item.InsertItem<sup::gui::AnyValueScalarItem>({});
  EXPECT_EQ(item.GetAnyValueItem(), anyvalue_item);

  EXPECT_NO_THROW(item.SetIsAvailable(true));
}

//! Testing appearance of properties.

TEST_F(StandardVariableItemsTest, LocalVariableItemPropertyAppearance)
{
  LocalVariableItem item;
  auto children = item.GetAllItems();

  ASSERT_EQ(children.size(), 1);

  auto dynamic_type_item = dynamic_cast<sup::gui::AnyValueScalarItem*>(children.at(0));
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

  workspace_t ws;
  local_variable->Setup(ws);

  sequencergui::LocalVariableItem item;
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
  const std::string expected_dynamic_flag("false");

  {  // case when AnyValueItem is set
    sequencergui::LocalVariableItem item;
    item.SetName(expected_name);

    sup::dto::AnyValue anyvalue(sup::dto::UnsignedInteger32Type, 42);
    SetAnyValue(anyvalue, item);

    auto domain_item = item.CreateDomainVariable();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kLocalVariableType);

    EXPECT_EQ(domain_item->GetStringAttributes().size(), 3);

    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_type);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kValueAttribute), expected_value);

    workspace_t ws;
    EXPECT_NO_THROW(domain_item->Setup(ws));
  }

  {  // case when AnyValueItem is not set
    sequencergui::LocalVariableItem item;
    item.SetName(expected_name);
    EXPECT_EQ(item.GetAnyValueItem(), nullptr);

    auto domain_item = item.CreateDomainVariable();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kLocalVariableType);

    EXPECT_EQ(domain_item->GetStringAttributes().size(), 1);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kTypeAttribute));
    EXPECT_FALSE(domain_item->HasAttribute(domainconstants::kValueAttribute));

    workspace_t ws;
    ws.AddVariable(expected_name, std::move(domain_item));
    ws.Setup();

    sup::dto::AnyValue anyvalue;
    EXPECT_FALSE(ws.GetValue(expected_name, anyvalue));
  }
}

//! ---------------------------------------------------------------------------
//! PVClientVariableItem
//! ---------------------------------------------------------------------------

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

  workspace_t ws;
  pv_variable->Setup(ws);

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

    sup::dto::AnyValue anyvalue(sup::dto::UnsignedInteger32Type, 42);
    SetAnyValue(anyvalue, item);

    auto domain_item = item.CreateDomainVariable();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kPvAccessClientVariableType);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kChannelAttribute),
              expected_channel);
    EXPECT_EQ(domain_item->GetAttributeString(domainconstants::kTypeAttribute), expected_datatype);
  }
}

//! ---------------------------------------------------------------------------
//! PVServerVariableItem
//! ---------------------------------------------------------------------------

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

  workspace_t ws;
  pvxs_variable->Setup(ws);

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

    workspace_t ws;
    EXPECT_NO_THROW(domain_item->Setup(ws));
  }
}
