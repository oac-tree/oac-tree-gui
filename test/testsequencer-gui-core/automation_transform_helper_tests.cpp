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

#include <sup/auto-server/instruction_info.h>

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

  auto instruction_item = CreateInstructionItemTree(info);

  auto wait_item = dynamic_cast<WaitItem*>(instruction_item.get());
  ASSERT_NE(wait_item, nullptr);
  EXPECT_EQ(wait_item->GetTimeout(), 42.0);
}

//! Testing CreateInstructionItemTree for automation InstructionInfo containing a single
//! instruction.
TEST_F(AutomationTransformHelperTest, CreateInstructionItemTreeForSequence)
{
  using namespace sequencergui::domainconstants;
  using sup::auto_server::InstructionInfo;

  InstructionInfo sequence_info(kSequenceInstructionType, 0, {});
  auto child0 = std::unique_ptr<InstructionInfo>(
      new InstructionInfo(kWaitInstructionType, 1, {{kTimeoutAttribute, "42"}}));
  auto child0_ptr = child0.get();
  auto child1 = std::unique_ptr<InstructionInfo>(
      new InstructionInfo(kWaitInstructionType, 2, {{kTimeoutAttribute, "43"}}));
  auto child1_ptr = child0.get();

  sequence_info.AppendChild(std::move(child0));
  sequence_info.AppendChild(std::move(child1));

  auto item = CreateInstructionItemTree(sequence_info);

  auto sequence_item = dynamic_cast<SequenceItem*>(item.get());
  ASSERT_NE(sequence_item, nullptr);
  ASSERT_EQ(sequence_item->GetInstructions().size(), 2);
  auto wait_items = sequence_item->GetItems<WaitItem>(mvvm::TagIndex::kDefaultTag);
  ASSERT_EQ(wait_items.size(), 2);
  EXPECT_EQ(wait_items[0]->GetTimeout(), 42);
  EXPECT_EQ(wait_items[1]->GetTimeout(), 43);
}
