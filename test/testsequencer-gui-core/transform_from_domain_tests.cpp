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

#include "sequencergui/transform/transform_from_domain.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/epics_instruction_items.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>

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
    auto created = sequencergui::CreateInstructionItem(seq_type);
    const bool is_correct_type = IsCorrectType<T>(created.get());

    // for now model type of Item coincides with model type of domain struction
    const bool is_type_coincides = created->GetType() == seq_type;

    return is_correct_type && is_type_coincides;
  }

  //! Returns true if we can create VariableItem of proper type for string representing
  //! "Type" of sup::sequencer::Variable.
  template <typename T>
  bool CanCreateVariableForType(const std::string& seq_type)
  {
    auto created = sequencergui::CreateVariableItem(seq_type);
    const bool is_correct_type = IsCorrectType<T>(created.get());

    // for now model type of Item coincides with model type of domain struction
    const bool is_type_coincides = created->GetType() == seq_type;

    return is_correct_type && is_type_coincides;
  }
};

TEST_F(TransformFromDomainTest, GetItemType)
{
  // for instructions
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

  // for variables
  EXPECT_EQ(ChannelAccessVariableItem::Type,
            GetItemType(domainconstants::kChannelAccessVariableType));
  EXPECT_EQ(FileVariableItem::Type, GetItemType(domainconstants::kFileVariableType));
  EXPECT_EQ(LocalVariableItem::Type, GetItemType(domainconstants::kLocalVariableType));
  EXPECT_EQ(PvAccessClientVariableItem::Type, GetItemType(domainconstants::kPVClientVariableType));
}

//! Validate CreateInstructionItem factory function.

TEST_F(TransformFromDomainTest, CreateVariableItem)
{
  EXPECT_TRUE(CanCreateVariableForType<sequencergui::LocalVariableItem>(
      domainconstants::kLocalVariableType));
  EXPECT_TRUE(
      CanCreateVariableForType<sequencergui::FileVariableItem>(domainconstants::kFileVariableType));
}

//! Validate CreateInstructionItem factory function (sequencer-plugin-epics).

TEST_F(TransformFromDomainTest, SequencerPluginEpicsCreateVariableItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  EXPECT_TRUE(CanCreateVariableForType<sequencergui::ChannelAccessVariableItem>(
      domainconstants::kChannelAccessVariableType));
  EXPECT_TRUE(CanCreateVariableForType<sequencergui::PvAccessClientVariableItem>(
      domainconstants::kPVClientVariableType));
  EXPECT_TRUE(CanCreateVariableForType<sequencergui::PvAccessServerVariableItem>(
      domainconstants::kPVServerVariableType));
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

//! Validate CreateInstructionItem factory function (sequencer-plugin-epics).

TEST_F(TransformFromDomainTest, SequencerPluginEpicsCreateInstructionItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::ChannelAccessReadInstructionItem>(
      domainconstants::kChannelAccessReadInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::ChannelAccessWriteInstructionItem>(
      domainconstants::kChannelAccessWriteInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::PvAccessReadInstructionItem>(
      domainconstants::kPvAccessReadInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::PvAccessWriteInstructionItem>(
      domainconstants::kPvAccessWriteInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::RPCClientInstruction>(
      domainconstants::kRPCClientInstructionType));
}

TEST_F(TransformFromDomainTest, CreateUnknownInstructionItem)
{
  auto item = CreateUnknownInstructionItem(domainconstants::kWaitInstructionType);

  EXPECT_EQ(item->GetType(), UnknownInstructionItem::Type);
  EXPECT_EQ(item->GetDomainType(), domainconstants::kWaitInstructionType);
}
