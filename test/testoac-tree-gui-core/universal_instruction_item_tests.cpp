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

#include "oac_tree_gui/model/universal_instruction_item.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/transform/instruction_item_transform_helper.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/instruction.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Tests UniversalVariableItem.

class UniversalInstructionItemTest : public ::testing::Test
{
};

TEST_F(UniversalInstructionItemTest, InitialState)
{
  const UniversalInstructionItem item;

  EXPECT_EQ(item.GetType(), mvvm::GetTypeName<UniversalInstructionItem>());
  EXPECT_EQ(item.GetDomainType(), std::string());
  EXPECT_EQ(item.GetDisplayName(), mvvm::GetTypeName<UniversalInstructionItem>());

  EXPECT_TRUE(mvvm::utils::RegisteredTags(item).empty());
}

//! SetDomainType sets the domain type only. GetType() and GetDomainType() differ.
TEST_F(UniversalInstructionItemTest, SetDomainType)
{
  UniversalInstructionItem item;

  item.SetDomainType(domainconstants::kWaitInstructionType);

  EXPECT_EQ(item.GetType(), mvvm::GetTypeName<UniversalInstructionItem>());
  EXPECT_EQ(item.GetDomainType(), domainconstants::kWaitInstructionType);
  EXPECT_TRUE(mvvm::utils::RegisteredTags(item).empty());
}

TEST_F(UniversalInstructionItemTest, Behavior)
{
  auto item = CreateInstructionItem(domainconstants::kWaitInstructionType);

  EXPECT_EQ(item->GetBehavior(), itemconstants::kNativeBehavior);
  item->SetBehavior(itemconstants::kHiddenBehavior);
  EXPECT_EQ(item->GetBehavior(), itemconstants::kHiddenBehavior);
}

}  // namespace oac_tree_gui::test
