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

#include "sequencergui/model/guiobjectbuilder.h"

#include "Instruction.h"
#include "Procedure.h"
#include "Variable.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/standardinstructionitems.h"
#include "sequencergui/model/procedureitems.h"
#include "sequencergui/model/standardvariableitems.h"
#include "sequencergui/model/transformfromdomain.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for utility functions related to the domain to presentation transformations.

class GUIObjectBuilderTest : public ::testing::Test
{
};

//! Populate InstructionContainerItem from empty Procedure.

TEST_F(GUIObjectBuilderTest, PopulateItemContainerFromEmptyProcedure)
{
  ::sup::sequencer::Procedure procedure;
  sequencergui::ProcedureItem procedure_item;

  GUIObjectBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item);

  EXPECT_EQ(procedure_item.GetInstructionContainer()->GetTotalItemCount(), 0);
  EXPECT_EQ(procedure_item.GetWorkspace()->GetTotalItemCount(), 0);
}

//! Populate InstructionContainerItem from Procedure with a single Wait instruction.

TEST_F(GUIObjectBuilderTest, PopulateItemContainerFromProcedureWithWait)
{
  ::sup::sequencer::Procedure procedure;

  auto wait = DomainUtils::CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait->AddAttribute(sequencergui::domainconstants::kWaitTimeoutAttribute, "42");
  auto wait_ptr = wait.get();
  procedure.PushInstruction(wait.release());

  sequencergui::ProcedureItem procedure_item;
  GUIObjectBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item);

  auto item = procedure_item.GetInstructionContainer()->GetItem<sequencergui::WaitItem>("");
  EXPECT_EQ(item->GetTimeout(), 42.0);
  EXPECT_EQ(builder.FindInstructionItemIdentifier(wait_ptr), item->GetIdentifier());
  EXPECT_EQ(builder.FindInstructionItem(wait_ptr), item);
}

//! Populate InstructionContainerItem from Procedure with a Sequence containing Wait instruction.

TEST_F(GUIObjectBuilderTest, PopulateItemContainerFromProcedureWithSequence)
{
  ::sup::sequencer::Procedure procedure;

  auto wait = DomainUtils::CreateDomainInstruction(domainconstants::kWaitInstructionType);
  auto wait_ptr = wait.get();
  wait->AddAttribute(sequencergui::domainconstants::kWaitTimeoutAttribute, "42");

  auto sequence = DomainUtils::CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto sequence_ptr = sequence.get();
  sequence->InsertInstruction(wait.release(), 0);

  procedure.PushInstruction(sequence.release());
  procedure.Setup();

  sequencergui::ProcedureItem procedure_item;
  GUIObjectBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item);

  auto sequence_item =
      procedure_item.GetInstructionContainer()->GetItem<sequencergui::SequenceItem>("");
  auto wait_item = sequence_item->GetItem<sequencergui::WaitItem>("");
  EXPECT_EQ(wait_item->GetTimeout(), 42.0);

  EXPECT_EQ(builder.FindInstructionItemIdentifier(wait_ptr), wait_item->GetIdentifier());
  EXPECT_EQ(builder.FindInstructionItemIdentifier(sequence_ptr), sequence_item->GetIdentifier());
  EXPECT_EQ(builder.FindInstructionItem(wait_ptr), wait_item);
  EXPECT_EQ(builder.FindInstructionItem(sequence_ptr), sequence_item);
}

//! Populate WorkspaceItem from empty procedure.

TEST_F(GUIObjectBuilderTest, PopulateWorkspaceItemFromProcedureWithLocalVariable)
{
  ::sup::sequencer::Procedure procedure;

  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  auto local_variable = DomainUtils::CreateDomainVariable(domainconstants::kLocalVariableType);
  local_variable->AddAttribute(domainconstants::kTypeAttribute, expected_type);
  local_variable->AddAttribute(domainconstants::kValueAttribute, expected_value);
  auto local_variable_ptr = local_variable.get();

  procedure.AddVariable("abc", local_variable.release());
  procedure.Setup();

  sequencergui::ProcedureItem procedure_item;
  GUIObjectBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item);

  EXPECT_EQ(procedure_item.GetWorkspace()->GetTotalItemCount(), 1);

  auto variable_item = procedure_item.GetWorkspace()->GetItem<sequencergui::LocalVariableItem>("");
  EXPECT_EQ(variable_item->GetJsonType(), expected_type);
  EXPECT_EQ(variable_item->GetJsonValue(), expected_value);

  EXPECT_EQ(builder.FindVariableItemIdentifier(local_variable_ptr), variable_item->GetIdentifier());
  EXPECT_EQ(builder.FindVariableItem(local_variable_ptr), variable_item);
  EXPECT_EQ(builder.FindVariableItemIdentifier(local_variable_ptr->GetName()),
            variable_item->GetIdentifier());
  EXPECT_EQ(builder.FindVariableItem(local_variable_ptr->GetName()), variable_item);
}

