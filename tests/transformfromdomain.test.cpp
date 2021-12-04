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

#include "sequencergui/model/transformfromdomain.h"

#include "Instruction.h"
#include "InstructionRegistry.h"
#include "Procedure.h"
#include "Variable.h"
#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/sequenceritems.h"

#include <gtest/gtest.h>

using namespace sequi;

//! Tests for utility functions related to the domain to presentation transformations.

class TransformFromDomainTest : public ::testing::Test
{
public:
  //! Returns true if given Instruction can be casted to specified type.
  template <typename T>
  bool IsCorrectType(const ModelView::SessionItem* item)
  {
    return dynamic_cast<const T*>(item) != nullptr;
  }

  //! Returns true if we can create InstructionItem of proper type for string representing
  //! "Type" of sup::sequencer::Instruction.
  template <typename T>
  bool CanCreateInstructionForType(const std::string& seq_type)
  {
    return IsCorrectType<T>(sequi::CreateInstructionItem(seq_type).get());
  }

  //! Returns true if we can create VariableItem of proper type for string representing
  //! "Type" of sup::sequencer::Variable.
  template <typename T>
  bool CanCreateVariableForType(const std::string& seq_type)
  {
    return IsCorrectType<T>(sequi::CreateVariableItem(seq_type).get());
  }
};

//! Validate CreateInstructionItem factory function.

TEST_F(TransformFromDomainTest, CreateVariableItem)
{
  EXPECT_TRUE(
      CanCreateVariableForType<sequi::LocalVariableItem>(DomainConstants::kLocalVariableType));
  EXPECT_TRUE(
      CanCreateVariableForType<sequi::FileVariableItem>(DomainConstants::kFileVariableType));
  if (DomainUtils::IsChannelAccessAvailable())
  {
    EXPECT_TRUE(CanCreateVariableForType<sequi::ChannelAccessVariableItem>(
        DomainConstants::kChannelAccessVariableType));
  }
}

//! Validate CreateInstructionItem factory function.

TEST_F(TransformFromDomainTest, CreateInstructionItem)
{
  EXPECT_TRUE(CanCreateInstructionForType<sequi::CopyItem>(DomainConstants::kCopyInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequi::FallbackItem>(DomainConstants::kFallbackInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequi::InputItem>(DomainConstants::kInputInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequi::InverterItem>(DomainConstants::kInverterInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequi::MessageItem>(DomainConstants::kMessageInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequi::OutputItem>(DomainConstants::kOutputInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequi::ParallelSequenceItem>(
      DomainConstants::kParallelInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequi::RepeatItem>(DomainConstants::kRepeatInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequi::SequenceItem>(DomainConstants::kSequenceInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequi::WaitItem>(DomainConstants::kWaitInstructionType));
}

//! Populate InstructionContainerItem from empty Procedure.

TEST_F(TransformFromDomainTest, PopulateItemContainerFromEmptyProcedure)
{
  ::sup::sequencer::Procedure procedure;

  sequi::InstructionContainerItem container;
  sequi::PopulateInstructionContainerItem(&procedure, &container);

  EXPECT_EQ(container.GetTotalItemCount(), 0);
}

//! Populate InstructionContainerItem from Procedure with a single Wait instruction.

TEST_F(TransformFromDomainTest, PopulateItemContainerFromProcedureWithWait)
{
  ::sup::sequencer::Procedure procedure;

  auto wait =
      ::sup::sequencer::GlobalInstructionRegistry().Create(DomainConstants::kWaitInstructionType);
  wait->AddAttribute(sequi::DomainConstants::kWaitTimeoutAttribute, "42");
  procedure.PushInstruction(wait.release());

  sequi::InstructionContainerItem container;
  sequi::PopulateInstructionContainerItem(&procedure, &container);

  auto item = container.GetItem<sequi::WaitItem>("");
  EXPECT_EQ(item->GetTimeout(), 42.0);
}

//! Populate InstructionContainerItem from Procedure with a Sequence containing Wait instruction.

TEST_F(TransformFromDomainTest, PopulateItemContainerFromProcedureWithSequence)
{
  ::sup::sequencer::Procedure procedure;

  auto wait =
      ::sup::sequencer::GlobalInstructionRegistry().Create(DomainConstants::kWaitInstructionType);
  wait->AddAttribute(sequi::DomainConstants::kWaitTimeoutAttribute, "42");

  auto sequence = DomainUtils::CreateDomainInstruction(DomainConstants::kSequenceInstructionType);
  sequence->InsertInstruction(wait.release(), 0);

  procedure.PushInstruction(sequence.release());

  sequi::InstructionContainerItem container;
  sequi::PopulateInstructionContainerItem(&procedure, &container);

  auto sequence_item = container.GetItem<sequi::SequenceItem>("");
  auto wait_item = sequence_item->GetItem<sequi::WaitItem>("");
  EXPECT_EQ(wait_item->GetTimeout(), 42.0);
}

//! Populate WorkspaceItem from empty procedure.

TEST_F(TransformFromDomainTest, PopulateWorkspaceItemFromEmtpyProcedure)
{
  ::sup::sequencer::Procedure procedure;

  sequi::WorkspaceItem workspace_item;
  sequi::PopulateWorkspaceItem(&procedure, &workspace_item);
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

  sequi::WorkspaceItem workspace_item;
  sequi::PopulateWorkspaceItem(&procedure, &workspace_item);
  EXPECT_EQ(workspace_item.GetTotalItemCount(), 1);

  auto variable_item = workspace_item.GetItem<sequi::LocalVariableItem>("");
  EXPECT_EQ(variable_item->GetJsonType(), expected_type);
  EXPECT_EQ(variable_item->GetJsonValue(), expected_value);
}
