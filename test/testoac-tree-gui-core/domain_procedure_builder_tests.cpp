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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/transform/domain_procedure_builder.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/sequencer_types_fwd.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/procedure_preamble_items.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/domain_workspace_builder.h>

#include <mvvm/model/session_item.h>

#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/procedure.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests of DomainProcedureBuilder class.
 */
class DomainProcedureBuilderTest : public ::testing::Test
{
};

TEST_F(DomainProcedureBuilderTest, EmptyProcedure)
{
  ProcedureItem procedure_item;
  procedure_item.SetFileName("aaa.xml");

  DomainProcedureBuilder builder;
  auto procedure = builder.CreateProcedure(procedure_item);

  EXPECT_EQ(procedure->GetFilename(), "aaa.xml");

  // Test constructed
  EXPECT_EQ(procedure->GetStatus(), ::sup::oac_tree::ExecutionStatus::SUCCESS);

  // Empty instructions and variables
  EXPECT_EQ(procedure->RootInstruction(), nullptr);
  auto instructions = procedure->GetTopInstructions();
  EXPECT_EQ(instructions.size(), 0);
  EXPECT_EQ(procedure->GetInstructionCount(), 0);
  EXPECT_TRUE(procedure->VariableNames().empty());
}

TEST_F(DomainProcedureBuilderTest, ProcedureWithPreamble)
{
  const ProcedureItem procedure_item;
  procedure_item.GetPreambleItem()->AddPluginPath("plugin_path");
  procedure_item.GetPreambleItem()->AddTypeRegistration(1, "json_type");

  DomainProcedureBuilder builder;
  auto procedure = builder.CreateProcedure(procedure_item);

  const std::vector<std::string> expected_paths{"plugin_path"};
  EXPECT_EQ(procedure->GetPreamble().GetPluginPaths(), expected_paths);
  ASSERT_EQ(procedure->GetPreamble().GetTypeRegistrations().size(), 1);
  EXPECT_EQ(procedure->GetPreamble().GetTypeRegistrations().at(0).GetRegistrationMode(), 1);
  EXPECT_EQ(procedure->GetPreamble().GetTypeRegistrations().at(0).GetString(), "json_type");
}

TEST_F(DomainProcedureBuilderTest, ProcedureWithSingleInstruction)
{
  const ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto sequence_item = container->InsertItem<SequenceItem>(mvvm::TagIndex::Append());

  DomainProcedureBuilder builder;
  auto procedure = builder.CreateProcedure(procedure_item);

  // Empty instruction list
  EXPECT_TRUE(procedure->RootInstruction() != nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 1);
  ASSERT_EQ(procedure->GetTopInstructions().at(0)->GetType(),
            domainconstants::kSequenceInstructionType);
  EXPECT_TRUE(procedure->VariableNames().empty());
}

