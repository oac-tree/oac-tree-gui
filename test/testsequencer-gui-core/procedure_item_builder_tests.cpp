/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/transform/procedure_item_builder.h"

#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/procedure_preamble_items.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/transform_from_domain.h>
#include <sequencergui/transform/anyvalue_item_transform_helper.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/procedure_preamble.h>
#include <sup/sequencer/variable.h>

#include <gtest/gtest.h>
#include <testutils/standard_procedures.h>

using namespace sequencergui;

//! Tests for utility functions related to the domain to presentation transformations.

class ProcedureItemBuilderTest : public ::testing::Test
{
};

//! Populate InstructionContainerItem from empty Procedure.

TEST_F(ProcedureItemBuilderTest, PopulateItemFromEmptyProcedure)
{
  ::sup::sequencer::Procedure procedure;
  sequencergui::ProcedureItem procedure_item;

  ProcedureItemBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item, /*root_only*/ false);

  EXPECT_EQ(procedure_item.GetInstructionContainer()->GetTotalItemCount(), 0);
  EXPECT_EQ(procedure_item.GetWorkspace()->GetTotalItemCount(), 0);
}

//! Populate InstructionContainerItem from Procedure with a single Wait instruction.

TEST_F(ProcedureItemBuilderTest, PopulateItemFromProcedureWithWait)
{
  ::sup::sequencer::Procedure procedure;

  auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait->AddAttribute(sequencergui::domainconstants::kTimeoutAttribute, "42");
  auto wait_ptr = wait.get();
  procedure.PushInstruction(std::move(wait));

  sequencergui::ProcedureItem procedure_item;
  ProcedureItemBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item, /*root_only*/ false);

  auto item = procedure_item.GetInstructionContainer()->GetItem<sequencergui::WaitItem>("");
  EXPECT_EQ(item->GetTimeout(), 42.0);
  EXPECT_EQ(builder.FindInstructionItem(wait_ptr), item);
}

//! Populate InstructionContainerItem from Procedure with two Wait instruction.
//! One is marked as root instruction, root_only=true is used

TEST_F(ProcedureItemBuilderTest, PopulateItemFromProcedureWithTwoWaits)
{
  ::sup::sequencer::Procedure procedure;

  auto wait0 = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait0->AddAttribute(sequencergui::domainconstants::kTimeoutAttribute, "42");
  procedure.PushInstruction(std::move(wait0));

  auto wait1 = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait1->AddAttribute(sequencergui::domainconstants::kIsRootAttribute, "true");
  wait1->AddAttribute(sequencergui::domainconstants::kTimeoutAttribute, "43");
  procedure.PushInstruction(std::move(wait1));

  sequencergui::ProcedureItem procedure_item;
  ProcedureItemBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item, /*root_only*/ true);

  EXPECT_EQ(procedure_item.GetInstructionContainer()->GetInstructionCount(), 1);

  auto item = procedure_item.GetInstructionContainer()->GetItem<sequencergui::WaitItem>("");
  EXPECT_EQ(item->GetTimeout(), 43.0);
}

//! Populate InstructionContainerItem from Procedure with a Sequence containing Wait instruction.

TEST_F(ProcedureItemBuilderTest, PopulateItemFromProcedureWithSequence)
{
  ::sup::sequencer::Procedure procedure;

  auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  auto wait_ptr = wait.get();
  wait->AddAttribute(sequencergui::domainconstants::kTimeoutAttribute, "42");

  auto sequence = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto sequence_ptr = sequence.get();
  sequence->InsertInstruction(std::move(wait), 0);

  procedure.PushInstruction(std::move(sequence));
  procedure.Setup();

  sequencergui::ProcedureItem procedure_item;
  ProcedureItemBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item, /*root_only*/ false);

  auto sequence_item =
      procedure_item.GetInstructionContainer()->GetItem<sequencergui::SequenceItem>("");
  auto wait_item = sequence_item->GetItem<sequencergui::WaitItem>("");
  EXPECT_EQ(wait_item->GetTimeout(), 42.0);

  EXPECT_EQ(builder.FindInstructionItem(wait_ptr), wait_item);
  EXPECT_EQ(builder.FindInstructionItem(sequence_ptr), sequence_item);
}

//! Populate WorkspaceItem from empty procedure.

