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

#include "oac_tree_gui/model/iterate_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_factory.h>
#include <oac_tree_gui/model/universal_item_helper.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Tests for helper methods initerate_helper.h.

class IterateHelperTest : public ::testing::Test
{
};

TEST_F(IterateHelperTest, IterateInstruction)
{
  {
    std::vector<const InstructionItem*> items;
    auto func = [&items](const InstructionItem* item) { items.push_back(item); };

    auto wait_item = CreateInstructionItem(domainconstants::kWaitInstructionType);
    IterateInstruction<const InstructionItem*>(wait_item.get(), func);
    EXPECT_EQ(items, std::vector<const InstructionItem*>({wait_item.get()}));
  }

  {
    std::vector<mvvm::SessionItem*> items;
    auto func = [&items](InstructionItem* item) { items.push_back(item); };

    auto sequence = CreateInstructionItem(domainconstants::kSequenceInstructionType);
    auto wait_item = InsertInstruction(domainconstants::kWaitInstructionType, sequence.get());
    EXPECT_EQ(sequence->GetInstructions().size(), 1);

    IterateInstruction<InstructionItem*>(sequence.get(), func);

    EXPECT_EQ(items, std::vector<mvvm::SessionItem*>({sequence.get(), wait_item}));
  }
}

}  // namespace oac_tree_gui::test
