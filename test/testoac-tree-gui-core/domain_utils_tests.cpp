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

#include "oac_tree_gui/domain/domain_helper.h"

#include <oac_tree_gui/domain/domain_object_type_registry.h>

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/variable.h>

#include <gtest/gtest.h>

using namespace oac_tree_gui::domainconstants;

namespace oac_tree_gui::test
{

class DomainUtilsTest : public ::testing::Test
{
};

//! An attempt to catch if our duplication of domain constants diverges from the domain.

TEST_F(DomainUtilsTest, DomainInstructionTypeConstants)
{
  const std::vector<std::string> domain_types = {
      kChoiceInstructionType,        kConditionInstructionType,
      kCopyInstructionType,          kDecrementInstructionType,
      kEqualsInstructionType,        kFallbackInstructionType,
      kForceSuccessInstructionType,  kIncludeInstructionType,
      kIncrementInstructionType,     kInputInstructionType,
      kInverterInstructionType,      kListenInstructionType,
      kMessageInstructionType,       kOutputInstructionType,
      kParallelInstructionType,      kRepeatInstructionType,
      kSequenceInstructionType,      kUserChoiceInstructionType,
      kVariableResetInstructionType, kWaitInstructionType,
      kLessThanInstructionType,      kLessThanOrEqualInstructionType,
      kGreaterThanInstructionType,   kGreaterThanOrEqualInstructionType,
      kSucceedInstructionType,       kFailedInstructionType};

  for (const auto& domain_type : domain_types)
  {
    EXPECT_EQ(CreateDomainInstruction(domain_type)->GetType(), domain_type);
  }
}

//! An attempt to catch if our duplication of domain constants diverges from the domain.
//! Testing instructions in sup-sequencer-epics-plugin.

TEST_F(DomainUtilsTest, SequencerPluginEpicsDomainInstructionTypeConstants)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::vector<std::string> domain_types = {kChannelAccessReadInstructionType,
                                                 kChannelAccessWriteInstructionType,
                                                 kPvAccessReadInstructionType,
                                                 kPvAccessWriteInstructionType,
                                                 kRPCClientInstructionType,
                                                 kLogInstructionType};
  for (const auto& domain_type : domain_types)
  {
    EXPECT_EQ(CreateDomainInstruction(domain_type)->GetType(), domain_type);
  }
}

//! An attempt to catch if our duplication of domain constants diverges from the domain.

TEST_F(DomainUtilsTest, DomainVariableTypeConstants)
{
  std::vector<std::string> domain_types = {kFileVariableType, kLocalVariableType};

  for (const auto& domain_type : domain_types)
  {
    EXPECT_EQ(CreateDomainVariable(domain_type)->GetType(), domain_type);
  }
}

//! An attempt to catch if our duplication of domain constants diverges from the domain.
//! Testing variables in sup-sequencer-epics-plugin.

TEST_F(DomainUtilsTest, SequencerPluginEpicsDomainVariableTypeConstants)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  std::vector<std::string> domain_types = {kChannelAccessVariableType, kPvAccessClientVariableType,
                                           kPvAccessServerVariableType};

  for (const auto& domain_type : domain_types)
  {
    EXPECT_EQ(CreateDomainVariable(domain_type)->GetType(), domain_type);
  }
}

TEST_F(DomainUtilsTest, DialogMetagadata)
{
  sup::dto::AnyValue anyvalue;
  EXPECT_FALSE(IsSelectTextDialog(anyvalue));
  EXPECT_FALSE(IsMessageBoxDialog(anyvalue));

  {  // pretending it is metadata representing test selection
    auto metadata = sup::oac_tree::CreateUserChoiceMetadata();
    metadata.AddMember(sup::oac_tree::Constants::USER_CHOICES_TEXT_NAME, "description");
    metadata.AddMember(sup::oac_tree::Constants::USER_CHOICES_DIALOG_TYPE_NAME,
                       {sup::dto::UnsignedInteger32Type, sup::oac_tree::dialog_type::kSelection});

    EXPECT_TRUE(IsSelectTextDialog(metadata));
    EXPECT_FALSE(IsMessageBoxDialog(metadata));
  }

  {  // pretending it is metadata representing ok/cancel choice
    auto metadata = sup::oac_tree::CreateUserChoiceMetadata();
    metadata.AddMember(sup::oac_tree::Constants::USER_CHOICES_TEXT_NAME, "description");
    metadata.AddMember(
        sup::oac_tree::Constants::USER_CHOICES_DIALOG_TYPE_NAME,
        {sup::dto::UnsignedInteger32Type, sup::oac_tree::dialog_type::kConfirmation});

    EXPECT_FALSE(IsSelectTextDialog(metadata));
    EXPECT_TRUE(IsMessageBoxDialog(metadata));
  }
}

TEST_F(DomainUtilsTest, GlobalDomainObjectTypeRegistry)
{
  const std::string undefined("undefined");

  // instructions and variables from core

  EXPECT_EQ(GlobalDomainObjectTypeRegistry()
                .GetPluginName(domainconstants::kWaitInstructionType)
                .value_or(undefined),
            domainconstants::kCorePluginName);

  EXPECT_EQ(GlobalDomainObjectTypeRegistry()
                .GetPluginName(domainconstants::kLocalVariableType)
                .value_or(undefined),
            domainconstants::kCorePluginName);

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  // instructions and variables from EPICS libs

  EXPECT_EQ(GlobalDomainObjectTypeRegistry()
                .GetPluginName(domainconstants::kChannelAccessReadInstructionType)
                .value_or(undefined),
            domainconstants::kEpicsCAPluginName);

  EXPECT_EQ(GlobalDomainObjectTypeRegistry()
                .GetPluginName(domainconstants::kPvAccessClientVariableType)
                .value_or(undefined),
            domainconstants::kEpicsPVXSPluginName);
}

}  // namespace oac_tree_gui::test