TEST_F(ProcedureItemBuilderTest, PopulateItemFromProcedureWithLocalVariable)
{
  ::sup::sequencer::Procedure procedure;

  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  auto local_variable = CreateDomainVariable(domainconstants::kLocalVariableType);
  local_variable->AddAttribute(domainconstants::kTypeAttribute, expected_type);
  local_variable->AddAttribute(domainconstants::kValueAttribute, expected_value);
  auto local_variable_ptr = local_variable.get();

  procedure.AddVariable("abc", std::move(local_variable));

  // For this test we do not need an instruction, but we have to add something to let
  // Proceture::Setup passing.
  auto message = CreateDomainInstruction(domainconstants::kMessageInstructionType);
  message->AddAttribute(domainconstants::kTextAttribute, "hello world");
  procedure.PushInstruction(std::move(message));

  procedure.Setup();

  sequencergui::ProcedureItem procedure_item;
  ProcedureItemBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item, /*root_only*/ false);

  EXPECT_EQ(procedure_item.GetWorkspace()->GetTotalItemCount(), 1);

  auto variable_item = procedure_item.GetWorkspace()->GetItem<sequencergui::LocalVariableItem>("");
  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  EXPECT_EQ(GetAnyValue(*variable_item), expected_anyvalue);

  EXPECT_EQ(builder.GetVariable(0), variable_item);

  EXPECT_EQ(builder.GetVariable(0), variable_item);
  EXPECT_EQ(builder.GetVariable(1), nullptr);
}

//! Procedure with local include after Setup call.
//! root_only mode is used.

TEST_F(ProcedureItemBuilderTest, LocalIncludeProcedure)
{
  auto procedure = testutils::CreateLocalIncludeProcedure();

  ASSERT_EQ(procedure->GetTopInstructions().size(), 2);
  auto sequence_ptr = procedure->GetTopInstructions().at(0);

  ASSERT_EQ(sequence_ptr->ChildInstructions().size(), 1);
  auto wait_ptr = sequence_ptr->ChildInstructions().at(0);

  auto repeat_ptr = procedure->GetTopInstructions().at(1);
  auto include_ptr = repeat_ptr->ChildInstructions().at(0);

  EXPECT_EQ(procedure->RootInstruction(), repeat_ptr);
  EXPECT_EQ(repeat_ptr->ChildInstructions().at(0), include_ptr);
  EXPECT_EQ(include_ptr->ChildInstructions().size(), 0);  // since no Setup has been called

  // Building ProcedureItem in root_only mode
  sequencergui::ProcedureItem procedure_item;
  ProcedureItemBuilder builder;
  builder.PopulateProcedureItem(procedure.get(), &procedure_item, /*root_only*/ false);

  // only one root instruction has been processed
  EXPECT_EQ(procedure_item.GetInstructionContainer()->GetTotalItemCount(), 2);

  auto repeat_item =
      procedure_item.GetInstructionContainer()->GetItem<sequencergui::RepeatItem>({"", 1});
  auto include_item = repeat_item->GetItem<sequencergui::IncludeItem>("");
  auto sequence_item =
      procedure_item.GetInstructionContainer()->GetItem<sequencergui::SequenceItem>({"", 0});
  auto wait_item = sequence_item->GetItem<sequencergui::WaitItem>("");

  // Repeat and Include instructions corresponds to their domain counterpart
  EXPECT_EQ(builder.FindInstructionItem(repeat_ptr), repeat_item);
  EXPECT_EQ(builder.FindInstructionItem(include_ptr), include_item);

  // include instruction doesn't have children yet (since no Setup has been called)
  EXPECT_EQ(include_item->GetInstructions().size(), 0);

  // SequenceItem and WaitItem remains on their place
  EXPECT_EQ(builder.FindInstructionItem(sequence_ptr), sequence_item);
  EXPECT_EQ(builder.FindInstructionItem(wait_ptr), wait_item);
}

//! Procedure with local include after Setup call.
//! root_only mode is used.

