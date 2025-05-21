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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/epics_instruction_items.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_variable_items.h>

#include <sup/oac-tree/procedure_preamble.h>

#include <gtest/gtest.h>
#include <testutils/universal_items.h>

#include "oac_tree_gui/transform/transform_from_domain.h"

namespace oac_tree_gui::test
{

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
  //! "Type" of sup::oac_tree::Instruction.
  template <typename T>
  bool CanCreateInstructionForType(const std::string& domain_type)
  {
    auto created = oac_tree_gui::CreateInstructionItem(domain_type);
    const bool is_correct_type = IsCorrectType<T>(created.get());

    // for now model type of Item coincides with model type of domain struction
    const bool is_type_coincides = created->GetType() == domain_type;

    return is_correct_type && is_type_coincides;
  }

  //! Returns true if we can create VariableItem of proper type for string representing
  //! "Type" of sup::oac_tree::Variable.
  template <typename T>
  bool CanCreateVariableForType(const std::string& seq_type)
  {
    auto created = oac_tree_gui::CreateVariableItem(seq_type);
    const bool is_correct_type = IsCorrectType<T>(created.get());

    // for now model type of Item coincides with model type of domain struction
    const bool is_type_coincides = created->GetType() == seq_type;

    return is_correct_type && is_type_coincides;
  }
};

//! Validate CreateInstructionItem factory function.

TEST_F(TransformFromDomainTest, CreateVariableItem)
{
  using namespace oac_tree_gui::domainconstants;

  EXPECT_TRUE(CanCreateVariableForType<oac_tree_gui::LocalVariableItem>(kLocalVariableType));
  EXPECT_TRUE(CanCreateVariableForType<oac_tree_gui::FileVariableItem>(kFileVariableType));
}

//! Validate CreateInstructionItem factory function (sequencer-plugin-epics).

TEST_F(TransformFromDomainTest, SequencerPluginEpicsCreateVariableItem)
{
  using namespace oac_tree_gui::domainconstants;

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  EXPECT_TRUE(CanCreateVariableForType<oac_tree_gui::ChannelAccessVariableItem>(
      kChannelAccessVariableType));
  EXPECT_TRUE(CanCreateVariableForType<oac_tree_gui::PvAccessClientVariableItem>(
      kPvAccessClientVariableType));
  EXPECT_TRUE(CanCreateVariableForType<oac_tree_gui::PvAccessServerVariableItem>(
      kPvAccessServerVariableType));
}

//! Validate CreateInstructionItem factory function.

TEST_F(TransformFromDomainTest, CreateInstructionItem)
{
  using namespace oac_tree_gui::domainconstants;

  EXPECT_TRUE(CanCreateInstructionForType<oac_tree_gui::IncludeItem>(kIncludeInstructionType));

  EXPECT_TRUE(
      CanCreateInstructionForType<oac_tree_gui::ParallelSequenceItem>(kParallelInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<oac_tree_gui::RepeatItem>(kRepeatInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<oac_tree_gui::SequenceItem>(kSequenceInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<oac_tree_gui::WaitItem>(kWaitInstructionType));
}

//! Validate CreateInstructionItem factory function (sequencer-plugin-epics).

TEST_F(TransformFromDomainTest, SequencerPluginEpicsCreateInstructionItem)
{
  using namespace oac_tree_gui::domainconstants;

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  EXPECT_TRUE(CanCreateInstructionForType<oac_tree_gui::ChannelAccessReadInstructionItem>(
      kChannelAccessReadInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<oac_tree_gui::ChannelAccessWriteInstructionItem>(
      kChannelAccessWriteInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<oac_tree_gui::PvAccessReadInstructionItem>(
      kPvAccessReadInstructionType));
  EXPECT_TRUE(CanCreateInstructionForType<oac_tree_gui::PvAccessWriteInstructionItem>(
      kPvAccessWriteInstructionType));
  EXPECT_TRUE(
      CanCreateInstructionForType<oac_tree_gui::RPCClientInstruction>(kRPCClientInstructionType));
}

TEST_F(TransformFromDomainTest, CreateUniversalInstruction)
{
  using test::UnknownDomainInstruction;

  UnknownDomainInstruction::RegisterUnknownDomainInstruction();

  auto item = CreateInstructionItem(UnknownDomainInstruction::Type);
  auto universal_item = dynamic_cast<UniversalInstructionItem*>(item.get());
  EXPECT_NE(universal_item, nullptr);

  EXPECT_EQ(universal_item->GetDomainType(), UnknownDomainInstruction::Type);
  EXPECT_EQ(universal_item->GetType(), UniversalInstructionItem::GetStaticType());
}

TEST_F(TransformFromDomainTest, CreateUniversalVariable)
{
  using test::UnknownDomainVariable;

  UnknownDomainVariable::RegisterUnknownDomainVariable();

  auto item = CreateVariableItem(UnknownDomainVariable::Type);
  auto universal_item = dynamic_cast<UniversalVariableItem*>(item.get());
  EXPECT_NE(universal_item, nullptr);

  EXPECT_EQ(universal_item->GetDomainType(), UnknownDomainVariable::Type);
  EXPECT_EQ(universal_item->GetType(), UniversalVariableItem::GetStaticType());
}

TEST_F(TransformFromDomainTest, GetRunnerStatusFromDomain)
{
  using sup::oac_tree::JobState;
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kInitial), RunnerStatus::kInitial);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kPaused), RunnerStatus::kPaused);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kStepping), RunnerStatus::kStepping);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kRunning), RunnerStatus::kRunning);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kSucceeded), RunnerStatus::kSucceeded);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kFailed), RunnerStatus::kFailed);
  EXPECT_EQ(GetRunnerStatusFromDomain(JobState::kHalted), RunnerStatus::kHalted);
}

}  // namespace oac_tree_gui::test
