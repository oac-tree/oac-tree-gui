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
  sequencergui::UniversalInstructionItem item;

  //  EXPECT_TRUE(item.GetName().empty());
  //  EXPECT_TRUE(item.GetDisplayName().empty());

  // tags registered in the c-tor of UniversalVariableItem
  std::vector<std::string> expected_tags({"kChildInstructions"});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);
  EXPECT_TRUE(item.GetDomainType().empty());
}

//! Initialisation from default constructed domain variable.
TEST_F(UniversalInstructionItemTests, InitFromDomain)
{
  auto local_variable = CreateDomainInstruction(domainconstants::kWaitInstructionType);

  sequencergui::UniversalInstructionItem item;
  item.InitFromDomain(local_variable.get());

  EXPECT_EQ(item.GetDomainType(), domainconstants::kWaitInstructionType);
}
