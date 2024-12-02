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

#include "sequencergui/transform/transform_from_domain.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/model/epics_instruction_items.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>

#include <sup/sequencer/procedure_preamble.h>

#include <gtest/gtest.h>
#include <testutils/universal_items.h>

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
  bool CanCreateInstructionForType(const std::string& domain_type)
  {
    auto created = sequencergui::CreateInstructionItem(domain_type);
    const bool is_correct_type = IsCorrectType<T>(created.get());

    // for now model type of Item coincides with model type of domain struction
    const bool is_type_coincides = created->GetType() == domain_type;

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

  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::IncludeItem>(kIncludeInstructionType));

  EXPECT_TRUE(
      CanCreateInstructionForType<sequencergui::ParallelSequenceItem>(kParallelInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::RepeatItem>(kRepeatInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::SequenceItem>(kSequenceInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<sequencergui::WaitItem>(kWaitInstructionType));
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

TEST_F(TransformFromDomainTest, CreateUniversalInstruction)
{
  using testutils::UnknownDomainInstruction;

  UnknownDomainInstruction::RegisterUnknownDomainInstruction();

  auto item = CreateInstructionItem(UnknownDomainInstruction::Type);
  auto universal_item = dynamic_cast<UniversalInstructionItem*>(item.get());
  EXPECT_NE(universal_item, nullptr);

  EXPECT_EQ(universal_item->GetDomainType(), UnknownDomainInstruction::Type);
  EXPECT_EQ(universal_item->GetType(), UniversalInstructionItem::Type);
}

TEST_F(TransformFromDomainTest, CreateUniversalVariable)
{
  using testutils::UnknownDomainVariable;

  UnknownDomainVariable::RegisterUnknownDomainVariable();

  auto item = CreateVariableItem(UnknownDomainVariable::Type);
  auto universal_item = dynamic_cast<UniversalVariableItem*>(item.get());
  EXPECT_NE(universal_item, nullptr);

  EXPECT_EQ(universal_item->GetDomainType(), UnknownDomainVariable::Type);
  EXPECT_EQ(universal_item->GetType(), UniversalVariableItem::Type);
}

TEST_F(TransformFromDomainTest, GetRunnerStatusFromDomain)
{
  using sup::sequencer::JobState;
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kInitial), RunnerStatus::kInitial);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kPaused), RunnerStatus::kPaused);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kStepping), RunnerStatus::kStepping);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kRunning), RunnerStatus::kRunning);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kSucceeded), RunnerStatus::kSucceeded);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kFailed), RunnerStatus::kFailed);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kHalted), RunnerStatus::kHalted);
}
