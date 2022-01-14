/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/standardvariableitems.h"

#include "AttributeMap.h"
#include "Variable.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/standardvariableitems.h"
#include "sequencergui/model/transformfromdomain.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for items in standardvariableitems.h

class StandardVariableItemsTest : public ::testing::Test
{
};

//! ChannelAccessItem

TEST_F(StandardVariableItemsTest, ChannelAccessVariableItem)
{
  ChannelAccessVariableItem item;
  EXPECT_TRUE(item.GetName().empty());
  EXPECT_TRUE(item.GetChannel().empty());
  EXPECT_TRUE(item.GetDataType().empty());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  item.SetChannel("def");
  EXPECT_EQ(item.GetChannel(), std::string("def"));

  item.SetDataType("jkl");
  EXPECT_EQ(item.GetDataType(), std::string("jkl"));
}

TEST_F(StandardVariableItemsTest, ChannelAccessVariableFromDomain)
{
  const std::string expected_name("expected_name");
  const std::string expected_channel("expected_channel");
  const std::string expected_datatype("expected_datatype");

  if (DomainUtils::IsChannelAccessAvailable())
  {
    auto ca_variable =
        DomainUtils::CreateDomainVariable(DomainConstants::kChannelAccessVariableType);
    ca_variable->AddAttribute(DomainConstants::kNameAttribute, expected_name);
    ca_variable->AddAttribute(DomainConstants::kChannelAttribute, expected_channel);
    ca_variable->AddAttribute(DomainConstants::kDataTypeAttribute, expected_datatype);

    ChannelAccessVariableItem ca_variable_item;
    ca_variable_item.InitFromDomain(ca_variable.get());

    EXPECT_EQ(ca_variable_item.GetName(), expected_name);
    EXPECT_EQ(ca_variable_item.GetChannel(), expected_channel);
    EXPECT_EQ(ca_variable_item.GetDataType(), expected_datatype);
  }
}

TEST_F(StandardVariableItemsTest, ChannelAccessVariableToDomain)
{
  const std::string expected_name("expected_name");
  const std::string expected_channel("expected_channel");
  const std::string expected_datatype("expected_datatype");

  if (DomainUtils::IsChannelAccessAvailable())
  {
    ChannelAccessVariableItem item;
    item.SetName(expected_name);
    item.SetChannel(expected_channel);
    item.SetDataType(expected_datatype);

    auto domain_item = item.CreateDomainVariable();
    EXPECT_EQ(domain_item->GetType(), DomainConstants::kChannelAccessVariableType);
    EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kChannelAttribute), expected_channel);
    EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kDataTypeAttribute), expected_datatype);
  }
}

//! FileVariableItem

TEST_F(StandardVariableItemsTest, FileVariableItem)
{
  FileVariableItem item;
  EXPECT_TRUE(item.GetName().empty());
  EXPECT_TRUE(item.GetFileName().empty());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  item.SetFileName("edf");
  EXPECT_EQ(item.GetFileName(), std::string("edf"));
}

TEST_F(StandardVariableItemsTest, FileVariableItemFromDomain)
{
  const std::string expected_name("abc");
  const std::string expected_file_name("edf");

  auto local_variable = DomainUtils::CreateDomainVariable(DomainConstants::kFileVariableType);
  local_variable->AddAttribute(DomainConstants::kNameAttribute, expected_name);
  local_variable->AddAttribute(DomainConstants::kFileAttribute, expected_file_name);

  sequencergui::FileVariableItem local_variable_item;
  local_variable_item.InitFromDomain(local_variable.get());

  EXPECT_EQ(local_variable_item.GetName(), expected_name);
  EXPECT_EQ(local_variable_item.GetFileName(), expected_file_name);
}

TEST_F(StandardVariableItemsTest, FileVariableItemToDomain)
{
  const std::string expected_name("abc");
  const std::string expected_file_name("edf");

  sequencergui::FileVariableItem item;
  item.SetName(expected_name);
  item.SetFileName(expected_file_name);

  auto domain_item = item.CreateDomainVariable();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kFileVariableType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kNameAttribute), expected_name);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kFileAttribute), expected_file_name);
}

//! LocalVariableItem

TEST_F(StandardVariableItemsTest, LocalVariableItem)
{
  sequencergui::LocalVariableItem item;
  EXPECT_TRUE(item.GetName().empty());
  EXPECT_TRUE(item.GetJsonType().empty());
  EXPECT_TRUE(item.GetJsonValue().empty());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  item.SetJsonType("cde");
  EXPECT_EQ(item.GetJsonType(), std::string("cde"));

  item.SetJsonValue("fjk");
  EXPECT_EQ(item.GetJsonValue(), std::string("fjk"));
}

TEST_F(StandardVariableItemsTest, LocalVariableItemFromDomain)
{
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  auto local_variable = DomainUtils::CreateDomainVariable(DomainConstants::kLocalVariableType);
  local_variable->AddAttribute("name", expected_name);
  local_variable->AddAttribute("type", expected_type);
  local_variable->AddAttribute("value", expected_value);

  sequencergui::LocalVariableItem local_variable_item;
  local_variable_item.InitFromDomain(local_variable.get());

  EXPECT_EQ(local_variable_item.GetName(), expected_name);
  EXPECT_EQ(local_variable_item.GetJsonType(), expected_type);
  EXPECT_EQ(local_variable_item.GetJsonValue(), expected_value);
}

