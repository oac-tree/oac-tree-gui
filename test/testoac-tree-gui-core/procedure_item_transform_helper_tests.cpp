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

#include "oac_tree_gui/transform/procedure_item_transform_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/procedure_preamble_items.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/transform_from_domain.h>

#include <sup/gui/model/anyvalue_conversion_utils.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/procedure.h>
#include <sup/oac-tree/procedure_preamble.h>
#include <sup/oac-tree/variable.h>

#include <gtest/gtest.h>
#include <testutils/standard_procedures.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests forhelper methods from procedure_item_transform_helper.h.
 */
class ProcedureItemTransformHelperTest : public ::testing::Test
{
};

//! Populate InstructionContainerItem from empty Procedure.
TEST_F(ProcedureItemTransformHelperTest, PopulateItemFromEmptyProcedure)
{
  const ::sup::oac_tree::Procedure procedure;

  auto procedure_item = CreateProcedureItem(procedure);

  EXPECT_EQ(procedure_item->GetInstructionContainer()->GetTotalItemCount(), 0);
  EXPECT_EQ(procedure_item->GetWorkspace()->GetTotalItemCount(), 0);
}

//! Populate InstructionContainerItem from Procedure with two Wait instruction.
TEST_F(ProcedureItemTransformHelperTest, PopulateItemFromProcedureWithTwoWaits)
{
  ::sup::oac_tree::Procedure procedure;

  auto wait0 = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait0->AddAttribute(domainconstants::kTimeoutAttribute, "42");
  procedure.PushInstruction(std::move(wait0));

  auto wait1 = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait1->AddAttribute(domainconstants::kIsRootAttribute, "true");
  wait1->AddAttribute(domainconstants::kTimeoutAttribute, "43");
  procedure.PushInstruction(std::move(wait1));

  auto procedure_item = CreateProcedureItem(procedure);
  auto container = procedure_item->GetInstructionContainer();

  ASSERT_EQ(container->GetInstructionCount(), 2);

  auto wait_item0 = container->GetItem<WaitItem>(mvvm::TagIndex::Default(0));
  EXPECT_EQ(wait_item0->GetTimeout(), 42.0);
  EXPECT_FALSE(wait_item0->IsRoot());

  auto wait_item1 = container->GetItem<WaitItem>(mvvm::TagIndex::Default(1));
  EXPECT_EQ(wait_item1->GetTimeout(), 43.0);
  EXPECT_TRUE(wait_item1->IsRoot());
}

//! Populate InstructionContainerItem from Procedure with a Sequence containing Wait instruction.
TEST_F(ProcedureItemTransformHelperTest, PopulateItemFromProcedureWithSequence)
{
  ::sup::oac_tree::Procedure procedure;

  auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  auto wait_ptr = wait.get();
  wait->AddAttribute(oac_tree_gui::domainconstants::kTimeoutAttribute, "42");

  auto sequence = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto sequence_ptr = sequence.get();
  sequence->InsertInstruction(std::move(wait), 0);

  procedure.PushInstruction(std::move(sequence));
  procedure.Setup();

  auto procedure_item = CreateProcedureItem(procedure);

  auto sequence_item =
      procedure_item->GetInstructionContainer()->GetItem<oac_tree_gui::SequenceItem>("");
  auto wait_item = sequence_item->GetItem<oac_tree_gui::WaitItem>("");
  EXPECT_EQ(wait_item->GetTimeout(), 42.0);
}

TEST_F(ProcedureItemTransformHelperTest, PopulateItemFromProcedureWithLocalVariable)
{
  ::sup::oac_tree::Procedure procedure;

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

  auto procedure_item = CreateProcedureItem(procedure);

  EXPECT_EQ(procedure_item->GetWorkspace()->GetTotalItemCount(), 1);

  auto variable_item = procedure_item->GetWorkspace()->GetItem<oac_tree_gui::LocalVariableItem>("");
  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);
  EXPECT_EQ(GetAnyValue(*variable_item), expected_anyvalue);
}

