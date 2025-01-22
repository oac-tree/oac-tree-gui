/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/universal_variable_item.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/model/item_constants.h>

#include <mvvm/model/item_utils.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>

namespace sequencergui::test
{

//! Tests UniversalVariableItem.

class UniversalVariableItemTest : public ::testing::Test
{
};

TEST_F(UniversalVariableItemTest, InitialState)
{
  UniversalVariableItem item;

  EXPECT_EQ(item.GetDisplayName(), UniversalVariableItem::Type);

  EXPECT_TRUE(mvvm::utils::RegisteredTags(item).empty());
  EXPECT_TRUE(item.GetDomainType().empty());
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);
  EXPECT_TRUE(item.IsAvailable());
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
  const std::vector<std::string> expected_tags(
      {domainconstants::kDynamicTypeAttribute, itemconstants::kAnyValueTag});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);

  // property items should give an access
  auto properties = mvvm::utils::SinglePropertyItems(item);
  ASSERT_EQ(properties.size(), 1);

  EXPECT_EQ(properties.at(0)->GetDisplayName(), domainconstants::kDynamicTypeAttribute);
  EXPECT_EQ(item.Property<bool>(domainconstants::kDynamicTypeAttribute), false);

  // setting up domain variable and repeat initialisation
  domain_variable->AddAttribute(domainconstants::kNameAttribute, "abc");
  workspace_t ws;
  domain_variable->Setup(ws);
  EXPECT_NO_THROW(item.InitFromDomain(domain_variable.get()));

  EXPECT_EQ(item.GetDisplayName(), std::string("abc"));
}

//! Attempt to create domain variable using uninitialized item.

TEST_F(UniversalVariableItemTest, AttemptToCreateDomainVariable)
{
  UniversalVariableItem item;
  EXPECT_THROW(item.CreateDomainVariable(), sup::sequencer::InvalidOperationException);
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
  EXPECT_EQ(item.GetType(), UniversalVariableItem::Type);
  EXPECT_EQ(item.GetDomainType(), domainconstants::kLocalVariableType);

  item.SetDisplayName("abc");

  auto domain_variable = item.CreateDomainVariable();
  EXPECT_EQ(domain_variable->GetType(), domainconstants::kLocalVariableType);
  EXPECT_EQ(domain_variable->GetAttributeString(domainconstants::kNameAttribute), "abc");

  // more tests in standard_variable_item_tests.cpp
}

}  // namespace sequencergui::test
