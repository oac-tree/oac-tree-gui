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

#include "sequencergui/model/standard_variable_items.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/transform/transform_from_domain.h>
#include <sequencergui/transform/variable_transform_helper.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/property_item.h>

#include <sup/sequencer/attribute_map.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/variable.h>

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
  ChannelAccessVariableItem item;
  EXPECT_TRUE(item.GetName().empty());
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
  ChannelAccessVariableItem item;
  auto children = item.GetAllItems();

  ASSERT_EQ(children.size(), 3);
  auto name = dynamic_cast<mvvm::PropertyItem*>(children.at(0));
  ASSERT_NE(name, nullptr);
  EXPECT_EQ(name->GetDisplayName(), std::string("name"));

  auto channel = dynamic_cast<mvvm::PropertyItem*>(children.at(1));
  ASSERT_NE(channel, nullptr);
  EXPECT_EQ(channel->GetDisplayName(), std::string("channel"));

  auto available = dynamic_cast<mvvm::PropertyItem*>(children.at(2));
  ASSERT_NE(available, nullptr);
  EXPECT_EQ(available->GetDisplayName(), std::string("connected"));
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

  ChannelAccessVariableItem item;
  item.InitFromDomain(ca_variable.get());

  EXPECT_EQ(item.GetName(), expected_name);
  EXPECT_EQ(item.GetChannel(), expected_channel);
  EXPECT_FALSE(item.IsAvailable());

  ASSERT_NE(item.GetAnyValueItem(), nullptr);
  auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
  const sup::dto::AnyValue expected_anyvalue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0});
  EXPECT_EQ(stored_anyvalue, expected_anyvalue);
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

  {  // case with empty attributes
    ChannelAccessVariableItem item;
    item.SetName(expected_name);
    item.SetChannel(expected_channel);

    EXPECT_THROW(item.CreateDomainVariable(), LogicErrorException);
  }

  {  // when AnyValueItem is set
    ChannelAccessVariableItem item;
    item.SetName(expected_name);
    item.SetChannel(expected_channel);

    sup::dto::AnyValue anyvalue(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 42});
    SetAnyValue(anyvalue, item);

    auto domain_item = item.CreateDomainVariable();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kChannelAccessVariableType);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kChannelAttribute), expected_channel);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kTypeAttribute), expected_datatype);

    EXPECT_NO_THROW(domain_item->Setup());
  }
}

//! ---------------------------------------------------------------------------
//! FileVariableItem
//! ---------------------------------------------------------------------------

TEST_F(StandardVariableItemsTest, FileVariableItem)
{
  FileVariableItem item;
  EXPECT_TRUE(item.GetName().empty());
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
  auto name = dynamic_cast<mvvm::PropertyItem*>(children.at(0));
  ASSERT_NE(name, nullptr);
  EXPECT_EQ(name->GetDisplayName(), std::string("name"));

  auto file_name = dynamic_cast<mvvm::PropertyItem*>(children.at(1));
  ASSERT_NE(file_name, nullptr);
  EXPECT_EQ(file_name->GetDisplayName(), std::string("File name"));
}

TEST_F(StandardVariableItemsTest, FileVariableItemFromDomain)
{
  const std::string expected_name("abc");
  const std::string expected_file_name("edf");

  auto local_variable = CreateDomainVariable(domainconstants::kFileVariableType);
  local_variable->AddAttribute(domainconstants::kNameAttribute, expected_name);
  local_variable->AddAttribute(domainconstants::kFileAttribute, expected_file_name);

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
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kNameAttribute), expected_name);
  EXPECT_EQ(domain_item->GetAttribute(domainconstants::kFileAttribute), expected_file_name);

  EXPECT_NO_THROW(domain_item->Setup());
}

//! ---------------------------------------------------------------------------
//! LocalVariableItem
//! ---------------------------------------------------------------------------

TEST_F(StandardVariableItemsTest, LocalVariableItem)
{
  sequencergui::LocalVariableItem item;
  EXPECT_TRUE(item.GetName().empty());
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
  auto name = dynamic_cast<mvvm::PropertyItem*>(children.at(0));
  ASSERT_NE(name, nullptr);
  EXPECT_EQ(name->GetDisplayName(), std::string("name"));
}

