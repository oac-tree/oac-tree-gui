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

#include "oac_tree_gui/transform/instruction_item_transform_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_info_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/standard_instruction_items.h>

#include <sup/oac-tree/instruction_info.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests of helper methods in instruction_item_transform_helper.h
 */
class JobInfoTransformHelperTest : public ::testing::Test
{
};

//! Testing CreateInstructionItem helper method.
TEST_F(JobInfoTransformHelperTest, CreateInstructionItemFromInfoObject)
{
  const std::size_t instruction_id{0};
  const sup::oac_tree::InstructionInfo info(oac_tree_gui::domainconstants::kWaitInstructionType,
                                            instruction_id,
                                            {{domainconstants::kTimeoutAttribute, "42"}});

  auto instruction_item = CreateInstructionItem(info);

  auto wait_item = dynamic_cast<WaitItem*>(instruction_item.get());
  ASSERT_NE(wait_item, nullptr);
  EXPECT_EQ(wait_item->GetTimeout(), 42.0);
}

//! Testing CreateInstructionInfoItem method.
TEST_F(JobInfoTransformHelperTest, CreateInstructionInfoItemFromInfoObject)
{
  const std::size_t instruction_id{0};
  const sup::oac_tree::InstructionInfo info(oac_tree_gui::domainconstants::kWaitInstructionType,
                                            instruction_id,
                                            {{domainconstants::kTimeoutAttribute, "42"}});

  auto instruction_item = CreateInstructionInfoItem(info);

  auto wait_item = dynamic_cast<InstructionInfoItem*>(instruction_item.get());
  ASSERT_NE(wait_item, nullptr);
}

//! Testing CreateInstructionItemTree for automation InstructionInfo containing a single
//! instruction.
TEST_F(JobInfoTransformHelperTest, CreateInstructionItemTreeForWaitInfo)
{
  const std::size_t instruction_id{0};
  const sup::oac_tree::InstructionInfo info(oac_tree_gui::domainconstants::kWaitInstructionType,
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
TEST_F(JobInfoTransformHelperTest, CreateInstructionItemTreeForSequenceInfo)
{
  using namespace oac_tree_gui::domainconstants;
  using sup::oac_tree::AttributeInfo;
  using sup::oac_tree::InstructionInfo;

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

//! Creates simplified InstructionInfoItem tree.
TEST_F(JobInfoTransformHelperTest, CreateInstructionInfoItemTreeForSequenceInfo)
{
  using namespace oac_tree_gui::domainconstants;
  using sup::oac_tree::AttributeInfo;
  using sup::oac_tree::InstructionInfo;

  InstructionInfo sequence_info(kSequenceInstructionType, 0, {});
  auto child0 = std::make_unique<InstructionInfo>(
      kWaitInstructionType, 1, std::vector<AttributeInfo>({{kTimeoutAttribute, "42"}}));
  auto child0_ptr = child0.get();
  auto child1 = std::make_unique<InstructionInfo>(
      kWaitInstructionType, 2, std::vector<AttributeInfo>({{kTimeoutAttribute, "43"}}));
  auto child1_ptr = child0.get();

  sequence_info.AppendChild(std::move(child0));
  sequence_info.AppendChild(std::move(child1));

  auto item_tree = CreateInstructionInfoItemTree(sequence_info);

  ASSERT_EQ(item_tree.indexes.size(), 3);

  auto sequence_item = dynamic_cast<InstructionInfoItem*>(item_tree.root.get());
  ASSERT_NE(sequence_item, nullptr);
  ASSERT_EQ(sequence_item->GetInstructions().size(), 2);
  auto wait_items = sequence_item->GetItems<InstructionInfoItem>(mvvm::TagIndex::kDefaultTag);
  ASSERT_EQ(wait_items.size(), 2);

  EXPECT_EQ(item_tree.indexes[0], sequence_item);
  EXPECT_EQ(item_tree.indexes[1], wait_items[0]);
  EXPECT_EQ(item_tree.indexes[2], wait_items[1]);
}

TEST_F(JobInfoTransformHelperTest, CreateInstructionTreeFromRootInstruction)
{
  const std::string procedure_xml = R"RAW(
<Procedure>
  <Repeat maxCount="10">
    <Sequence>
      <Wait timeout="42"/>
      <Wait timeout="43"/>
    </Sequence>
  </Repeat>
</Procedure>
)RAW";
  auto procedure = sup::oac_tree::ParseProcedureString(procedure_xml);

  auto item_tree = CreateInstructionItemTree(*procedure->RootInstruction());
  ASSERT_EQ(item_tree.indexes.size(), 4);

  auto repeat_item = dynamic_cast<RepeatItem*>(item_tree.root.get());
  ASSERT_NE(repeat_item, nullptr);
  ASSERT_EQ(repeat_item->GetInstructions().size(), 1);
  auto sequence_item =
      dynamic_cast<SequenceItem*>(repeat_item->GetItem<SequenceItem>(mvvm::TagIndex::kDefaultTag));
  ASSERT_NE(sequence_item, nullptr);

  ASSERT_EQ(sequence_item->GetInstructions().size(), 2);

  auto wait_items = sequence_item->GetItems<WaitItem>(mvvm::TagIndex::kDefaultTag);
  ASSERT_EQ(wait_items.size(), 2);
  EXPECT_EQ(wait_items[0]->GetTimeout(), 42);
  EXPECT_EQ(wait_items[1]->GetTimeout(), 43);

  EXPECT_EQ(item_tree.indexes[0], repeat_item);
  EXPECT_EQ(item_tree.indexes[1], sequence_item);
  EXPECT_EQ(item_tree.indexes[2], wait_items[0]);
  EXPECT_EQ(item_tree.indexes[3], wait_items[1]);
}

}  // namespace oac_tree_gui::test