//! Procedure with local include.
TEST_F(ProcedureItemTransformHelperTest, LocalIncludeProcedure)
{
  auto procedure = test::CreateLocalIncludeProcedure();

  ASSERT_EQ(procedure->GetTopInstructions().size(), 2);
  auto sequence_ptr = procedure->GetTopInstructions().at(0);

  ASSERT_EQ(sequence_ptr->ChildInstructions().size(), 1);
  auto wait_ptr = sequence_ptr->ChildInstructions().at(0);

  auto repeat_ptr = procedure->GetTopInstructions().at(1);
  auto include_ptr = repeat_ptr->ChildInstructions().at(0);

  EXPECT_EQ(procedure->RootInstruction(), repeat_ptr);
  EXPECT_EQ(repeat_ptr->ChildInstructions().at(0), include_ptr);
  EXPECT_EQ(include_ptr->ChildInstructions().size(), 0);  // since no Setup has been called

  auto procedure_item = CreateProcedureItem(*procedure);

  EXPECT_EQ(procedure_item->GetInstructionContainer()->GetTotalItemCount(), 2);

  auto repeat_item = procedure_item->GetInstructionContainer()->GetItem<oac_tree_gui::RepeatItem>(
      mvvm::TagIndex::Default(1));
  auto include_item = repeat_item->GetItem<IncludeItem>(mvvm::TagIndex::Default(0));
  auto sequence_item =
      procedure_item->GetInstructionContainer()->GetItem<SequenceItem>(mvvm::TagIndex::Default(0));
  auto wait_item = sequence_item->GetItem<oac_tree_gui::WaitItem>(mvvm::TagIndex::Default(0));
  // include instruction doesn't have children yet (since no Setup has been called)
  EXPECT_EQ(include_item->GetInstructions().size(), 0);
}

TEST_F(ProcedureItemTransformHelperTest, PopulateProcedurePreambleItem)
{
  {  // empty
    const sup::oac_tree::ProcedurePreamble preamble;
    ProcedurePreambleItem item;
    PopulateProcedurePreambleItem(preamble, item);

    EXPECT_TRUE(preamble.GetPluginPaths().empty());
    EXPECT_TRUE(preamble.GetTypeRegistrations().empty());
  }

  {  // preamble
    using sup::oac_tree::TypeRegistrationInfo;

    sup::oac_tree::ProcedurePreamble preamble;
    preamble.AddPluginPath("abc");
    preamble.AddPluginPath("def");
    preamble.AddTypeRegistration(TypeRegistrationInfo(TypeRegistrationInfo::kJSONFile, "a1"));
    preamble.AddTypeRegistration(TypeRegistrationInfo(TypeRegistrationInfo::kJSONString, "a2"));

    ProcedurePreambleItem item;
    PopulateProcedurePreambleItem(preamble, item);

    const std::vector<std::string> expected_paths{"abc", "def"};
    const std::vector<std::pair<int, std::string> > expected_info = {{0, "a1"}, {1, "a2"}};

    EXPECT_EQ(item.GetPluginPaths(), expected_paths);
    EXPECT_EQ(item.GetTypeRegistrations(), expected_info);
  }

  {  // attempt to add in non-empty preamble
    using sup::oac_tree::TypeRegistrationInfo;

    sup::oac_tree::ProcedurePreamble preamble;
    preamble.AddPluginPath("abc");

    ProcedurePreambleItem item;
    item.AddPluginPath("aaa");
    EXPECT_THROW(PopulateProcedurePreambleItem(preamble, item), LogicErrorException);
  }
}

//! Populate ProcedureItem from the domain procedure containing preamble.
TEST_F(ProcedureItemTransformHelperTest, PopulateItemFromProcedureWithPreamble)
{
  using sup::oac_tree::TypeRegistrationInfo;

  ::sup::oac_tree::Procedure procedure;

  const std::string json_type =
      R"RAW({"type":"ranges_uint32","multiplicity":3,"element":{"type":"uint32"}})RAW";

  procedure.GetPreamble().AddPluginPath("abc");
  procedure.GetPreamble().AddPluginPath("def");
  procedure.GetPreamble().AddTypeRegistration(
      TypeRegistrationInfo(TypeRegistrationInfo::kJSONString, json_type));

  auto procedure_item = CreateProcedureItem(procedure);

  const std::vector<std::string> expected_paths{"abc", "def"};
  const std::vector<std::pair<int, std::string> > expected_info = {{1, json_type}};

  EXPECT_EQ(procedure_item->GetPreambleItem()->GetPluginPaths(), expected_paths);
  EXPECT_EQ(procedure_item->GetPreambleItem()->GetTypeRegistrations(), expected_info);
}

}  // namespace oac_tree_gui::test
