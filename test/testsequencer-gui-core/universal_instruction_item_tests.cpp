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

#include "sequencergui/model/universal_instruction_item.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/item_constants.h>

#include <mvvm/model/item_utils.h>

#include <sup/sequencer/instruction.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests UniversalVariableItem.

class UniversalInstructionItemTests : public ::testing::Test
{
};

TEST_F(UniversalInstructionItemTests, InitialState)
{
  UniversalInstructionItem item;

  EXPECT_EQ(item.GetDisplayName(), UniversalInstructionItem::Type);

  std::vector<std::string> expected_tags({});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);

  EXPECT_TRUE(item.GetDomainType().empty());
}

//! Attempt to create domain variable using uninitialized item.

TEST_F(UniversalInstructionItemTests, AttemptToCreateDomainVariable)
{
  UniversalInstructionItem item;
  EXPECT_THROW(item.CreateDomainInstruction(), sup::sequencer::InvalidOperationException);
}

//! Initialisation from default constructed domain variable.

TEST_F(UniversalInstructionItemTests, InitFromDomain)
{
  auto domain_instruction = CreateDomainInstruction(domainconstants::kWaitInstructionType);

  UniversalInstructionItem item;
  item.InitFromDomain(domain_instruction.get());

  EXPECT_EQ(item.GetDomainType(), domainconstants::kWaitInstructionType);

  // registered tags should coincide with instruction attribute and AnyValueTag
  std::vector<std::string> expected_tags(
      {domainconstants::kNameAttribute, domainconstants::kIsRootAttribute,
       domainconstants::kTimeoutAttribute, itemconstants::kStatus, itemconstants::kXpos,
       itemconstants::kYpos});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);

  // property items should provide an access to underlying values
  auto properties = mvvm::utils::SinglePropertyItems(item);
  ASSERT_EQ(properties.size(), 3);
  EXPECT_EQ(properties.at(0)->GetDisplayName(), domainconstants::kNameAttribute);
  EXPECT_TRUE(item.Property<std::string>(domainconstants::kNameAttribute).empty());

  EXPECT_EQ(properties.at(1)->GetDisplayName(), domainconstants::kIsRootAttribute);
  EXPECT_EQ(item.Property<bool>(domainconstants::kIsRootAttribute), false);

  EXPECT_EQ(properties.at(2)->GetDisplayName(), domainconstants::kTimeoutAttribute);
  EXPECT_EQ(item.Property<double>(domainconstants::kTimeoutAttribute), 0);
}

TEST_F(UniversalInstructionItemTests, CreateUsingDomainName)
{
  UniversalInstructionItem item(domainconstants::kWaitInstructionType);
  item.SetProperty(domainconstants::kNameAttribute, "abc");

  auto domain_variable = item.CreateDomainInstruction();
  EXPECT_EQ(domain_variable->GetType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(domain_variable->GetAttributeString(domainconstants::kNameAttribute), "abc");

  // more tests in standard_instruction_item_tests.cpp
}
