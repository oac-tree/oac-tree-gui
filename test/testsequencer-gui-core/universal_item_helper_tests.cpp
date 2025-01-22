/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "sequencergui/model/universal_item_helper.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/utils/numeric_utils.h>

#include <gtest/gtest.h>
#include <testutils/universal_items.h>

namespace sequencergui::test
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

  EXPECT_EQ(item->GetType(), WaitItem::Type);
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

  EXPECT_EQ(item->GetType(), UniversalInstructionItem::Type);
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

}  // namespace sequencergui::test
