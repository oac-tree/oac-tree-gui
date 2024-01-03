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

#include "sequencergui/model/iterate_helper.h"

#include <sequencergui/model/standard_instruction_items.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for helper methods initerate_helper.h.

class IterateHelperTest : public ::testing::Test
{
};

TEST_F(IterateHelperTest, IterateInstruction)
{
  {
    std::vector<const InstructionItem *> items;
    auto func = [&items](const InstructionItem *item) { items.push_back(item); };

    WaitItem wait_item;
    IterateInstruction<const InstructionItem *>(&wait_item, func);
    EXPECT_EQ(items, std::vector<const InstructionItem *>({&wait_item}));
  }

  {
    std::vector<mvvm::SessionItem *> items;
    auto func = [&items](InstructionItem *item) { items.push_back(item); };

    SequenceItem sequence;
    auto wait_item = sequence.InsertItem<WaitItem>(mvvm::TagIndex::Append());
    EXPECT_EQ(sequence.GetInstructions().size(), 1);

    IterateInstruction<InstructionItem *>(&sequence, func);

    EXPECT_EQ(items, std::vector<mvvm::SessionItem *>({&sequence, wait_item}));
  }
}
