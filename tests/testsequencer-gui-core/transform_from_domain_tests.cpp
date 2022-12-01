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

#include "sequencergui/model/transform_from_domain.h"

#include <gtest/gtest.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/variable.h>

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

TEST_F(TransformFromDomainTest, GetItemType)
{
  EXPECT_EQ(ConditionItem::Type, GetItemType(domainconstants::kConditionInstructionType));
  EXPECT_EQ(CopyItem::Type, GetItemType(domainconstants::kCopyInstructionType));
  EXPECT_EQ(EqualsItem::Type, GetItemType(domainconstants::kEqualsInstructionType));
  EXPECT_EQ(FallbackItem::Type, GetItemType(domainconstants::kFallbackInstructionType));
  EXPECT_EQ(ForceSuccessItem::Type, GetItemType(domainconstants::kForceSuccessInstructionType));
  EXPECT_EQ(IncludeItem::Type, GetItemType(domainconstants::kIncludeInstructionType));
  EXPECT_EQ(InputItem::Type, GetItemType(domainconstants::kInputInstructionType));
  EXPECT_EQ(InverterItem::Type, GetItemType(domainconstants::kInverterInstructionType));
  EXPECT_EQ(ListenItem::Type, GetItemType(domainconstants::kListenInstructionType));
  EXPECT_EQ(MessageItem::Type, GetItemType(domainconstants::kMessageInstructionType));
  EXPECT_EQ(OutputItem::Type, GetItemType(domainconstants::kOutputInstructionType));
  EXPECT_EQ(ParallelSequenceItem::Type, GetItemType(domainconstants::kParallelInstructionType));
  EXPECT_EQ(RepeatItem::Type, GetItemType(domainconstants::kRepeatInstructionType));
  EXPECT_EQ(SequenceItem::Type, GetItemType(domainconstants::kSequenceInstructionType));
  EXPECT_EQ(UserChoiceItem::Type, GetItemType(domainconstants::kUserChoiceInstructionType));
  EXPECT_EQ(WaitItem::Type, GetItemType(domainconstants::kWaitInstructionType));
}

//! Validate CreateInstructionItem factory function.

TEST_F(TransformFromDomainTest, CreateVariableItem)
{
  EXPECT_TRUE(CanCreateVariableForType<sequencergui::LocalVariableItem>(
      domainconstants::kLocalVariableType));
  EXPECT_TRUE(
      CanCreateVariableForType<sequencergui::FileVariableItem>(domainconstants::kFileVariableType));
  if (DomainUtils::IsChannelAccessAvailable())
  {
    EXPECT_TRUE(CanCreateVariableForType<sequencergui::ChannelAccessVariableItem>(
        domainconstants::kChannelAccessVariableType));
  }
  if (DomainUtils::IsPVAccessAvailable())
  {
    EXPECT_TRUE(CanCreateVariableForType<sequencergui::PVClientVariableItem>(
        domainconstants::kPVClientVariableType));
    EXPECT_TRUE(CanCreateVariableForType<sequencergui::PVServerVariableItem>(
        domainconstants::kPVServerVariableType));
  }
}

//! Validate CreateInstructionItem factory function.