TEST_F(StandardVariableItemsTest, LocalVariableItemToDomain)
{
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  sequencergui::LocalVariableItem item;
  item.SetName(expected_name);
  item.SetJsonType(expected_type);
  item.SetJsonValue(expected_value);

  auto domain_item = item.CreateDomainVariable();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kLocalVariableType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kNameAttribute), expected_name);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kTypeAttribute), expected_type);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kValueAttribute), expected_value);
  EXPECT_EQ(domain_item->GetAttributes().GetAttributeNames().size(), 3);
}

//! Check domain variable construction when "value" attribute is not set
//! (real-life bug: LocalVariable doesn't like value=""

TEST_F(StandardVariableItemsTest, LocalVariableItemToDomainMissedValue)
{
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("");

  sequencergui::LocalVariableItem item;
  item.SetName(expected_name);
  item.SetJsonType(expected_type);
  item.SetJsonValue(expected_value);

  auto domain_item = item.CreateDomainVariable();
  EXPECT_FALSE(domain_item->HasAttribute(DomainConstants::kValueAttribute));
}

//! PVClientVariableItem

TEST_F(StandardVariableItemsTest, PVClientVariableItem)
{
  PVClientVariableItem item;
  EXPECT_TRUE(item.GetName().empty());
  EXPECT_TRUE(item.GetChannel().empty());
  EXPECT_TRUE(item.GetDataType().empty());

  item.SetName("abc");
  EXPECT_EQ(item.GetName(), std::string("abc"));

  item.SetChannel("def");
  EXPECT_EQ(item.GetChannel(), std::string("def"));

  item.SetDataType("jkl");
  EXPECT_EQ(item.GetDataType(), std::string("jkl"));
}

TEST_F(StandardVariableItemsTest, PVClientVariableItemFromDomain)
{
  const std::string expected_name("expected_name");
  const std::string expected_channel("expected_channel");
  const std::string expected_datatype("expected_datatype");

  if (DomainUtils::IsChannelAccessAvailable())
  {
    auto ca_variable = DomainUtils::CreateDomainVariable(DomainConstants::kPVClientVariableType);
    ca_variable->AddAttribute(DomainConstants::kNameAttribute, expected_name);
    ca_variable->AddAttribute(DomainConstants::kChannelAttribute, expected_channel);
    ca_variable->AddAttribute(DomainConstants::kDataTypeAttribute, expected_datatype);

    PVClientVariableItem ca_variable_item;
    ca_variable_item.InitFromDomain(ca_variable.get());

    EXPECT_EQ(ca_variable_item.GetName(), expected_name);
    EXPECT_EQ(ca_variable_item.GetChannel(), expected_channel);
    EXPECT_EQ(ca_variable_item.GetDataType(), expected_datatype);
  }
}

TEST_F(StandardVariableItemsTest, PVClientVariableItemToDomain)
{
  const std::string expected_name("expected_name");
  const std::string expected_channel("expected_channel");
  const std::string expected_datatype("expected_datatype");

  if (DomainUtils::IsChannelAccessAvailable())
  {
    PVClientVariableItem item;
    item.SetName(expected_name);
    item.SetChannel(expected_channel);
    item.SetDataType(expected_datatype);

    auto domain_item = item.CreateDomainVariable();
    EXPECT_EQ(domain_item->GetType(), DomainConstants::kPVClientVariableType);
    EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kNameAttribute), expected_name);
    EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kChannelAttribute), expected_channel);
    EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kDataTypeAttribute), expected_datatype);
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

  auto domain_variable = DomainUtils::CreateDomainVariable(DomainConstants::kLocalVariableType);
  domain_variable->AddAttribute(DomainConstants::kNameAttribute, expected_name);
  domain_variable->AddAttribute(DomainConstants::kTypeAttribute, expected_type);
  domain_variable->AddAttribute(DomainConstants::kValueAttribute, expected_value);

  // from domain
  UnknownVariableItem item;
  item.InitFromDomain(domain_variable.get());
  EXPECT_EQ(item.Property<std::string>(DomainConstants::kNameAttribute), expected_name);
  EXPECT_EQ(item.Property<std::string>(DomainConstants::kTypeAttribute), expected_type);
  EXPECT_EQ(item.Property<std::string>(DomainConstants::kValueAttribute), expected_value);

  // to domain
  auto new_domain_item = item.CreateDomainVariable();
  EXPECT_EQ(new_domain_item->GetType(), DomainConstants::kLocalVariableType);
  EXPECT_EQ(new_domain_item->GetAttribute(DomainConstants::kNameAttribute), expected_name);
  EXPECT_EQ(new_domain_item->GetAttribute(DomainConstants::kTypeAttribute), expected_type);
  EXPECT_EQ(new_domain_item->GetAttribute(DomainConstants::kValueAttribute), expected_value);
}
