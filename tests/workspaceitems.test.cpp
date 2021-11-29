/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/workspaceitems.h"

#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/transformfromdomain.h"

#include "Variable.h"

#include <gtest/gtest.h>

using namespace sequi;

//! Tests for items from workspaceitems.h

class WorkspaceItemsTest : public ::testing::Test
{
};

//! Validate Workspace

TEST_F(WorkspaceItemsTest, WorkspaceItem)
{
  WorkspaceItem item;
  EXPECT_TRUE(item.GetVariables().empty());

  auto var0 = item.InsertItem<LocalVariableItem>(ModelView::TagIndex::Append());
  auto var1 = item.InsertItem<LocalVariableItem>(ModelView::TagIndex::Append());
  EXPECT_EQ(item.GetVariables(), std::vector<VariableItem*>({var0, var1}));
}

//! LocalVariable Item

TEST_F(WorkspaceItemsTest, LocalVariableItem)
{
  sequi::LocalVariableItem item;
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

TEST_F(WorkspaceItemsTest, LocalVariableItemFromDomain)
{
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  auto local_variable = DomainUtils::CreateDomainVariable(DomainConstants::kLocalVariableType);
  local_variable->AddAttribute("name", expected_name);
  local_variable->AddAttribute("type", expected_type);
  local_variable->AddAttribute("value", expected_value);

  sequi::LocalVariableItem local_variable_item;
  local_variable_item.InitFromDomain(local_variable.get());

  EXPECT_EQ(local_variable_item.GetName(), expected_name);
  EXPECT_EQ(local_variable_item.GetJsonType(), expected_type);
  EXPECT_EQ(local_variable_item.GetJsonValue(), expected_value);
}

TEST_F(WorkspaceItemsTest, LocalVariableItemToDomain)
{
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  sequi::LocalVariableItem item;
  item.SetName(expected_name);
  item.SetJsonType(expected_type);
  item.SetJsonValue(expected_value);

  auto domain_item = item.CreateDomainVariable();
  EXPECT_EQ(domain_item->GetType(), DomainConstants::kLocalVariableType);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kNameAttribute), expected_name);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kTypeAttribute), expected_type);
  EXPECT_EQ(domain_item->GetAttribute(DomainConstants::kValueAttribute), expected_value);
}