TEST_F(StandardVariableItemsTest, LocalVariableItemFromDomain)
{
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  auto local_variable = CreateDomainVariable(domainconstants::kLocalVariableType);
  local_variable->AddAttribute("name", expected_name);
  local_variable->AddAttribute("type", expected_type);
  local_variable->AddAttribute("value", expected_value);

  sequencergui::LocalVariableItem item;
  item.InitFromDomain(local_variable.get());

  EXPECT_EQ(item.GetName(), expected_name);

  ASSERT_NE(item.GetAnyValueItem(), nullptr);
  auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
  const sup::dto::AnyValue expected_anyvalue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  EXPECT_EQ(stored_anyvalue, expected_anyvalue);
}

TEST_F(StandardVariableItemsTest, LocalVariableItemToDomain)
{
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  {  // case with empty attributes
    ChannelAccessVariableItem item;
    item.SetName(expected_name);

    EXPECT_THROW(item.CreateDomainVariable(), LogicErrorException);
  }

  {  // case when AnyValueItem is set
    sequencergui::LocalVariableItem item;
    item.SetName(expected_name);

    sup::dto::AnyValue anyvalue(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 42});
    SetAnyValue(anyvalue, item);

    auto domain_item = item.CreateDomainVariable();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kLocalVariableType);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kTypeAttribute), expected_type);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kValueAttribute), expected_value);
    EXPECT_EQ(domain_item->GetAttributes().GetAttributeNames().size(), 3);
  }
}

//! ---------------------------------------------------------------------------
//! PVClientVariableItem
//! ---------------------------------------------------------------------------

TEST_F(StandardVariableItemsTest, PVClientVariableItem)
{
  PVClientVariableItem item;
  EXPECT_TRUE(item.GetName().empty());
  EXPECT_TRUE(item.GetChannel().empty());
  EXPECT_FALSE(item.IsAvailable());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  item.SetChannel("def");
  EXPECT_EQ(item.GetChannel(), std::string("def"));
}

TEST_F(StandardVariableItemsTest, PVClientVariableItemFromDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("expected_channel");
  const std::string expected_datatype(R"RAW({"type":"int32"})RAW");

  auto pv_variable = CreateDomainVariable(domainconstants::kPVClientVariableType);
  pv_variable->AddAttribute(domainconstants::kNameAttribute, expected_name);
  pv_variable->AddAttribute(domainconstants::kChannelAttribute, expected_channel);
  pv_variable->AddAttribute(domainconstants::kTypeAttribute, expected_datatype);

  PVClientVariableItem item;
  item.InitFromDomain(pv_variable.get());

  EXPECT_EQ(item.GetName(), expected_name);
  EXPECT_EQ(item.GetChannel(), expected_channel);

  ASSERT_NE(item.GetAnyValueItem(), nullptr);
  auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
  const sup::dto::AnyValue expected_anyvalue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0});
  EXPECT_EQ(stored_anyvalue, expected_anyvalue);
}

TEST_F(StandardVariableItemsTest, PVClientVariableItemToDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("expected_channel");
  const std::string expected_datatype(R"RAW({"type":"uint32"})RAW");

  {  // case with empty attributes
    PVClientVariableItem item;
    item.SetName(expected_name);

    EXPECT_THROW(item.CreateDomainVariable(), LogicErrorException);
  }

  {  // case when AnyValueItem is set
    PVClientVariableItem item;
    item.SetName(expected_name);
    item.SetChannel(expected_channel);

    sup::dto::AnyValue anyvalue(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 42});
    SetAnyValue(anyvalue, item);

    auto domain_item = item.CreateDomainVariable();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kPVClientVariableType);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kChannelAttribute), expected_channel);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kTypeAttribute), expected_datatype);
  }
}

//! ---------------------------------------------------------------------------
//! PVServerVariableItem
//! ---------------------------------------------------------------------------

