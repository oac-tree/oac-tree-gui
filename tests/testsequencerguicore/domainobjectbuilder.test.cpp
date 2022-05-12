/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/domainobjectbuilder.h"

#include "ExecutionStatus.h"
#include "Instruction.h"
#include "Procedure.h"
#include "Workspace.h"
#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/procedureitems.h"
#include "sequencergui/model/sequencer_types.h"
#include "sequencergui/model/standardvariableitems.h"

#include "mvvm/model/sessionitem.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Testing DomainObjectBuilder class.

class DomainObjectBuilderTest : public ::testing::Test
{
};

//! Building domain procedure from empty ProcedureItem.

TEST_F(DomainObjectBuilderTest, EmptyProcedure)
{
  ProcedureItem procedure_item;

  DomainObjectBuilder builder;
  auto procedure = builder.CreateProcedure(&procedure_item);

  // Test constructed
  EXPECT_TRUE(procedure->GetFilename().empty());
  EXPECT_EQ(procedure->GetStatus(), ::sup::sequencer::ExecutionStatus::SUCCESS);

  // Empty instructions and variables
  EXPECT_EQ(procedure->RootInstruction(), nullptr);
  auto instructions = procedure->GetInstructions();
  EXPECT_EQ(instructions.size(), 0);
  EXPECT_EQ(procedure->GetInstructionCount(), 0);
  EXPECT_TRUE(procedure->VariableNames().empty());
}

//! Building domain procedure from ProcedureItem with a single sequence.

TEST_F(DomainObjectBuilderTest, ProcedureWithSingleInstruction)
{
  ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto sequence_item = container->InsertItem<SequenceItem>({"", -1});

  DomainObjectBuilder builder;
  auto procedure = builder.CreateProcedure(&procedure_item);

  // Empty instruction list
  EXPECT_TRUE(procedure->RootInstruction() != nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 1);
  ASSERT_EQ(procedure->GetInstructions().at(0)->GetType(),
            domainconstants::kSequenceInstructionType);
  EXPECT_TRUE(procedure->VariableNames().empty());
}

//! Building domain procedure from ProcedureItem with two instructions.

TEST_F(DomainObjectBuilderTest, ProcedureWithTwoInstructions)
{
  ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto wait0 = container->InsertItem<WaitItem>({"", -1});
  wait0->SetTimeout(0.1);
  auto wait1 = container->InsertItem<WaitItem>({"", -1});
  wait1->SetTimeout(0.2);

  DomainObjectBuilder builder;
  auto procedure = builder.CreateProcedure(&procedure_item);

  // Empty instruction list
  EXPECT_EQ(procedure->RootInstruction(), nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 2);
  auto instructions = procedure->GetInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(instructions.at(1)->GetType(), domainconstants::kWaitInstructionType);

  EXPECT_EQ(instructions.at(0)->GetAttribute(domainconstants::kWaitTimeoutAttribute), "0.1");
  EXPECT_EQ(instructions.at(1)->GetAttribute(domainconstants::kWaitTimeoutAttribute), "0.2");
}

//! Building domain procedure from ProcedureItem containing Sequence and wait in it.

TEST_F(DomainObjectBuilderTest, ProcedureWithParentAndChild)
{
  ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto sequence = container->InsertItem<SequenceItem>({"", -1});
  auto wait = sequence->InsertItem<WaitItem>({"", -1});

  DomainObjectBuilder builder;
  auto procedure = builder.CreateProcedure(&procedure_item);

  // Empty instruction list
  EXPECT_TRUE(procedure->RootInstruction() != nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 1);
  auto domain_sequence = procedure->GetInstructions().at(0);
  EXPECT_EQ(domain_sequence->GetType(), domainconstants::kSequenceInstructionType);
  ASSERT_EQ(domain_sequence->ChildrenCount(), 1);
  EXPECT_EQ(domain_sequence->ChildInstructions().at(0)->GetType(),
            domainconstants::kWaitInstructionType);
}

//! Building domain procedure from ProcedureItem containing Sequence and wait in it.
//! Using build with BuildProcedure method, check correspondance between domain objects in
//! InstructionItems.

TEST_F(DomainObjectBuilderTest, BuildProcedureWithParentAndChild)
{
  ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto sequence = container->InsertItem<SequenceItem>({"", -1});
  auto wait = sequence->InsertItem<WaitItem>({"", -1});

  DomainObjectBuilder builder;
  builder.BuildProcedure(&procedure_item);
  auto procedure = builder.GetProcedure();

  ASSERT_TRUE(procedure != nullptr);

  // Empty instruction list
  EXPECT_TRUE(procedure->RootInstruction() != nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 1);
  auto domain_sequence = procedure->GetInstructions().at(0);

  EXPECT_EQ(domain_sequence->GetType(), domainconstants::kSequenceInstructionType);
  ASSERT_EQ(domain_sequence->ChildrenCount(), 1);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain_sequence), sequence->GetIdentifier());

  auto domain_wait = domain_sequence->ChildInstructions().at(0);
  EXPECT_EQ(domain_wait->GetType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain_wait), wait->GetIdentifier());
}

//! Procedure containing inverter instruction with sequence, containing in turn a wait instruction

