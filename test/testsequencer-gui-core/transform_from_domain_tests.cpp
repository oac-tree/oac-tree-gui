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
#include <sequencergui/model/equality_instruction_items.h>
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
  using namespace sequencergui::domainconstants;

  // for instructions
  EXPECT_EQ(ChoiceItem::Type, GetItemType(kChoiceInstructionType));
  EXPECT_EQ(ConditionItem::Type, GetItemType(kConditionInstructionType));
  EXPECT_EQ(CopyItem::Type, GetItemType(kCopyInstructionType));
  EXPECT_EQ(DecrementItem::Type, GetItemType(kDecrementInstructionType));
  EXPECT_EQ(FallbackItem::Type, GetItemType(kFallbackInstructionType));
  EXPECT_EQ(ForceSuccessItem::Type, GetItemType(kForceSuccessInstructionType));
  EXPECT_EQ(IncludeItem::Type, GetItemType(kIncludeInstructionType));
  EXPECT_EQ(IncrementItem::Type, GetItemType(kIncrementInstructionType));
  EXPECT_EQ(InputItem::Type, GetItemType(kInputInstructionType));
  EXPECT_EQ(InverterItem::Type, GetItemType(kInverterInstructionType));
  EXPECT_EQ(ListenItem::Type, GetItemType(kListenInstructionType));
  EXPECT_EQ(MessageItem::Type, GetItemType(kMessageInstructionType));
  EXPECT_EQ(OutputItem::Type, GetItemType(kOutputInstructionType));
  EXPECT_EQ(ParallelSequenceItem::Type, GetItemType(kParallelInstructionType));
  EXPECT_EQ(RepeatItem::Type, GetItemType(kRepeatInstructionType));
  EXPECT_EQ(SequenceItem::Type, GetItemType(kSequenceInstructionType));
  EXPECT_EQ(UserChoiceItem::Type, GetItemType(kUserChoiceInstructionType));
  EXPECT_EQ(VariableResetItem::Type, GetItemType(kVariableResetInstructionType));
  EXPECT_EQ(WaitItem::Type, GetItemType(kWaitInstructionType));

  // equality instructions
  EXPECT_EQ(LessThanItem::Type, GetItemType(kLessThanInstructionType));
  EXPECT_EQ(LessThanOrEqualItem::Type, GetItemType(kLessThanOrEqualInstructionType));
  EXPECT_EQ(EqualsItem::Type, GetItemType(kEqualsInstructionType));
  EXPECT_EQ(GreaterThanOrEqualItem::Type, GetItemType(kGreaterThanOrEqualInstructionType));
  EXPECT_EQ(GreaterThanItem::Type, GetItemType(kGreaterThanInstructionType));

  // instructions from sequencer-plugin-epics
  EXPECT_EQ(ChannelAccessReadInstructionItem::Type, GetItemType(kChannelAccessReadInstructionType));
  EXPECT_EQ(ChannelAccessWriteInstructionItem::Type,
            GetItemType(kChannelAccessWriteInstructionType));
  EXPECT_EQ(PvAccessReadInstructionItem::Type, GetItemType(kPvAccessReadInstructionType));
  EXPECT_EQ(PvAccessWriteInstructionItem::Type, GetItemType(kPvAccessWriteInstructionType));
  EXPECT_EQ(RPCClientInstruction::Type, GetItemType(kRPCClientInstructionType));
  EXPECT_EQ(SystemCallInstructionItem::Type, GetItemType(kSystemCallInstructionType));
  EXPECT_EQ(LogInstructionItem::Type, GetItemType(kLogInstructionType));

  // for variables
  EXPECT_EQ(ChannelAccessVariableItem::Type, GetItemType(kChannelAccessVariableType));
  EXPECT_EQ(FileVariableItem::Type, GetItemType(kFileVariableType));
  EXPECT_EQ(LocalVariableItem::Type, GetItemType(kLocalVariableType));
  EXPECT_EQ(PvAccessClientVariableItem::Type, GetItemType(kPvAccessClientVariableType));
}

//! Validate CreateInstructionItem factory function.

TEST_F(TransformFromDomainTest, CreateVariableItem)
{
  using namespace sequencergui::domainconstants;

  EXPECT_TRUE(CanCreateVariableForType<sequencergui::LocalVariableItem>(kLocalVariableType));
  EXPECT_TRUE(CanCreateVariableForType<sequencergui::FileVariableItem>(kFileVariableType));
}

//! Validate CreateInstructionItem factory function (sequencer-plugin-epics).

TEST_F(TransformFromDomainTest, SequencerPluginEpicsCreateVariableItem)
{
  using namespace sequencergui::domainconstants;

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  EXPECT_TRUE(CanCreateVariableForType<sequencergui::ChannelAccessVariableItem>(
      kChannelAccessVariableType));
  EXPECT_TRUE(CanCreateVariableForType<sequencergui::PvAccessClientVariableItem>(
      kPvAccessClientVariableType));
  EXPECT_TRUE(CanCreateVariableForType<sequencergui::PvAccessServerVariableItem>(
      kPvAccessServerVariableType));
}

//! Validate CreateInstructionItem factory function.

TEST_F(TransformFromDomainTest, CreateInstructionItem)
{
  using namespace sequencergui::domainconstants;

  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::ChoiceItem>(kChoiceInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::ConditionItem>(kConditionInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::CopyItem>(kCopyInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::DecrementItem>(kDecrementInstructionType));

  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::FallbackItem>(kFallbackInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequencergui::ForceSuccessItem>(kForceSuccessInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::IncludeItem>(kIncludeInstructionType));

  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::IncrementItem>(kIncrementInstructionType));

  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::InputItem>(kInputInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::InverterItem>(kInverterInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::ListenItem>(kListenInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::MessageItem>(kMessageInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::OutputItem>(kOutputInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequencergui::ParallelSequenceItem>(kParallelInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::RepeatItem>(kRepeatInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::SequenceItem>(kSequenceInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequencergui::UserChoiceItem>(kUserChoiceInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequencergui::VariableResetItem>(kVariableResetInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::WaitItem>(kWaitInstructionType));

  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::LessThanItem>(kLessThanInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::LessThanOrEqualItem>(
      kLessThanOrEqualInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::EqualsItem>(kEqualsInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::GreaterThanOrEqualItem>(
      kGreaterThanOrEqualInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequencergui::GreaterThanItem>(kGreaterThanInstructionType));
}

//! Validate CreateInstructionItem factory function (sequencer-plugin-epics).

TEST_F(TransformFromDomainTest, SequencerPluginEpicsCreateInstructionItem)
{
  using namespace sequencergui::domainconstants;

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::ChannelAccessReadInstructionItem>(
      kChannelAccessReadInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::ChannelAccessWriteInstructionItem>(
      kChannelAccessWriteInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::PvAccessReadInstructionItem>(
      kPvAccessReadInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::PvAccessWriteInstructionItem>(
      kPvAccessWriteInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<sequencergui::RPCClientInstruction>(kRPCClientInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::SystemCallInstructionItem>(
      kSystemCallInstructionType));
}
