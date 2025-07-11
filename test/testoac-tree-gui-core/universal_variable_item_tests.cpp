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

#include "oac_tree_gui/model/universal_variable_item.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Tests UniversalVariableItem.

class UniversalVariableItemTest : public ::testing::Test
{
};

TEST_F(UniversalVariableItemTest, InitialState)
{
  const UniversalVariableItem item;

  EXPECT_EQ(item.GetDisplayName(), UniversalVariableItem::GetStaticType());

  const std::vector<std::string> expected_tags({itemconstants::kIsAvailable});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);
  EXPECT_TRUE(item.GetDomainType().empty());
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);
  EXPECT_FALSE(item.IsAvailable());
}

//! Initialisation from default constructed domain variable.

TEST_F(UniversalVariableItemTest, InitFromDomain)
{
  auto domain_variable = CreateDomainVariable(domainconstants::kLocalVariableType);

  UniversalVariableItem item;
  item.InitFromDomain(domain_variable.get());

  EXPECT_EQ(item.GetDomainType(), domainconstants::kLocalVariableType);

  // registered tags should coincide with name and dynamicType attributes, and AnyValueTag
  // (json type and value are filtered out)
  const std::vector<std::string> expected_tags({itemconstants::kIsAvailable,
                                                domainconstants::kDynamicTypeAttribute,
                                                itemconstants::kAnyValueTag});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);

  // property items should give an access
  auto properties = mvvm::utils::SinglePropertyItems(item);
  ASSERT_EQ(properties.size(), 1);

  EXPECT_EQ(properties.at(0)->GetDisplayName(), domainconstants::kDynamicTypeAttribute);
  EXPECT_EQ(item.Property<bool>(domainconstants::kDynamicTypeAttribute), false);

  // setting up domain variable and repeat initialisation
  domain_variable->AddAttribute(domainconstants::kNameAttribute, "abc");
  const workspace_t workspace;
  domain_variable->Setup(workspace);
  EXPECT_NO_THROW(item.InitFromDomain(domain_variable.get()));

  EXPECT_EQ(item.GetDisplayName(), std::string("abc"));
}

//! Attempt to create domain variable using uninitialized item.

TEST_F(UniversalVariableItemTest, AttemptToCreateDomainVariable)
{
  const UniversalVariableItem item;
  EXPECT_THROW(item.CreateDomainVariable(), sup::oac_tree::InvalidOperationException);
}

//! Creating variable using existing domain name.
//! Please note, that the resulting object will report same name for GetType() and GetDomainType().

TEST_F(UniversalVariableItemTest, CreateUsingDomainName)
{
  UniversalVariableItem item(domainconstants::kLocalVariableType);
  item.SetDisplayName("abc");

  // different wrt test below
  EXPECT_EQ(item.GetType(), domainconstants::kLocalVariableType);
  EXPECT_EQ(item.GetDomainType(), domainconstants::kLocalVariableType);

  auto domain_variable = item.CreateDomainVariable();
  EXPECT_EQ(domain_variable->GetType(), domainconstants::kLocalVariableType);
  EXPECT_EQ(domain_variable->GetAttributeString(domainconstants::kNameAttribute), "abc");

  // more tests in standard_variable_item_tests.cpp
}

//! Creating variable using default constructor followed by SetDomainName.
//! Please note, that the resulting object will report different names for GetType() and
//! GetDomainType().

TEST_F(UniversalVariableItemTest, SetDomainName)
{
  UniversalVariableItem item;

  item.SetDomainType(domainconstants::kLocalVariableType);

  // different wrt test below
  EXPECT_EQ(item.GetType(), UniversalVariableItem::GetStaticType());
  EXPECT_EQ(item.GetDomainType(), domainconstants::kLocalVariableType);

  item.SetDisplayName("abc");

  auto domain_variable = item.CreateDomainVariable();
  EXPECT_EQ(domain_variable->GetType(), domainconstants::kLocalVariableType);
  EXPECT_EQ(domain_variable->GetAttributeString(domainconstants::kNameAttribute), "abc");

  // more tests in standard_variable_item_tests.cpp
}

}  // namespace oac_tree_gui::test