TEST_F(DomainProcedureBuilderTest, ProcedureWithTwoInstructions)
{
  const ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto wait0 = container->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  wait0->SetTimeout(0.1);
  auto wait1 = container->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  wait1->SetTimeout(0.2);

  DomainProcedureBuilder builder;
  auto procedure = builder.CreateProcedure(procedure_item);

  // Empty instruction list
  EXPECT_EQ(procedure->RootInstruction(), nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 2);
  auto instructions = procedure->GetTopInstructions();
  EXPECT_EQ(instructions.at(0)->GetType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(instructions.at(1)->GetType(), domainconstants::kWaitInstructionType);

  EXPECT_EQ(instructions.at(0)->GetAttributeString(domainconstants::kTimeoutAttribute), "0.1");
  EXPECT_EQ(instructions.at(1)->GetAttributeString(domainconstants::kTimeoutAttribute), "0.2");
}

TEST_F(DomainProcedureBuilderTest, ProcedureWithParentAndChild)
{
  const ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto sequence = container->InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  auto wait = sequence->InsertItem<WaitItem>(mvvm::TagIndex::Append());

  DomainProcedureBuilder builder;
  auto procedure = builder.CreateProcedure(procedure_item);

  // Empty instruction list
  EXPECT_TRUE(procedure->RootInstruction() != nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 1);
  auto domain_sequence = procedure->GetTopInstructions().at(0);
  EXPECT_EQ(domain_sequence->GetType(), domainconstants::kSequenceInstructionType);
  ASSERT_EQ(domain_sequence->ChildrenCount(), 1);
  EXPECT_EQ(domain_sequence->ChildInstructions().at(0)->GetType(),
            domainconstants::kWaitInstructionType);
}

TEST_F(DomainProcedureBuilderTest, BuildProcedureWithParentAndChild)
{
  ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto sequence = container->InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  auto wait = sequence->InsertItem<WaitItem>(mvvm::TagIndex::Append());

  auto procedure = std::make_unique<procedure_t>();
  DomainProcedureBuilder builder;
  builder.PopulateProcedure(procedure_item, *procedure);

  EXPECT_TRUE(procedure->RootInstruction() != nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 1);
  auto domain_sequence = procedure->GetTopInstructions().at(0);

  EXPECT_EQ(domain_sequence->GetType(), domainconstants::kSequenceInstructionType);
  ASSERT_EQ(domain_sequence->ChildrenCount(), 1);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain_sequence), sequence->GetIdentifier());

  auto domain_wait = domain_sequence->ChildInstructions().at(0);
  EXPECT_EQ(domain_wait->GetType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain_wait), wait->GetIdentifier());
}

TEST_F(DomainProcedureBuilderTest, InverterWithSequence)
{
  const ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto inverter = InsertInstruction(domainconstants::kInverterInstructionType, container);

  auto sequence = inverter->InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  EXPECT_EQ(inverter->GetItem<SequenceItem>({"", 0}), sequence);
  auto wait = sequence->InsertItem<WaitItem>(mvvm::TagIndex::Append());

  auto procedure = std::make_unique<procedure_t>();
  DomainProcedureBuilder builder;
  builder.PopulateProcedure(procedure_item, *procedure);

  ASSERT_TRUE(procedure != nullptr);

  // Empty instruction list
  EXPECT_TRUE(procedure->RootInstruction() != nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 1);
  auto domain_inverter = procedure->GetTopInstructions().at(0);

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

TEST_F(DomainProcedureBuilderTest, RepeatWithSingleInstruction)
{
  const ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto repeater = container->InsertItem<RepeatItem>(mvvm::TagIndex::Append());
  auto sequence = repeater->InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  EXPECT_EQ(repeater->GetItem<SequenceItem>({"", 0}), sequence);

  auto procedure = std::make_unique<procedure_t>();
  DomainProcedureBuilder builder;
  builder.PopulateProcedure(procedure_item, *procedure);

  ASSERT_TRUE(procedure != nullptr);

  // Empty instruction list
  EXPECT_TRUE(procedure->RootInstruction() != nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 1);
  auto domain_repeat = procedure->GetTopInstructions().at(0);

  EXPECT_EQ(domain_repeat->GetType(), domainconstants::kRepeatInstructionType);
  ASSERT_EQ(domain_repeat->ChildrenCount(), 1);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain_repeat), repeater->GetIdentifier());

  auto domain_sequence = domain_repeat->ChildInstructions().at(0);
  EXPECT_EQ(domain_sequence->GetType(), domainconstants::kSequenceInstructionType);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain_sequence), sequence->GetIdentifier());
}

