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

#include "oac_tree_gui/model/universal_instruction_item.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/universal_item_helper.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/instruction.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Tests UniversalVariableItem.

class UniversalInstructionItemTest : public ::testing::Test
{
};

TEST_F(UniversalInstructionItemTest, InitialState)
{
  const UniversalInstructionItem item;

  EXPECT_EQ(item.GetDisplayName(), UniversalInstructionItem::GetStaticType());
  EXPECT_TRUE(mvvm::utils::RegisteredTags(item).empty());
  EXPECT_TRUE(item.GetDomainType().empty());
}

//! Attempt to create domain variable using uninitialized item.

TEST_F(UniversalInstructionItemTest, AttemptToCreateDomainVariable)
{
  const UniversalInstructionItem item;
  EXPECT_THROW(item.CreateDomainInstruction(), sup::oac_tree::InvalidOperationException);
}

//! Initialisation from default constructed domain variable.

TEST_F(UniversalInstructionItemTest, InitFromDomain)
{
  auto domain_instruction = CreateDomainInstruction(domainconstants::kWaitInstructionType);

  UniversalInstructionItem item;
  item.InitFromDomain(domain_instruction.get());

  EXPECT_EQ(item.GetDomainType(), domainconstants::kWaitInstructionType);

  // registered tags should coincide with instruction attribute and AnyValueTag
  const std::vector<std::string> expected_tags(
      {domainconstants::kNameAttribute, domainconstants::kIsRootAttribute,
       domainconstants::kTimeoutAttribute, domainconstants::kBlockingAttribute,
       itemconstants::kBehaviorTag, itemconstants::kStatus, itemconstants::kXpos,
       itemconstants::kYpos, itemconstants::kBreakpoint});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);

  // property items should provide an access to underlying values
  auto properties = mvvm::utils::SinglePropertyItems(item);
  ASSERT_EQ(properties.size(), 5);
  EXPECT_EQ(properties.at(0)->GetDisplayName(), domainconstants::kNameAttribute);
  EXPECT_TRUE(item.Property<std::string>(domainconstants::kNameAttribute).empty());

  EXPECT_EQ(properties.at(1)->GetDisplayName(), domainconstants::kIsRootAttribute);
  EXPECT_EQ(item.Property<bool>(domainconstants::kIsRootAttribute), false);

  EXPECT_EQ(properties.at(2)->GetDisplayName(), domainconstants::kTimeoutAttribute);
  EXPECT_EQ(item.Property<double>(domainconstants::kTimeoutAttribute), 0);
}

//! Creating instruction using existing domain name.
//! Please note, that the resulting object will report same name for GetType() and GetDomainType().

TEST_F(UniversalInstructionItemTest, CreateUsingDomainName)
{
  UniversalInstructionItem item(domainconstants::kWaitInstructionType);
  (void) item.SetProperty(domainconstants::kNameAttribute, "abc");

  // different wrt test below
  EXPECT_EQ(item.GetType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(item.GetDomainType(), domainconstants::kWaitInstructionType);

  auto domain_instruction = item.CreateDomainInstruction();
  EXPECT_EQ(domain_instruction->GetType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(domain_instruction->GetAttributeString(domainconstants::kNameAttribute), "abc");

  // more tests in standard_instruction_item_tests.cpp
}

//! Creating instruction using default constructor followed by SetDomainName.
//! Please note, that the resulting object will report different names for GetType() and
//! GetDomainType().

TEST_F(UniversalInstructionItemTest, SetDomainName)
{
  UniversalInstructionItem item;

  item.SetDomainType(domainconstants::kWaitInstructionType);

  // different wrt test above
  EXPECT_EQ(item.GetType(), UniversalInstructionItem::GetStaticType());
  EXPECT_EQ(item.GetDomainType(), domainconstants::kWaitInstructionType);

  (void) item.SetProperty(domainconstants::kNameAttribute, "abc");

  auto domain_instruction = item.CreateDomainInstruction();
  EXPECT_EQ(domain_instruction->GetType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(domain_instruction->GetAttributeString(domainconstants::kNameAttribute), "abc");

  // more tests in standard_instruction_item_tests.cpp
}

TEST_F(UniversalInstructionItemTest, Behavior)
{
  UniversalInstructionItem item(domainconstants::kWaitInstructionType);
  ;

  EXPECT_EQ(item.GetBehavior(), itemconstants::kNativeBehavior);
  item.SetBehavior(itemconstants::kHiddenBehavior);
  EXPECT_EQ(item.GetBehavior(), itemconstants::kHiddenBehavior);
}

TEST_F(UniversalInstructionItemTest, IncludeProcedureCollapsedAttribute)
{
  {  // domain doesn't have ShowCollapsed defined
    auto domain = CreateDomainInstruction(domainconstants::kIncludeProcedureInstructionType);
    domain->AddAttribute(domainconstants::kFileNameAttribute, "abc");

    UniversalInstructionItem item(domainconstants::kIncludeProcedureInstructionType);
    item.InitFromDomain(domain.get());

    // by default item is collapsed
    EXPECT_TRUE(IsCollapsed(item));
  }

  {  // domain has ShowCollapsed set to false
    auto domain = CreateDomainInstruction(domainconstants::kIncludeProcedureInstructionType);
    domain->AddAttribute(domainconstants::kFileNameAttribute, "abc");
    domain->AddAttribute(domainconstants::kShowCollapsedAttribute, "false");

    UniversalInstructionItem item(domainconstants::kIncludeProcedureInstructionType);
    item.InitFromDomain(domain.get());

    // item got the value from the domain
    EXPECT_FALSE(IsCollapsed(item));
  }

  {  // domain has ShowCollapsed set to true
    auto domain = CreateDomainInstruction(domainconstants::kIncludeProcedureInstructionType);
    domain->AddAttribute(domainconstants::kFileNameAttribute, "abc");
    domain->AddAttribute(domainconstants::kShowCollapsedAttribute, "true");

    UniversalInstructionItem item(domainconstants::kIncludeProcedureInstructionType);
    item.InitFromDomain(domain.get());

    // item got the value from the domain
    EXPECT_TRUE(IsCollapsed(item));
  }

  {  // creating domain, when GUI has ShowCollapsed set to true
    const UniversalInstructionItem item(domainconstants::kIncludeProcedureInstructionType);
    EXPECT_TRUE(IsCollapsed(item));

    auto domain = item.CreateDomainInstruction();
    EXPECT_TRUE(domain->HasAttribute(domainconstants::kShowCollapsedAttribute));
    EXPECT_EQ(domain->GetAttributeString(domainconstants::kShowCollapsedAttribute),
              std::string("true"));
  }

  {  // creating domain, when GUI has ShowCollapsed set to false
    UniversalInstructionItem item(domainconstants::kIncludeProcedureInstructionType);
    (void) item.SetProperty(domainconstants::kShowCollapsedAttribute, false);
    EXPECT_FALSE(IsCollapsed(item));

    auto domain = item.CreateDomainInstruction();
    // we propagate false attributes too
    EXPECT_TRUE(domain->HasAttribute(domainconstants::kShowCollapsedAttribute));
    EXPECT_EQ(domain->GetAttributeString(domainconstants::kShowCollapsedAttribute),
              std::string("false"));
  }
}

}  // namespace oac_tree_gui::test