TEST_F(DomainObjectBuilderTest, InverterWithSequence)
{
  ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto inverter = container->InsertItem<InverterItem>({"", -1});
  auto sequence = inverter->InsertItem<SequenceItem>({"", -1});
  EXPECT_EQ(inverter->GetItem<SequenceItem>("", 0), sequence);
  auto wait = sequence->InsertItem<WaitItem>({"", -1});

  DomainObjectBuilder builder;
  builder.BuildProcedure(&procedure_item);
  auto procedure = builder.GetProcedure();

  ASSERT_TRUE(procedure != nullptr);

  // Empty instruction list
  EXPECT_TRUE(procedure->RootInstruction() != nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 1);
  auto domain_inverter = procedure->GetInstructions().at(0);

  EXPECT_EQ(domain_inverter->GetType(), domainconstants::kInverterInstructionType);
  ASSERT_EQ(domain_inverter->ChildrenCount(), 1);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain_inverter), inverter->GetIdentifier());

  auto domain_sequence = domain_inverter->ChildInstructions().at(0);
  EXPECT_EQ(domain_sequence->GetType(), domainconstants::kSequenceInstructionType);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain_sequence), sequence->GetIdentifier());

  auto domain_wait = domain_sequence->ChildInstructions().at(0);
  EXPECT_EQ(domain_wait->GetType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain_wait), wait->GetIdentifier());
}

//! Procedure containing inverter instruction with sequence, containing in turn a wait instruction

TEST_F(DomainObjectBuilderTest, RepeatWithSingleInstruction)
{
  ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto repeater = container->InsertItem<RepeatItem>({"", -1});
  auto sequence = repeater->InsertItem<SequenceItem>({"", -1});
  EXPECT_EQ(repeater->GetItem<SequenceItem>("", 0), sequence);

  DomainObjectBuilder builder;
  builder.BuildProcedure(&procedure_item);
  auto procedure = builder.GetProcedure();

  ASSERT_TRUE(procedure != nullptr);

  // Empty instruction list
  EXPECT_TRUE(procedure->RootInstruction() != nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 1);
  auto domain_repeat = procedure->GetInstructions().at(0);

  EXPECT_EQ(domain_repeat->GetType(), domainconstants::kRepeatInstructionType);
  ASSERT_EQ(domain_repeat->ChildrenCount(), 1);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain_repeat), repeater->GetIdentifier());

  auto domain_sequence = domain_repeat->ChildInstructions().at(0);
  EXPECT_EQ(domain_sequence->GetType(), domainconstants::kSequenceInstructionType);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain_sequence), sequence->GetIdentifier());
}

//! Building domain procedure from ProcedureItem with a single sequence.

TEST_F(DomainObjectBuilderTest, ProcedureWithVariable)
{
  ProcedureItem procedure_item;
  auto workspace = procedure_item.GetWorkspace();

  auto var_item0 = workspace->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var_item0->SetName("var0");
  auto var_item1 = workspace->InsertItem<FileVariableItem>(mvvm::TagIndex::Append());
  var_item1->SetName("var1");

  DomainObjectBuilder builder;
  builder.BuildProcedure(&procedure_item);

  auto procedure = builder.GetProcedure();
  EXPECT_EQ(procedure->VariableNames(), std::vector<std::string>({"var0", "var1"}));
  EXPECT_EQ(builder.FindVariableItemIdentifier(var_item0->GetName()), var_item0->GetIdentifier());
  EXPECT_EQ(builder.FindVariableItemIdentifier(var_item1->GetName()), var_item1->GetIdentifier());

  auto domain_var0 = procedure->GetWorkspace()->GetVariable("var0");
  auto domain_var1 = procedure->GetWorkspace()->GetVariable("var1");
  EXPECT_EQ(domain_var0->GetName(), var_item0->GetName());
  EXPECT_EQ(domain_var1->GetName(), var_item1->GetName());
  EXPECT_EQ(builder.FindVariableItemIdentifier(domain_var0), var_item0->GetIdentifier());
  EXPECT_EQ(builder.FindVariableItemIdentifier(domain_var1), var_item1->GetIdentifier());
}

TEST_F(DomainObjectBuilderTest, ProcedureWithParallelSequence)
{
  ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto sequence = container->InsertItem<ParallelSequenceItem>(mvvm::TagIndex::Append());
  auto wait0 = sequence->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  auto wait1 = sequence->InsertItem<WaitItem>(mvvm::TagIndex::Append());

  DomainObjectBuilder builder;
  auto procedure = builder.CreateProcedure(&procedure_item);

  // Empty instruction list
  EXPECT_TRUE(procedure->RootInstruction() != nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 1);
  auto domain_sequence = procedure->GetInstructions().at(0);
  EXPECT_EQ(domain_sequence->GetType(), domainconstants::kParallelInstructionType);
  ASSERT_EQ(domain_sequence->ChildrenCount(), 2);
  EXPECT_EQ(domain_sequence->ChildInstructions().at(0)->GetType(),
            domainconstants::kWaitInstructionType);
  EXPECT_EQ(domain_sequence->ChildInstructions().at(1)->GetType(),
            domainconstants::kWaitInstructionType);
}
