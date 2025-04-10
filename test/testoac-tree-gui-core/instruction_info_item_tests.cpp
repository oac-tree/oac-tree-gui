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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/model/instruction_info_item.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/universal_item_helper.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_info.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

namespace oac_tree_gui::test
{

class InstructionInfoItemTest : public ::testing::Test
{
};

TEST_F(InstructionInfoItemTest, InitialState)
{
  const InstructionInfoItem item;

  EXPECT_TRUE(item.GetDomainType().empty());
  EXPECT_TRUE(mvvm::utils::RegisteredTags(item).empty());
  EXPECT_THROW(item.CreateDomainInstruction(), sup::oac_tree::InvalidOperationException);
}

TEST_F(InstructionInfoItemTest, InitFromWaitInstructionInfo)
{
  const std::string expected_name("MySpecialWait");
  const std::size_t instruction_id{0};
  const sup::oac_tree::InstructionInfo info(oac_tree_gui::domainconstants::kWaitInstructionType,
                                            sup::oac_tree::Instruction::Category::kAction,
                                            instruction_id,
                                            {{domainconstants::kTimeoutAttribute, "42"},
                                             {domainconstants::kNameAttribute, expected_name}});

  InstructionInfoItem item;
  item.InitFromDomainInfo(info);
  EXPECT_EQ(item.GetDomainType(), oac_tree_gui::domainconstants::kWaitInstructionType);
  EXPECT_EQ(item.GetName(), expected_name);

  ASSERT_TRUE(mvvm::utils::HasTag(item, domainconstants::kTimeoutAttribute));
  EXPECT_EQ(item.Property<std::string>(domainconstants::kTimeoutAttribute), "42");

  const std::vector<std::string> expected_tags(
      {domainconstants::kNameAttribute, itemconstants::kBehaviorTag, itemconstants::kStatus,
       itemconstants::kXpos, itemconstants::kYpos, itemconstants::kBreakpoint,
       domainconstants::kTimeoutAttribute});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);
}

//! Same as previous test, except that we init from InstructionInfo twice. This checks that there is
//! no attempts to recreate existing properties.
TEST_F(InstructionInfoItemTest, InitFromInstructionInfoTwice)
{
  const std::string expected_value("MySpecialWait");
  const std::size_t instruction_id{0};
  const sup::oac_tree::InstructionInfo info(oac_tree_gui::domainconstants::kWaitInstructionType,
                                            sup::oac_tree::Instruction::Category::kAction,
                                            instruction_id,
                                            {{domainconstants::kTimeoutAttribute, "42"},
                                             {domainconstants::kNameAttribute, expected_value}});

  InstructionInfoItem item;
  item.InitFromDomainInfo(info);
  item.InitFromDomainInfo(info);
  EXPECT_EQ(item.GetDomainType(), oac_tree_gui::domainconstants::kWaitInstructionType);
  EXPECT_EQ(item.GetName(), expected_value);

  ASSERT_TRUE(mvvm::utils::HasTag(item, domainconstants::kTimeoutAttribute));
  EXPECT_EQ(item.Property<std::string>(domainconstants::kTimeoutAttribute), "42");
}

TEST_F(InstructionInfoItemTest, InitFromSequenceInstructionInfo)
{
  const std::string expected_value = "expected_value";
  auto instruction = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  instruction->AddAttribute(domainconstants::kNameAttribute, expected_value);
  auto info = CreateInstructionInfo(*instruction);

  InstructionInfoItem item;
  item.InitFromDomainInfo(info);
  EXPECT_EQ(item.GetDomainType(), oac_tree_gui::domainconstants::kSequenceInstructionType);
  EXPECT_EQ(item.GetName(), expected_value);

  const std::vector<std::string> expected_tags(
      {domainconstants::kNameAttribute, itemconstants::kChildInstructions,
       itemconstants::kBehaviorTag, itemconstants::kStatus, itemconstants::kXpos,
       itemconstants::kYpos, itemconstants::kBreakpoint, domainconstants::kShowCollapsedAttribute});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);

  EXPECT_FALSE(IsCollapsed(item));  // Sequence by default is expanded
}

TEST_F(InstructionInfoItemTest, InitFromIncludeInstructionInfo)
{
  const std::string expected_value = "expected_value";
  auto instruction = CreateDomainInstruction(domainconstants::kIncludeInstructionType);
  instruction->AddAttribute(domainconstants::kNameAttribute, expected_value);
  auto info = CreateInstructionInfo(*instruction);

  InstructionInfoItem item;
  item.InitFromDomainInfo(info);
  EXPECT_EQ(item.GetDomainType(), oac_tree_gui::domainconstants::kIncludeInstructionType);
  EXPECT_EQ(item.GetName(), expected_value);

  const std::vector<std::string> expected_tags(
      {domainconstants::kNameAttribute, itemconstants::kChildInstructions,
       itemconstants::kBehaviorTag, itemconstants::kStatus, itemconstants::kXpos,
       itemconstants::kYpos, itemconstants::kBreakpoint, domainconstants::kShowCollapsedAttribute});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);

  EXPECT_TRUE(IsCollapsed(item));  // Inclusde by default is collapsed
}

TEST_F(InstructionInfoItemTest, InitFromIncludeInstructionInfoWithCollapsedAttributeSetTrue)
{
  const std::string expected_value = "expected_value";
  auto instruction = CreateDomainInstruction(domainconstants::kIncludeInstructionType);
  instruction->AddAttribute(domainconstants::kNameAttribute, expected_value);
  instruction->AddAttribute(domainconstants::kShowCollapsedAttribute, "true");
  auto info = CreateInstructionInfo(*instruction);

  InstructionInfoItem item;
  item.InitFromDomainInfo(info);

  EXPECT_TRUE(IsCollapsed(item));
}

TEST_F(InstructionInfoItemTest, InitFromIncludeInstructionInfoWithCollapsedAttributeSetFalse)
{
  const std::string expected_value = "expected_value";
  auto instruction = CreateDomainInstruction(domainconstants::kIncludeInstructionType);
  instruction->AddAttribute(domainconstants::kNameAttribute, expected_value);
  instruction->AddAttribute(domainconstants::kShowCollapsedAttribute, "false");
  auto info = CreateInstructionInfo(*instruction);

  InstructionInfoItem item;
  item.InitFromDomainInfo(info);

  EXPECT_FALSE(IsCollapsed(item));
}

}  // namespace oac_tree_gui::test