TEST_F(DomainProcedureBuilderTest, ProcedureWithVariable)
{
  const ProcedureItem procedure_item;
  auto workspace = procedure_item.GetWorkspace();

  auto var_item0 = workspace->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var_item0->SetName("var0");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, *var_item0);

  auto var_item1 = workspace->InsertItem<FileVariableItem>(mvvm::TagIndex::Append());
  var_item1->SetName("var1");

  auto procedure = std::make_unique<procedure_t>();
  DomainProcedureBuilder builder;
  builder.PopulateProcedure(procedure_item, *procedure);

  EXPECT_EQ(procedure->VariableNames(), std::vector<std::string>({"var0", "var1"}));

  auto domain_var0 = procedure->GetWorkspace().GetVariable("var0");
  auto domain_var1 = procedure->GetWorkspace().GetVariable("var1");
  EXPECT_EQ(domain_var0->GetName(), var_item0->GetName());
  EXPECT_EQ(domain_var1->GetName(), var_item1->GetName());
  EXPECT_EQ(builder.FindVariableItemIdentifier(domain_var0), var_item0->GetIdentifier());
  EXPECT_EQ(builder.FindVariableItemIdentifier(domain_var1), var_item1->GetIdentifier());
}

TEST_F(DomainProcedureBuilderTest, ProcedureWithParallelSequence)
{
  const ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto sequence = container->InsertItem<ParallelSequenceItem>(mvvm::TagIndex::Append());
  auto wait0 = sequence->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  auto wait1 = sequence->InsertItem<WaitItem>(mvvm::TagIndex::Append());

  DomainProcedureBuilder builder;
  auto procedure = builder.CreateProcedure(procedure_item);

  // Empty instruction list
  EXPECT_TRUE(procedure->RootInstruction() != nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 1);
  auto domain_sequence = procedure->GetTopInstructions().at(0);
  EXPECT_EQ(domain_sequence->GetType(), domainconstants::kParallelInstructionType);
  ASSERT_EQ(domain_sequence->ChildrenCount(), 2);
  EXPECT_EQ(domain_sequence->ChildInstructions().at(0)->GetType(),
            domainconstants::kWaitInstructionType);
  EXPECT_EQ(domain_sequence->ChildInstructions().at(1)->GetType(),
            domainconstants::kWaitInstructionType);
}

TEST_F(DomainProcedureBuilderTest, SequenceWIthFourChildrenAndAdjustedBehavior)
{
  const ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto sequence = container->InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  auto wait0 = sequence->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  wait0->SetBehavior(itemconstants::kFailBehavior);
  auto wait1 = sequence->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  wait1->SetBehavior(itemconstants::kSucceedBehavior);
  auto wait2 = sequence->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  wait2->SetBehavior(itemconstants::kHiddenBehavior);
  auto wait3 = sequence->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  wait3->SetBehavior(itemconstants::kNativeBehavior);

  auto procedure = std::make_unique<procedure_t>();
  DomainProcedureBuilder builder;
  builder.PopulateProcedure(procedure_item, *procedure);

  EXPECT_TRUE(procedure->RootInstruction() != nullptr);
  ASSERT_EQ(procedure->GetInstructionCount(), 1);
  auto domain_sequence = procedure->GetTopInstructions().at(0);

  EXPECT_EQ(domain_sequence->GetType(), domainconstants::kSequenceInstructionType);
  ASSERT_EQ(domain_sequence->ChildrenCount(), 3);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain_sequence), sequence->GetIdentifier());

  // counterpart of wait0 is Fail instruction
  auto domain0 = domain_sequence->ChildInstructions().at(0);
  EXPECT_EQ(domain0->GetType(), domainconstants::kFailedInstructionType);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain0), wait0->GetIdentifier());

  // counterpart of wait1   is Succeed instruction
  auto domain1 = domain_sequence->ChildInstructions().at(1);
  EXPECT_EQ(domain1->GetType(), domainconstants::kSucceedInstructionType);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain1), wait1->GetIdentifier());

  // wait2 wasn't propagated to the domain

  // counterpart of wait3
  auto domain2 = domain_sequence->ChildInstructions().at(2);
  EXPECT_EQ(domain2->GetType(), domainconstants::kWaitInstructionType);
  EXPECT_EQ(builder.FindInstructionIdentifier(domain2), wait3->GetIdentifier());
}

}  // namespace oac_tree_gui::test
