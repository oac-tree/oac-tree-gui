/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/utils/numeric_utils.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for helper methods from universal_item_helper.h

class UniversalItemHelperTest : public ::testing::Test
{
public:
  //! Test instruction playing the role of domain instruction unknown to the GUI.
  class UnknownDomainInstruction : public ::sup::sequencer::Instruction
  {
  public:
    UnknownDomainInstruction() : Instruction(Type) {}

    ::sup::sequencer::ExecutionStatus ExecuteSingleImpl(::sup::sequencer::UserInterface&,
                                                        ::sup::sequencer::Workspace&) override
    {
      return {};
    }
    static inline const std::string Type = "UnknownDomainInstruction";

    static void RegisterUnknownDomainInstruction()
    {
      sup::sequencer::RegisterGlobalInstruction<UnknownDomainInstruction>();
    }
  };
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
  UnknownDomainInstruction::RegisterUnknownDomainInstruction();

  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  auto item =
      InsertInstruction(UnknownDomainInstruction::Type, procedure->GetInstructionContainer());

  EXPECT_EQ(item->GetType(), UniversalInstructionItem::Type);
  EXPECT_EQ(item->GetDomainType(), UnknownDomainInstruction::Type);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetTotalItemCount(), 1);
}
