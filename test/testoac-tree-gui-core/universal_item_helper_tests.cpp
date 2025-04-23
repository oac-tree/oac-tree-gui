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

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/utils/numeric_utils.h>

#include <gtest/gtest.h>
#include <testutils/universal_items.h>

#include "oac_tree_gui/model/universal_item_helper.h"

namespace oac_tree_gui::test
{

//! Tests for helper methods from universal_item_helper.h

class UniversalItemHelperTest : public ::testing::Test
{
};

//! Validating helper method GetDropTagIndex.

TEST_F(UniversalItemHelperTest, IsAggregateName)
{
  EXPECT_FALSE(IsAggregateName(""));
  EXPECT_FALSE(IsAggregateName("Wait"));
  EXPECT_TRUE(IsAggregateName("if-then-else"));
}

//! Checking adding known domain_type instruction to SequencerModel.

TEST_F(UniversalItemHelperTest, AddKnownInstruction)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  auto item = InsertInstruction(domainconstants::kWaitInstructionType,
                                procedure->GetInstructionContainer());

  EXPECT_EQ(item->GetType(), WaitItem::GetStaticType());
  EXPECT_EQ(item->GetDomainType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetTotalItemCount(), 1);
}

//! Checking adding unknown domain_type instruction to SequencerModel.

TEST_F(UniversalItemHelperTest, AddUnknownInstruction)
{
  using test::UnknownDomainInstruction;

  UnknownDomainInstruction::RegisterUnknownDomainInstruction();

  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  auto item =
      InsertInstruction(UnknownDomainInstruction::Type, procedure->GetInstructionContainer());

  EXPECT_EQ(item->GetType(), UniversalInstructionItem::GetStaticType());
  EXPECT_EQ(item->GetDomainType(), UnknownDomainInstruction::Type);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetTotalItemCount(), 1);
}

TEST_F(UniversalItemHelperTest, IsCollapsed)
{
  SequenceItem item;
  EXPECT_FALSE(IsCollapsed(item));

  item.SetProperty(domainconstants::kShowCollapsedAttribute, true);
  EXPECT_TRUE(IsCollapsed(item));
}

TEST_F(UniversalItemHelperTest, GetCollapsedItems)
{
  InstructionContainerItem container;
  auto sequence0 = container.InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  auto wait0 = container.InsertItem<WaitItem>(mvvm::TagIndex::Append());
  auto sequence1 = sequence0->InsertItem<SequenceItem>(mvvm::TagIndex::Append());

  auto collapsed = GetCollapsedItems(container);
  EXPECT_TRUE(collapsed.empty());

  sequence0->SetProperty(domainconstants::kShowCollapsedAttribute, true);
  sequence1->SetProperty(domainconstants::kShowCollapsedAttribute, true);

  collapsed = GetCollapsedItems(container);
  EXPECT_EQ(collapsed, std::vector<const InstructionItem*>({sequence0, sequence1}));
}

}  // namespace oac_tree_gui::test
