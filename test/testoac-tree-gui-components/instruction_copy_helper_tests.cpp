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

#include "oac_tree_gui/composer/instruction_copy_helper.h"

#include <oac_tree_gui/model/standard_instruction_items.h>

#include <mvvm/model/application_model.h>
#include <mvvm/utils/container_utils.h>

#include <gtest/gtest.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

/**
 * @brief Testing helper methods from instruction_copy_helper.h header.
 */
class InstructionCopyHelperTest : public ::testing::Test
{
public:
  /**
   * @brief Helper function to take vector of pointers casted to certain type from the vector of
   * unique_ptr.
   */
  template <typename ContainerT, typename InstructionT = InstructionItem>
  std::vector<InstructionT*> GetInstructionsPtr(const ContainerT& vector_of_unique_ptr)
  {
    auto vector_of_pointers = mvvm::utils::GetVectorOfPtrs(vector_of_unique_ptr);
    return mvvm::utils::CastItems<InstructionT>(vector_of_pointers);
  }
};

//! Validating that CreateInstructionCopyMimeData copies only top level instruction and skips
//! children instructions.
TEST_F(InstructionCopyHelperTest, CreateInstructionCopyMimeData)
{
  mvvm::ApplicationModel model;

  auto sequence = model.InsertItem<SequenceItem>();
  sequence->SetDisplayName("abc");
  auto wait = model.InsertItem<WaitItem>(sequence);

  auto mime_data = CreateInstructionCopyMimeData(*sequence);

  auto reconstructed_items = CreateInstructions(mime_data.get());
  auto reconstructed_instructions = GetInstructionsPtr(reconstructed_items);
  ASSERT_EQ(reconstructed_instructions.size(), 1);

  auto reconstructed_sequence = reconstructed_instructions.at(0);
  ASSERT_NE(reconstructed_sequence, nullptr);
  EXPECT_EQ(reconstructed_sequence->GetDisplayName(), std::string("abc"));

  // children instruction wasn't copied
  EXPECT_TRUE(reconstructed_sequence->GetInstructions().empty());
}

TEST_F(InstructionCopyHelperTest, CreateInstructionTreeCopyMimeData)
{
  mvvm::ApplicationModel model;

  auto sequence = model.InsertItem<SequenceItem>();
  sequence->SetDisplayName("abc");
  auto wait = model.InsertItem<WaitItem>(sequence);
  wait->SetDisplayName("def");

  auto mime_data = CreateInstructionTreeCopyMimeData(*sequence);

  auto reconstructed_items = CreateInstructions(mime_data.get());
  auto reconstructed_instructions = GetInstructionsPtr(reconstructed_items);
  ASSERT_EQ(reconstructed_instructions.size(), 1);

  auto reconstructed_sequence = reconstructed_instructions.at(0);
  EXPECT_EQ(reconstructed_sequence->GetDisplayName(), std::string("abc"));

  // children were copied
  ASSERT_EQ(reconstructed_sequence->GetInstructions().size(), 1);
  EXPECT_EQ(reconstructed_sequence->GetInstructions().at(0)->GetDisplayName(), std::string("def"));
}

TEST_F(InstructionCopyHelperTest, CreateInstructionSelectionCopyMimeData)
{
  mvvm::ApplicationModel model;

  // parent and two children
  auto sequence0 = model.InsertItem<SequenceItem>();
  sequence0->SetDisplayName("sequence0");
  auto wait0 = model.InsertItem<WaitItem>(sequence0);
  wait0->SetDisplayName("wait0");
  auto sequence1 = model.InsertItem<SequenceItem>(sequence0);
  sequence1->SetDisplayName("sequence1");

  // grandchildren
  auto wait1 = model.InsertItem<WaitItem>(sequence1);
  wait1->SetDisplayName("wait1");

  {  // user selection contains two children only
    const std::vector<InstructionItem*> selection({wait0, sequence1});

    const auto mime_data = CreateInstructionSelectionCopyMimeData(selection);

    auto reconstructed_items = CreateInstructions(mime_data.get());
    auto reconstructed_instructions = GetInstructionsPtr(reconstructed_items);

    ASSERT_EQ(reconstructed_instructions.size(), 2);
    EXPECT_EQ(reconstructed_instructions.at(0)->GetDisplayName(), std::string("wait0"));
    EXPECT_EQ(reconstructed_instructions.at(1)->GetDisplayName(), std::string("sequence1"));

    // sequence doesn't contain any childre
    EXPECT_TRUE(reconstructed_instructions.at(1)->GetInstructions().empty());
  }

  {  // user selection contains main sequence
    const std::vector<InstructionItem*> selection({sequence0, sequence1, wait1});

    const auto mime_data = CreateInstructionSelectionCopyMimeData(selection);

    auto reconstructed_items = CreateInstructions(mime_data.get());
    auto reconstructed_instructions = GetInstructionsPtr(reconstructed_items);

    ASSERT_EQ(reconstructed_instructions.size(), 1);
    auto reconstructed_parent = reconstructed_instructions.at(0);
    EXPECT_EQ(reconstructed_parent->GetDisplayName(), std::string("sequence0"));

    // checking child
    ASSERT_EQ(reconstructed_parent->GetInstructions().size(), 1);
    auto reconstructed_child = reconstructed_parent->GetInstructions().at(0);
    EXPECT_EQ(reconstructed_child->GetDisplayName(), std::string("sequence1"));

    // checking grandchild
    ASSERT_EQ(reconstructed_child->GetInstructions().size(), 1);
    auto reconstructed_grandchild = reconstructed_child->GetInstructions().at(0);
    EXPECT_EQ(reconstructed_grandchild->GetDisplayName(), std::string("wait1"));
  }
}

}  // namespace oac_tree_gui::test