//! Named Sequence and Include instruction
//! <Sequence name="CountTwice">
//!     <Wait/>
//! </Sequence>
//! <Repeat isRoot="true" maxCount="10">
//!     <Include name="Counts" path="CountTwice"/>
//! </Repeat>

TEST_F(GUIObjectBuilderTest, LocalIncludeScenario)
{
  ::sup::sequencer::Procedure procedure;

  // Sequence with wait instruction
  auto wait = DomainUtils::CreateDomainInstruction(domainconstants::kWaitInstructionType);
  auto wait_ptr = wait.get();
  wait->AddAttribute(sequencergui::domainconstants::kWaitTimeoutAttribute, "42");

  auto sequence = DomainUtils::CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto sequence_ptr = sequence.get();
  sequence->AddAttribute(sequencergui::domainconstants::kNameAttribute, "MySequence");
  sequence->InsertInstruction(wait.release(), 0);

  // Repeat with include instruction
  auto include = DomainUtils::CreateDomainInstruction(domainconstants::kIncludeInstructionType);
  auto include_ptr = include.get();
  include->AddAttribute(sequencergui::domainconstants::kNameAttribute, "MyInclude");
  include->AddAttribute(sequencergui::domainconstants::kPathAttribute, "MySequence");

  auto repeat = DomainUtils::CreateDomainInstruction(domainconstants::kRepeatInstructionType);
  auto repeat_ptr = repeat.get();
  repeat->AddAttribute(sequencergui::domainconstants::kIsRootAttribute, "true");
  repeat->AddAttribute(sequencergui::domainconstants::kMaxCountAttribute, "10");
  repeat->InsertInstruction(include.release(), 0);

  // procedure with two instructions
  procedure.PushInstruction(sequence.release());
  procedure.PushInstruction(repeat.release());
  procedure.Setup();

  // Building ProcedureItem
  sequencergui::ProcedureItem procedure_item;
  GUIObjectBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item);

  // only one root instruction has been processed
  EXPECT_EQ(procedure_item.GetInstructionContainer()->GetTotalItemCount(), 1);

  auto repeat_item =
      procedure_item.GetInstructionContainer()->GetItem<sequencergui::RepeatItem>("");
  auto include_item = repeat_item->GetItem<sequencergui::IncludeItem>("");
  auto sequence_item = include_item->GetItem<sequencergui::SequenceItem>("");
  auto wait_item = sequence_item->GetItem<sequencergui::WaitItem>("");

  // Repeat and Include instructions corresponds to their domain counterpart
  EXPECT_EQ(builder.FindInstructionItemIdentifier(repeat_ptr), repeat_item->GetIdentifier());
  EXPECT_EQ(builder.FindInstructionItemIdentifier(include_ptr), include_item->GetIdentifier());

  // However SequenceItem and WaitItem have been made from cloned versions of Sequence and Wait
  EXPECT_EQ(builder.FindInstructionItemIdentifier(sequence_ptr), std::string());
  EXPECT_EQ(builder.FindInstructionItemIdentifier(wait_ptr), std::string());

  // Domain clones should lead to constructed SequenceItem and WaitItem
  EXPECT_EQ(procedure.RootInstruction(), repeat_ptr);
  EXPECT_EQ(repeat_ptr->ChildInstructions().at(0), include_ptr);

  auto cloned_domain_sequence = include_ptr->ChildInstructions().at(0);
  auto cloned_domain_wait = cloned_domain_sequence->ChildInstructions().at(0);
  EXPECT_NE(cloned_domain_sequence, sequence_ptr);
  EXPECT_NE(cloned_domain_wait, wait_ptr);

  EXPECT_EQ(builder.FindInstructionItemIdentifier(cloned_domain_sequence),
            sequence_item->GetIdentifier());
  EXPECT_EQ(builder.FindInstructionItemIdentifier(cloned_domain_wait), wait_item->GetIdentifier());
}