TEST_F(StandardVariableItemsTest, PVServerVariableItem)
{
  PVServerVariableItem item;
  EXPECT_TRUE(item.GetName().empty());
  EXPECT_TRUE(item.GetChannel().empty());
  EXPECT_FALSE(item.IsAvailable());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  item.SetChannel("def");
  EXPECT_EQ(item.GetChannel(), std::string("def"));
}

TEST_F(StandardVariableItemsTest, PVServerVariableItemFromDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("expected_channel");
  const std::string expected_datatype(R"RAW({"type":"int32"})RAW");
  const std::string expected_value("42");

  auto pvxs_variable = CreateDomainVariable(domainconstants::kPVServerVariableType);
  pvxs_variable->AddAttribute(domainconstants::kNameAttribute, expected_name);
  pvxs_variable->AddAttribute(domainconstants::kChannelAttribute, expected_channel);
  pvxs_variable->AddAttribute(domainconstants::kTypeAttribute, expected_datatype);
  pvxs_variable->AddAttribute(domainconstants::kValueAttribute, expected_value);

  PVServerVariableItem item;
  item.InitFromDomain(pvxs_variable.get());

  EXPECT_EQ(item.GetName(), expected_name);
  EXPECT_EQ(item.GetChannel(), expected_channel);
  EXPECT_FALSE(item.IsAvailable());

  ASSERT_NE(item.GetAnyValueItem(), nullptr);
  auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
  const sup::dto::AnyValue expected_anyvalue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  EXPECT_EQ(stored_anyvalue, expected_anyvalue);
}

TEST_F(StandardVariableItemsTest, PVServerVariableItemToDomain)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string expected_name("expected_name");
  const std::string expected_channel("expected_channel");
  const std::string expected_datatype(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  {  // case with empty attributes
    PVServerVariableItem item;
    item.SetName(expected_name);

    EXPECT_THROW(item.CreateDomainVariable(), LogicErrorException);
  }

  {  // case when AnyValueItem is set
    PVServerVariableItem item;
    item.SetChannel(expected_channel);
    item.SetName(expected_name);

    sup::dto::AnyValue anyvalue(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 42});
    SetAnyValue(anyvalue, item);

    auto domain_item = item.CreateDomainVariable();
    EXPECT_EQ(domain_item->GetType(), domainconstants::kPVServerVariableType);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kChannelAttribute), expected_channel);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kTypeAttribute), expected_datatype);
    EXPECT_EQ(domain_item->GetAttribute(domainconstants::kValueAttribute), expected_value);

    EXPECT_NO_THROW(domain_item->Setup());
  }
}

//! UnknownVariableItem tests.
//! Here we pretend that LocalVariableItem is unknown for a GUI, and check how UnknownVariableItem
//! behaves in the way from/to domain.

TEST_F(StandardVariableItemsTest, UnknownVariableItemFromLocalVariable)
{
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  auto domain_variable = CreateDomainVariable(domainconstants::kLocalVariableType);
  domain_variable->AddAttribute(domainconstants::kNameAttribute, expected_name);
  domain_variable->AddAttribute(domainconstants::kTypeAttribute, expected_type);
  domain_variable->AddAttribute(domainconstants::kValueAttribute, expected_value);

  // from domain
  UnknownVariableItem item;
  item.InitFromDomain(domain_variable.get());
  EXPECT_EQ(item.Property<std::string>(domainconstants::kNameAttribute), expected_name);
  EXPECT_EQ(item.Property<std::string>(domainconstants::kTypeAttribute), expected_type);
  EXPECT_EQ(item.Property<std::string>(domainconstants::kValueAttribute), expected_value);

  // to domain
  auto new_domain_item = item.CreateDomainVariable();
  EXPECT_EQ(new_domain_item->GetType(), domainconstants::kLocalVariableType);
  EXPECT_EQ(new_domain_item->GetAttribute(domainconstants::kNameAttribute), expected_name);
  EXPECT_EQ(new_domain_item->GetAttribute(domainconstants::kTypeAttribute), expected_type);
  EXPECT_EQ(new_domain_item->GetAttribute(domainconstants::kValueAttribute), expected_value);

  EXPECT_NO_THROW(new_domain_item->Setup());
}
