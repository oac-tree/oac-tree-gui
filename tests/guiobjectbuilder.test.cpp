/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
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
#include "InstructionRegistry.h"
#include "Procedure.h"
#include "Variable.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/transformfromdomain.h"

#include <gtest/gtest.h>

using namespace sequi;

//! Tests for utility functions related to the domain to presentation transformations.

class GUIObjectBuilderTest : public ::testing::Test
{
};

//! Populate InstructionContainerItem from empty Procedure.

TEST_F(GUIObjectBuilderTest, PopulateItemContainerFromEmptyProcedure)
{
  ::sup::sequencer::Procedure procedure;
  sequi::ProcedureItem procedure_item;

  GUIObjectBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item);

  EXPECT_EQ(procedure_item.GetInstructionContainer()->GetTotalItemCount(), 0);
  EXPECT_EQ(procedure_item.GetWorkspace()->GetTotalItemCount(), 0);
}

//! Populate InstructionContainerItem from Procedure with a single Wait instruction.

TEST_F(GUIObjectBuilderTest, PopulateItemContainerFromProcedureWithWait)
{
  ::sup::sequencer::Procedure procedure;

  auto wait =
      ::sup::sequencer::GlobalInstructionRegistry().Create(DomainConstants::kWaitInstructionType);
  wait->AddAttribute(sequi::DomainConstants::kWaitTimeoutAttribute, "42");
  auto wait_ptr = wait.get();
  procedure.PushInstruction(wait.release());

  sequi::ProcedureItem procedure_item;
  GUIObjectBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item);

  auto item = procedure_item.GetInstructionContainer()->GetItem<sequi::WaitItem>("");
  EXPECT_EQ(item->GetTimeout(), 42.0);
  EXPECT_EQ(builder.FindInstructionIdentifier(wait_ptr), item->GetIdentifier());
}

//! Populate InstructionContainerItem from Procedure with a Sequence containing Wait instruction.

TEST_F(GUIObjectBuilderTest, PopulateItemContainerFromProcedureWithSequence)
{
  ::sup::sequencer::Procedure procedure;

  auto wait =
      ::sup::sequencer::GlobalInstructionRegistry().Create(DomainConstants::kWaitInstructionType);
  auto wait_ptr = wait.get();
  wait->AddAttribute(sequi::DomainConstants::kWaitTimeoutAttribute, "42");

  auto sequence = DomainUtils::CreateDomainInstruction(DomainConstants::kSequenceInstructionType);
  auto sequence_ptr = sequence.get();
  sequence->InsertInstruction(wait.release(), 0);

  procedure.PushInstruction(sequence.release());

  sequi::ProcedureItem procedure_item;
  GUIObjectBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item);

  auto sequence_item = procedure_item.GetInstructionContainer()->GetItem<sequi::SequenceItem>("");
  auto wait_item = sequence_item->GetItem<sequi::WaitItem>("");
  EXPECT_EQ(wait_item->GetTimeout(), 42.0);

  EXPECT_EQ(builder.FindInstructionIdentifier(wait_ptr), wait_item->GetIdentifier());
  EXPECT_EQ(builder.FindInstructionIdentifier(sequence_ptr), sequence_item->GetIdentifier());
}

//! Populate WorkspaceItem from empty procedure.

TEST_F(GUIObjectBuilderTest, PopulateWorkspaceItemFromProcedureWithLocalVariable)
{
  ::sup::sequencer::Procedure procedure;

  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  auto local_variable = DomainUtils::CreateDomainVariable(DomainConstants::kLocalVariableType);
  local_variable->AddAttribute(DomainConstants::kTypeAttribute, expected_type);
  local_variable->AddAttribute(DomainConstants::kValueAttribute, expected_value);
  auto local_variable_ptr = local_variable.get();

  procedure.AddVariable("abc", local_variable.release());

  sequi::ProcedureItem procedure_item;
  GUIObjectBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item);

  EXPECT_EQ(procedure_item.GetWorkspace()->GetTotalItemCount(), 1);

  auto variable_item = procedure_item.GetWorkspace()->GetItem<sequi::LocalVariableItem>("");
  EXPECT_EQ(variable_item->GetJsonType(), expected_type);
  EXPECT_EQ(variable_item->GetJsonValue(), expected_value);

  EXPECT_EQ(builder.FindVariableItemIdentifier(local_variable_ptr), variable_item->GetIdentifier());
  EXPECT_EQ(builder.FindVariableItemIdentifier(local_variable_ptr->GetName()),
            variable_item->GetIdentifier());
}
