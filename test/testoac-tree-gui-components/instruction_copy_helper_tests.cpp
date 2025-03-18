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

#include <gtest/gtest.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

/**
 * @brief Testing helper methods from instruction_copy_helper.h header.
 */
class InstructionCopyHelperTest : public ::testing::Test
{
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
  ASSERT_EQ(reconstructed_items.size(), 1);

  auto reconstructed_sequence = dynamic_cast<SequenceItem*>(reconstructed_items.at(0).get());
  ASSERT_NE(reconstructed_sequence, nullptr);
  EXPECT_EQ(reconstructed_sequence->GetDisplayName(), std::string("abc"));

  // children instruction wasn't copied
  EXPECT_TRUE(reconstructed_sequence->GetInstructions().empty());
}

}  // namespace oac_tree_gui::test
