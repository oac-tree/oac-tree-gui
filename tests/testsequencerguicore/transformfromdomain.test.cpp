/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
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

#include "sequencergui/model/transformfromdomain.h"

#include "Instruction.h"
#include "Procedure.h"
#include "Variable.h"
#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/sequenceritems.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for utility functions related to the domain to presentation transformations.

class TransformFromDomainTest : public ::testing::Test
{
public:
  //! Returns true if given Instruction can be casted to specified type.
  template <typename T>
  bool IsCorrectType(const mvvm::SessionItem* item)
  {
    return dynamic_cast<const T*>(item) != nullptr;
  }

  //! Returns true if we can create InstructionItem of proper type for string representing
  //! "Type" of sup::sequencer::Instruction.
  template <typename T>
  bool CanCreateInstructionForType(const std::string& seq_type)
  {
    return IsCorrectType<T>(sequencergui::CreateInstructionItem(seq_type).get());
  }

  //! Returns true if we can create VariableItem of proper type for string representing
  //! "Type" of sup::sequencer::Variable.
  template <typename T>
  bool CanCreateVariableForType(const std::string& seq_type)
  {
    return IsCorrectType<T>(sequencergui::CreateVariableItem(seq_type).get());
  }
};

//! Validate CreateInstructionItem factory function.

TEST_F(TransformFromDomainTest, CreateVariableItem)
{
  EXPECT_TRUE(CanCreateVariableForType<sequencergui::LocalVariableItem>(
      DomainConstants::kLocalVariableType));
  EXPECT_TRUE(
      CanCreateVariableForType<sequencergui::FileVariableItem>(DomainConstants::kFileVariableType));
  if (DomainUtils::IsChannelAccessAvailable())
  {
    EXPECT_TRUE(CanCreateVariableForType<sequencergui::ChannelAccessVariableItem>(
        DomainConstants::kChannelAccessVariableType));
    EXPECT_TRUE(CanCreateVariableForType<sequencergui::PVClientVariableItem>(
        DomainConstants::kPVClientVariableType));
  }
}

//! Validate CreateInstructionItem factory function.

TEST_F(TransformFromDomainTest, CreateInstructionItem)
{
  EXPECT_TRUE(
      CanCreateInstructionForType<sequencergui::CopyItem>(DomainConstants::kCopyInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::EqualsItem>(
      DomainConstants::kEqualsInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::FallbackItem>(
      DomainConstants::kFallbackInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequencergui::InputItem>(DomainConstants::kInputInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::InverterItem>(
      DomainConstants::kInverterInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::MessageItem>(
      DomainConstants::kMessageInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::OutputItem>(
      DomainConstants::kOutputInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::ParallelSequenceItem>(
      DomainConstants::kParallelInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::RepeatItem>(
      DomainConstants::kRepeatInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::SequenceItem>(
      DomainConstants::kSequenceInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequencergui::WaitItem>(DomainConstants::kWaitInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::ForceSuccessItem>(
      DomainConstants::kForceSuccessInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::UserChoiceItem>(
      DomainConstants::kUserChoiceInstructionType));
}

//! Populate InstructionContainerItem from empty Procedure.

TEST_F(TransformFromDomainTest, PopulateItemContainerFromEmptyProcedure)
{
  ::sup::sequencer::Procedure procedure;

  sequencergui::InstructionContainerItem container;
  sequencergui::PopulateInstructionContainerItem(&procedure, &container);

  EXPECT_EQ(container.GetTotalItemCount(), 0);
}

//! Populate InstructionContainerItem from Procedure with a single Wait instruction.

TEST_F(TransformFromDomainTest, PopulateItemContainerFromProcedureWithWait)
{
  ::sup::sequencer::Procedure procedure;

  auto wait = DomainUtils::CreateDomainInstruction(DomainConstants::kWaitInstructionType);
  wait->AddAttribute(sequencergui::DomainConstants::kWaitTimeoutAttribute, "42");
  procedure.PushInstruction(wait.release());

  sequencergui::InstructionContainerItem container;
  sequencergui::PopulateInstructionContainerItem(&procedure, &container);

  auto item = container.GetItem<sequencergui::WaitItem>("");
  EXPECT_EQ(item->GetTimeout(), 42.0);
}

//! Populate InstructionContainerItem from Procedure with a Sequence containing Wait instruction.

TEST_F(TransformFromDomainTest, PopulateItemContainerFromProcedureWithSequence)
{
  ::sup::sequencer::Procedure procedure;

  auto wait = DomainUtils::CreateDomainInstruction(DomainConstants::kWaitInstructionType);
  wait->AddAttribute(sequencergui::DomainConstants::kWaitTimeoutAttribute, "42");

  auto sequence = DomainUtils::CreateDomainInstruction(DomainConstants::kSequenceInstructionType);
  sequence->InsertInstruction(wait.release(), 0);

  procedure.PushInstruction(sequence.release());

  sequencergui::InstructionContainerItem container;
  sequencergui::PopulateInstructionContainerItem(&procedure, &container);

  auto sequence_item = container.GetItem<sequencergui::SequenceItem>("");
  auto wait_item = sequence_item->GetItem<sequencergui::WaitItem>("");
  EXPECT_EQ(wait_item->GetTimeout(), 42.0);
}

//! Populate WorkspaceItem from empty procedure.

TEST_F(TransformFromDomainTest, PopulateWorkspaceItemFromEmtpyProcedure)
{
  ::sup::sequencer::Procedure procedure;

  sequencergui::WorkspaceItem workspace_item;
  sequencergui::PopulateWorkspaceItem(&procedure, &workspace_item);
  EXPECT_EQ(workspace_item.GetTotalItemCount(), 0);
}

//! Populate WorkspaceItem from empty procedure.

TEST_F(TransformFromDomainTest, PopulateWorkspaceItemFromProcedureWithLocalVariable)
{
  ::sup::sequencer::Procedure procedure;

  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  auto local_variable = DomainUtils::CreateDomainVariable(DomainConstants::kLocalVariableType);
  local_variable->AddAttribute(DomainConstants::kTypeAttribute, expected_type);
  local_variable->AddAttribute(DomainConstants::kValueAttribute, expected_value);

  procedure.AddVariable("abc", local_variable.release());

  sequencergui::WorkspaceItem workspace_item;
  sequencergui::PopulateWorkspaceItem(&procedure, &workspace_item);
  EXPECT_EQ(workspace_item.GetTotalItemCount(), 1);

  auto variable_item = workspace_item.GetItem<sequencergui::LocalVariableItem>("");
  EXPECT_EQ(variable_item->GetJsonType(), expected_type);
  EXPECT_EQ(variable_item->GetJsonValue(), expected_value);
}
