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

#include "sequencergui/transform/automation_transform_helper.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/variable_item.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>

#include <sup/auto-server/instruction_info.h>
#include <sup/auto-server/variable_info.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests of helper methods from automation_transform_helper.h

class AutomationTransformHelperTest : public ::testing::Test
{
};

//! Testing CreateInstructionItem helper method.
TEST_F(AutomationTransformHelperTest, CreateInstructionItem)
{
  const size_t instruction_id{0};
  sup::auto_server::InstructionInfo info(sequencergui::domainconstants::kWaitInstructionType,
                                         instruction_id,
                                         {{domainconstants::kTimeoutAttribute, "42"}});

  auto instruction_item = CreateInstructionItem(info);

  auto wait_item = dynamic_cast<WaitItem*>(instruction_item.get());
  ASSERT_NE(wait_item, nullptr);
  EXPECT_EQ(wait_item->GetTimeout(), 42.0);
}

//! Testing CreateInstructionItemTree for automation InstructionInfo containing a single
//! instruction.
TEST_F(AutomationTransformHelperTest, CreateInstructionItemTreeForWait)
{
  const size_t instruction_id{0};
  sup::auto_server::InstructionInfo info(sequencergui::domainconstants::kWaitInstructionType,
                                         instruction_id,
                                         {{domainconstants::kTimeoutAttribute, "42"}});

  auto item_tree = CreateInstructionItemTree(info);
  ASSERT_EQ(item_tree.indexes.size(), 1);

  auto wait_item = dynamic_cast<WaitItem*>(item_tree.root.get());

  ASSERT_NE(wait_item, nullptr);
  EXPECT_EQ(wait_item->GetTimeout(), 42.0);

  EXPECT_EQ(item_tree.indexes[0], wait_item);
}

//! Testing CreateInstructionItemTree for automation InstructionInfo containing a sequence with two
//! children.
TEST_F(AutomationTransformHelperTest, CreateInstructionItemTreeForSequence)
{
  using namespace sequencergui::domainconstants;
  using sup::auto_server::AttributeInfo;
  using sup::auto_server::InstructionInfo;

  InstructionInfo sequence_info(kSequenceInstructionType, 0, {});
  auto child0 = std::make_unique<InstructionInfo>(
      kWaitInstructionType, 1, std::vector<AttributeInfo>({{kTimeoutAttribute, "42"}}));
  auto child0_ptr = child0.get();
  auto child1 = std::make_unique<InstructionInfo>(
      kWaitInstructionType, 2, std::vector<AttributeInfo>({{kTimeoutAttribute, "43"}}));
  auto child1_ptr = child0.get();

  sequence_info.AppendChild(std::move(child0));
  sequence_info.AppendChild(std::move(child1));

  auto item_tree = CreateInstructionItemTree(sequence_info);

  ASSERT_EQ(item_tree.indexes.size(), 3);

  auto sequence_item = dynamic_cast<SequenceItem*>(item_tree.root.get());
  ASSERT_NE(sequence_item, nullptr);
  ASSERT_EQ(sequence_item->GetInstructions().size(), 2);
  auto wait_items = sequence_item->GetItems<WaitItem>(mvvm::TagIndex::kDefaultTag);
  ASSERT_EQ(wait_items.size(), 2);
  EXPECT_EQ(wait_items[0]->GetTimeout(), 42);
  EXPECT_EQ(wait_items[1]->GetTimeout(), 43);

  EXPECT_EQ(item_tree.indexes[0], sequence_item);
  EXPECT_EQ(item_tree.indexes[1], wait_items[0]);
  EXPECT_EQ(item_tree.indexes[2], wait_items[1]);
}

//! Testing CreateVariableItem helper method.
TEST_F(AutomationTransformHelperTest, CreateVariableItem)
{
  const size_t variable_id{0};
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  std::vector<sup::auto_server::AttributeInfo> attributes(
      {{domainconstants::kNameAttribute, expected_name},
       {domainconstants::kTypeAttribute, expected_type},
       {domainconstants::kValueAttribute, expected_value}});
  sup::auto_server::VariableInfo info(domainconstants::kLocalVariableType, variable_id, attributes);

  auto variable_item = CreateVariableItem(info);

  EXPECT_EQ(variable_item->GetType(), domainconstants::kLocalVariableType);
  auto stored_anyvalue = CreateAnyValue(*variable_item->GetAnyValueItem());
  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  EXPECT_EQ(stored_anyvalue, expected_anyvalue);
}
