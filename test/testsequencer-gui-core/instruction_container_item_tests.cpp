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

#include "sequencergui/model/instruction_container_item.h"

#include <sequencergui/model/standard_instruction_items.h>

#include <gtest/gtest.h>

using namespace sequencergui;

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
