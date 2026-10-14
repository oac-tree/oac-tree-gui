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

#include "oac_tree_gui/model/instruction_item_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/transform/instruction_item_transform_helper.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>
#include <testutils/universal_items.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for helper methods from instruction_item_helper.h
 */
class InstructionItemHelperTest : public ::testing::Test
{
};

TEST_F(InstructionItemHelperTest, AddKnownInstructionToSequencerModel)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  auto item = InsertInstruction(domainconstants::kWaitInstructionType,
                                procedure->GetInstructionContainer());

  EXPECT_EQ(item->GetDomainType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetTotalItemCount(), 1);
}

TEST_F(InstructionItemHelperTest, AddUnknownDomainInstructionToTheGUI)
{
  using test::UnknownDomainInstruction;

  UnknownDomainInstruction::RegisterUnknownDomainInstruction();

  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  auto item =
      InsertInstruction(UnknownDomainInstruction::Type, procedure->GetInstructionContainer());

  EXPECT_EQ(item->GetType(), mvvm::GetTypeName<InstructionItem>());
  EXPECT_EQ(item->GetDomainType(), UnknownDomainInstruction::Type);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetTotalItemCount(), 1);
}

TEST_F(InstructionItemHelperTest, IsCollapsed)
{
  auto item = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  EXPECT_FALSE(IsCollapsed(*item));

  SetCollapsed(true, *item);
  EXPECT_TRUE(IsCollapsed(*item));

  SetCollapsed(false, *item);
  EXPECT_FALSE(IsCollapsed(*item));
}

TEST_F(InstructionItemHelperTest, ToggleCollapsed)
{
  auto item = CreateInstructionItem(domainconstants::kSequenceInstructionType);

  EXPECT_FALSE(IsCollapsed(*item));
  ToggleCollapsed(*item);
  EXPECT_TRUE(IsCollapsed(*item));
  ToggleCollapsed(*item);
  EXPECT_FALSE(IsCollapsed(*item));
}

TEST_F(InstructionItemHelperTest, IsCollapsible)
{
  EXPECT_TRUE(IsCollapsible(*CreateInstructionItem(domainconstants::kSequenceInstructionType)));
  EXPECT_FALSE(IsCollapsible(*CreateInstructionItem(domainconstants::kWaitInstructionType)));
}

TEST_F(InstructionItemHelperTest, GetCollapsedItems)
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

TEST_F(InstructionItemHelperTest, IsCompoundInstruction)
{
  auto sequence = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  EXPECT_TRUE(IsCompoundInstruction(sequence.get()));

  auto repeat = CreateInstructionItem(domainconstants::kRepeatInstructionType);
  EXPECT_TRUE(IsCompoundInstruction(repeat.get()));

  auto wait = CreateInstructionItem(domainconstants::kWaitInstructionType);
  EXPECT_FALSE(IsCompoundInstruction(wait.get()));
}

TEST_F(InstructionItemHelperTest, IsDecoratorInstruction)
{
  auto sequence = CreateInstructionItem(domainconstants::kSequenceInstructionType);
  EXPECT_FALSE(IsDecoratorInstruction(sequence.get()));

  auto repeat = CreateInstructionItem(domainconstants::kRepeatInstructionType);
  EXPECT_TRUE(IsDecoratorInstruction(repeat.get()));

  auto wait = CreateInstructionItem(domainconstants::kWaitInstructionType);
  EXPECT_FALSE(IsDecoratorInstruction(wait.get()));
}

TEST_F(InstructionItemHelperTest, IsPotentialRootInstruction)
{
  {  // instruction outside container
    const auto sequence = CreateInstructionItem(domainconstants::kSequenceInstructionType);
    EXPECT_FALSE(IsPotentialRootInstruction(*sequence));
  }

  {  // two instructions in container
    InstructionContainerItem container;
    auto sequence0 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
    auto sequence1 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
    EXPECT_TRUE(IsPotentialRootInstruction(*sequence0));
    EXPECT_TRUE(IsPotentialRootInstruction(*sequence1));
  }

  {  // instruction with child in container
    InstructionContainerItem container;
    auto sequence0 = InsertInstruction(domainconstants::kSequenceInstructionType, &container);
    auto wait0 = InsertInstruction(domainconstants::kWaitInstructionType, sequence0);
    EXPECT_TRUE(IsPotentialRootInstruction(*sequence0));
    EXPECT_FALSE(IsPotentialRootInstruction(*wait0));
  }
}

TEST_F(InstructionItemHelperTest, GetIsRootItem)
{
  const mvvm::SessionItem item;
  EXPECT_EQ(GetIsRootItem(item), nullptr);

  auto wait = CreateInstructionItem(domainconstants::kWaitInstructionType);
  EXPECT_NE(GetIsRootItem(*wait), nullptr);
}

}  // namespace oac_tree_gui::test