TEST_F(TransformFromDomainTest, CreateInstructionItem)
{
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::ConditionItem>(
      domainconstants::kConditionInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequencergui::CopyItem>(domainconstants::kCopyInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::EqualsItem>(
      domainconstants::kEqualsInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::FallbackItem>(
      domainconstants::kFallbackInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::ForceSuccessItem>(
      domainconstants::kForceSuccessInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::IncludeItem>(
      domainconstants::kIncludeInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequencergui::InputItem>(domainconstants::kInputInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::InverterItem>(
      domainconstants::kInverterInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::ListenItem>(
      domainconstants::kListenInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::MessageItem>(
      domainconstants::kMessageInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::OutputItem>(
      domainconstants::kOutputInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::ParallelSequenceItem>(
      domainconstants::kParallelInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::RepeatItem>(
      domainconstants::kRepeatInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::SequenceItem>(
      domainconstants::kSequenceInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::UserChoiceItem>(
      domainconstants::kUserChoiceInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequencergui::WaitItem>(domainconstants::kWaitInstructionType));
}

//! Populate InstructionContainerItem from empty Procedure.

TEST_F(TransformFromDomainTest, PopulateItemContainerFromEmptyProcedure)
{
  ::sup::sequencer::Procedure procedure;

  sequencergui::InstructionContainerItem container;
  sequencergui::PopulateInstructionContainerItem(&procedure, &container, /*root_only*/ false);

  EXPECT_EQ(container.GetTotalItemCount(), 0);
}

//! Populate InstructionContainerItem from Procedure with a single Wait instruction.

TEST_F(TransformFromDomainTest, PopulateItemContainerFromProcedureWithWait)
{
  ::sup::sequencer::Procedure procedure;

  auto wait = DomainUtils::CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait->AddAttribute(sequencergui::domainconstants::kWaitTimeoutAttribute, "42");
  procedure.PushInstruction(wait.release());

  sequencergui::InstructionContainerItem container;
  sequencergui::PopulateInstructionContainerItem(&procedure, &container, /*root_only*/ false);

  auto item = container.GetItem<sequencergui::WaitItem>("");
  EXPECT_EQ(item->GetTimeout(), 42.0);
}

//! Populate InstructionContainerItem from Procedure with two Wait instruction.
//! One is marked as root instruction, root_only=true is used

TEST_F(TransformFromDomainTest, PopulateItemContainerFromProcedureWithTwoWaits)
{
  ::sup::sequencer::Procedure procedure;

  auto wait0 = DomainUtils::CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait0->AddAttribute(sequencergui::domainconstants::kWaitTimeoutAttribute, "42");
  procedure.PushInstruction(wait0.release());

  auto wait1 = DomainUtils::CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait1->AddAttribute(sequencergui::domainconstants::kIsRootAttribute, "true");
  wait1->AddAttribute(sequencergui::domainconstants::kWaitTimeoutAttribute, "43");
  procedure.PushInstruction(wait1.release());

  sequencergui::InstructionContainerItem container;
  sequencergui::PopulateInstructionContainerItem(&procedure, &container, /*root_only*/ true);

  auto item = container.GetItem<sequencergui::WaitItem>("");
  EXPECT_EQ(item->GetTimeout(), 43.0);
}

//! Populate InstructionContainerItem from Procedure with a Sequence containing Wait instruction.

TEST_F(TransformFromDomainTest, PopulateItemContainerFromProcedureWithSequence)
{
  ::sup::sequencer::Procedure procedure;

  auto wait = DomainUtils::CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait->AddAttribute(sequencergui::domainconstants::kWaitTimeoutAttribute, "42");

  auto sequence = DomainUtils::CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  sequence->InsertInstruction(wait.release(), 0);

  procedure.PushInstruction(sequence.release());

  sequencergui::InstructionContainerItem container;
  sequencergui::PopulateInstructionContainerItem(&procedure, &container, /*root_only*/ false);

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

  auto local_variable = DomainUtils::CreateDomainVariable(domainconstants::kLocalVariableType);
  local_variable->AddAttribute(domainconstants::kTypeAttribute, expected_type);
  local_variable->AddAttribute(domainconstants::kValueAttribute, expected_value);

  procedure.AddVariable("abc", local_variable.release());

  sequencergui::WorkspaceItem workspace_item;
  sequencergui::PopulateWorkspaceItem(&procedure, &workspace_item);
  EXPECT_EQ(workspace_item.GetTotalItemCount(), 1);

  auto variable_item = workspace_item.GetItem<sequencergui::LocalVariableItem>("");
  EXPECT_EQ(variable_item->GetJsonType(), expected_type);
  EXPECT_EQ(variable_item->GetJsonValue(), expected_value);
}

TEST_F(TransformFromDomainTest, CreateUnknownInstructionItem)
{
  auto item = CreateUnknownInstructionItem(domainconstants::kWaitInstructionType);

  EXPECT_EQ(item->GetType(), UnknownInstructionItem::Type);
  EXPECT_EQ(item->GetDomainType(), domainconstants::kWaitInstructionType);
}
