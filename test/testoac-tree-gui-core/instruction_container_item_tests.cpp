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

#include <oac_tree_gui/model/standard_instruction_items.h>

#include <gtest/gtest.h>

#include "oac_tree_gui/model/instruction_container_item.h"

namespace oac_tree_gui::test
{

class InstructionContainerItemTest : public ::testing::Test
{
};

TEST_F(InstructionContainerItemTest, InitialState)
{
  InstructionContainerItem item;
  EXPECT_TRUE(item.GetInstructions().empty());
  EXPECT_EQ(item.GetInstructionCount(), 0);
}

TEST_F(InstructionContainerItemTest, InsertItem)
{
  InstructionContainerItem item;
  auto wait = item.InsertItem<WaitItem>(mvvm::TagIndex::Append());
  auto sequence = item.InsertItem<SequenceItem>(mvvm::TagIndex::Append());

  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait, sequence}));
  EXPECT_EQ(item.GetInstructionCount(), 2);
}

}  // namespace oac_tree_gui::test
