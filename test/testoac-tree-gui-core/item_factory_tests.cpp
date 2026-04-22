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

#include "oac_tree_gui/model/item_factory.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/epics_instruction_items.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_variable_items.h>

#include <gtest/gtest.h>
#include <testutils/universal_items.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests functions from item_factory.h
 */
class ItemFactoryTest : public ::testing::Test
{
public:
  /**
   * @brief Checks if given item can be casted to specific type.
   */
  template <typename T>
  bool IsCorrectType(const mvvm::SessionItem* item)
  {
    return dynamic_cast<const T*>(item) != nullptr;
  }

  /**
   * @brief Checks if CreateInstructionItem factory function creates item of proper type for string
   * representing "Type" of sup::oac_tree::Instruction.
   */
  template <typename T>
  bool CanCreateCustomInstruction(const std::string& domain_type)
  {
    auto created = oac_tree_gui::CreateInstructionItem(domain_type);
    const bool is_correct_type = IsCorrectType<T>(created.get());

    // for custom item (not universal item) model type coincides with domain type
    const bool are_types_coincide = created->GetType() == domain_type;

    return is_correct_type && are_types_coincide;
  }

  /**
   * @brief Checks if CreateVariableItem factory function creates item of proper type for string
   * representing "Type" of sup::oac_tree::Variable.
   */
  template <typename T>
  bool CanCreateCustomVariable(const std::string& domain_type)
  {
    auto created = oac_tree_gui::CreateVariableItem(domain_type);
    const bool is_correct_type = IsCorrectType<T>(created.get());

    // for custom item (not universal item) model type coincides with domain type
    const bool are_types_coincide = created->GetType() == domain_type;

    return is_correct_type && are_types_coincide;
  }
};

TEST_F(ItemFactoryTest, CreateCustomVariableItem)
{
  using namespace oac_tree_gui::domainconstants;

  EXPECT_TRUE(CanCreateCustomVariable<oac_tree_gui::LocalVariableItem>(kLocalVariableType));
  EXPECT_TRUE(CanCreateCustomVariable<oac_tree_gui::FileVariableItem>(kFileVariableType));
}

TEST_F(ItemFactoryTest, CreateCustomEpicsVariableItem)
{
  using namespace oac_tree_gui::domainconstants;

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  EXPECT_TRUE(
      CanCreateCustomVariable<oac_tree_gui::ChannelAccessVariableItem>(kChannelAccessVariableType));
  EXPECT_TRUE(CanCreateCustomVariable<oac_tree_gui::PvAccessClientVariableItem>(
      kPvAccessClientVariableType));
  EXPECT_TRUE(CanCreateCustomVariable<oac_tree_gui::PvAccessServerVariableItem>(
      kPvAccessServerVariableType));
}

TEST_F(ItemFactoryTest, CreateCustomInstructionItem)
{
  using namespace oac_tree_gui::domainconstants;

  EXPECT_TRUE(CanCreateCustomInstruction<oac_tree_gui::IncludeItem>(kIncludeInstructionType));

  EXPECT_TRUE(
      CanCreateCustomInstruction<oac_tree_gui::ParallelSequenceItem>(kParallelInstructionType));
  EXPECT_TRUE(CanCreateCustomInstruction<oac_tree_gui::RepeatItem>(kRepeatInstructionType));
  EXPECT_TRUE(CanCreateCustomInstruction<oac_tree_gui::SequenceItem>(kSequenceInstructionType));
  EXPECT_TRUE(CanCreateCustomInstruction<oac_tree_gui::WaitItem>(kWaitInstructionType));
}

TEST_F(ItemFactoryTest, CreateCustomEpicsInstructionItem)
{
  using namespace oac_tree_gui::domainconstants;

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  EXPECT_TRUE(CanCreateCustomInstruction<oac_tree_gui::ChannelAccessReadInstructionItem>(
      kChannelAccessReadInstructionType));
  EXPECT_TRUE(CanCreateCustomInstruction<oac_tree_gui::ChannelAccessWriteInstructionItem>(
      kChannelAccessWriteInstructionType));
  EXPECT_TRUE(CanCreateCustomInstruction<oac_tree_gui::PvAccessReadInstructionItem>(
      kPvAccessReadInstructionType));
  EXPECT_TRUE(CanCreateCustomInstruction<oac_tree_gui::PvAccessWriteInstructionItem>(
      kPvAccessWriteInstructionType));
  EXPECT_TRUE(
      CanCreateCustomInstruction<oac_tree_gui::RPCClientInstruction>(kRPCClientInstructionType));
}

TEST_F(ItemFactoryTest, CreateUniversalInstruction)
{
  using test::UnknownDomainInstruction;

  UnknownDomainInstruction::RegisterUnknownDomainInstruction();

  auto item = CreateInstructionItem(UnknownDomainInstruction::Type);
  auto universal_item = dynamic_cast<UniversalInstructionItem*>(item.get());
  EXPECT_NE(universal_item, nullptr);

  EXPECT_EQ(universal_item->GetDomainType(), UnknownDomainInstruction::Type);
  EXPECT_EQ(universal_item->GetType(), mvvm::GetTypeName<UniversalInstructionItem>());
}

TEST_F(ItemFactoryTest, CreateUniversalVariable)
{
  using test::UnknownDomainVariable;

  UnknownDomainVariable::RegisterUnknownDomainVariable();

  auto item = CreateVariableItem(UnknownDomainVariable::Type);
  auto universal_item = dynamic_cast<UniversalVariableItem*>(item.get());
  EXPECT_NE(universal_item, nullptr);

  EXPECT_EQ(universal_item->GetDomainType(), UnknownDomainVariable::Type);
  EXPECT_EQ(universal_item->GetType(), mvvm::GetTypeName<UniversalVariableItem>());
}

}  // namespace oac_tree_gui::test