TEST_F(ProcedureItemBuilderTest, LocalIncludeAfterSetup)
{
  auto procedure = testutils::CreateLocalIncludeProcedure();

  ASSERT_EQ(procedure->GetTopInstructions().size(), 2);
  auto sequence_ptr = procedure->GetTopInstructions().at(0);

  ASSERT_EQ(sequence_ptr->ChildInstructions().size(), 1);
  auto wait_ptr = sequence_ptr->ChildInstructions().at(0);

  auto repeat_ptr = procedure->GetTopInstructions().at(1);
  auto include_ptr = repeat_ptr->ChildInstructions().at(0);

  procedure->Setup();

  // Building ProcedureItem in root_only mode
  sequencergui::ProcedureItem procedure_item;
  ProcedureItemBuilder builder;
  builder.PopulateProcedureItem(procedure.get(), &procedure_item, /*root_only*/ true);

  // only one root instruction has been processed
  EXPECT_EQ(procedure_item.GetInstructionContainer()->GetTotalItemCount(), 1);

  auto repeat_item =
      procedure_item.GetInstructionContainer()->GetItem<sequencergui::RepeatItem>("");
  auto include_item = repeat_item->GetItem<sequencergui::IncludeItem>("");
  auto sequence_item = include_item->GetItem<sequencergui::SequenceItem>("");
  auto wait_item = sequence_item->GetItem<sequencergui::WaitItem>("");

  // Repeat and Include instructions corresponds to their domain counterpart
  EXPECT_EQ(builder.FindInstructionItem(repeat_ptr), repeat_item);
  EXPECT_EQ(builder.FindInstructionItem(include_ptr), include_item);

  // However SequenceItem and WaitItem have been made from cloned versions of Sequence and Wait
  EXPECT_EQ(builder.FindInstructionItem(sequence_ptr), nullptr);
  EXPECT_EQ(builder.FindInstructionItem(wait_ptr), nullptr);

  // Domain clones should lead to constructed SequenceItem and WaitItem
  EXPECT_EQ(procedure->RootInstruction(), repeat_ptr);
  EXPECT_EQ(repeat_ptr->ChildInstructions().at(0), include_ptr);

  auto cloned_domain_sequence = include_ptr->ChildInstructions().at(0);
  auto cloned_domain_wait = cloned_domain_sequence->ChildInstructions().at(0);
  EXPECT_NE(cloned_domain_sequence, sequence_ptr);
  EXPECT_NE(cloned_domain_wait, wait_ptr);

  EXPECT_EQ(builder.FindInstructionItem(cloned_domain_sequence), sequence_item);
  EXPECT_EQ(builder.FindInstructionItem(cloned_domain_wait), wait_item);
}

TEST_F(ProcedureItemBuilderTest, FindInstruction)
{
  ProcedureItemBuilder builder;

  auto procedure = testutils::CreateSequenceWithSingleMessageProcedure();

  auto procedure_item = builder.CreateProcedureItem(procedure.get(), /*root only*/ false);

  auto sequence = procedure->GetTopInstructions().at(0);
  auto sequence_item = procedure_item->GetInstructionContainer()->GetInstructions().at(0);

  auto message = sequence->ChildInstructions().at(0);
  auto message_item = sequence_item->GetInstructions().at(0);

  EXPECT_EQ(builder.FindInstruction(sequence_item), sequence);
  EXPECT_EQ(builder.FindInstruction(message_item), message);

  WaitItem wait_item;
  EXPECT_EQ(builder.FindInstruction(&wait_item), nullptr);
}

//! Populate ProcedureItem from the domain procedure containing preamble.

TEST_F(ProcedureItemBuilderTest, PopulateItemFromProcedureWithPreamble)
{
  using sup::sequencer::TypeRegistrationInfo;

  ::sup::sequencer::Procedure procedure;

  const std::string json_type =
      R"RAW({"type":"ranges_uint32","multiplicity":3,"element":{"type":"uint32"}})RAW";

  procedure.GetPreamble().AddPluginPath("abc");
  procedure.GetPreamble().AddPluginPath("def");
  procedure.GetPreamble().AddTypeRegistration(
      TypeRegistrationInfo(TypeRegistrationInfo::kJSONString, json_type));

  sequencergui::ProcedureItem procedure_item;
  ProcedureItemBuilder builder;
  builder.PopulateProcedureItem(&procedure, &procedure_item, /*root_only*/ false);

  std::vector<std::string> expected_paths{"abc", "def"};
  std::vector<std::pair<int, std::string> > expected_info = {{1, json_type}};

  EXPECT_EQ(procedure_item.GetPreambleItem()->GetPluginPaths(), expected_paths);
  EXPECT_EQ(procedure_item.GetPreambleItem()->GetTypeRegistrations(), expected_info);
}
