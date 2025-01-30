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

#include <oac_tree_gui/model/standard_instruction_items.h>

#include <gtest/gtest.h>

#include "oac_tree_gui/model/iterate_helper.h"

namespace oac_tree_gui::test
{

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

}  // namespace oac_tree_gui::test
