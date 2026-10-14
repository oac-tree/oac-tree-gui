/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "oac_tree_gui/model/instruction_container_item.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>

#include <gtest/gtest.h>

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
  auto wait = InsertInstruction(domainconstants::kWaitInstructionType, &item);
  auto sequence = InsertInstruction(domainconstants::kSequenceInstructionType, &item);

  EXPECT_EQ(item.GetInstructions(), std::vector<InstructionItem*>({wait, sequence}));
  EXPECT_EQ(item.GetInstructionCount(), 2);
}

}  // namespace oac_tree_gui::test
