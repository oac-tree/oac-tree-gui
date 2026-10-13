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

#include "oac_tree_gui/model/universal_item_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_factory.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/universal_instruction_item.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>
#include <testutils/universal_items.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for helper methods from universal_item_helper.h
 */
class UniversalItemHelperTest : public ::testing::Test
{
};

TEST_F(UniversalItemHelperTest, AddKnownInstructionToSequencerModel)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  auto item = InsertInstruction(domainconstants::kWaitInstructionType,
                                procedure->GetInstructionContainer());

  EXPECT_EQ(item->GetDomainType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetTotalItemCount(), 1);
}

TEST_F(UniversalItemHelperTest, AddUnknownDomainInstructionToTheGUI)
{
  using test::UnknownDomainInstruction;

  UnknownDomainInstruction::RegisterUnknownDomainInstruction();

  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  auto item =
      InsertInstruction(UnknownDomainInstruction::Type, procedure->GetInstructionContainer());

  EXPECT_EQ(item->GetType(), mvvm::GetTypeName<UniversalInstructionItem>());
  EXPECT_EQ(item->GetDomainType(), UnknownDomainInstruction::Type);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetTotalItemCount(), 1);
}

TEST_F(UniversalItemHelperTest, IsCollapsed)
{
  auto item = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  EXPECT_FALSE(IsCollapsed(*item));

  SetCollapsed(true, *item);
  EXPECT_TRUE(IsCollapsed(*item));

  SetCollapsed(false, *item);
  EXPECT_FALSE(IsCollapsed(*item));
}

TEST_F(UniversalItemHelperTest, ToggleCollapsed)
{
  auto item = CreateInstructionItem(domainconstants::kSequenceInstructionType);

  EXPECT_FALSE(IsCollapsed(*item));
  ToggleCollapsed(*item);
  EXPECT_TRUE(IsCollapsed(*item));
  ToggleCollapsed(*item);
  EXPECT_FALSE(IsCollapsed(*item));
}

TEST_F(UniversalItemHelperTest, IsCollapsible)
{
  EXPECT_TRUE(IsCollapsible(*CreateInstructionItem(domainconstants::kSequenceInstructionType)));
  EXPECT_FALSE(IsCollapsible(*CreateInstructionItem(domainconstants::kWaitInstructionType)));
}

TEST_F(UniversalItemHelperTest, GetCollapsedItems)
{
  InstructionContainerItem container;
  auto sequence0 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
  auto wait0 = InsertInstruction(domainconstants::kWaitInstructionType, &container);
  auto sequence1 = InsertInstruction(domainconstants::kSequenceInstructionType, sequence0);

  auto collapsed = GetCollapsedItems(container);
  EXPECT_TRUE(collapsed.empty());

  (void)sequence0->SetProperty(domainconstants::kShowCollapsedAttribute, true);
  (void)sequence1->SetProperty(domainconstants::kShowCollapsedAttribute, true);

  collapsed = GetCollapsedItems(container);
  EXPECT_EQ(collapsed, std::vector<const InstructionItem*>({sequence0, sequence1}));
}

}  // namespace oac_tree_gui::test
