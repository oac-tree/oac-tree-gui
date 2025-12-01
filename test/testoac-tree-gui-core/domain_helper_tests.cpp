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

#include <oac_tree_gui/core/exceptions.h>

#include <mvvm/core/platform.h>
#include <mvvm/utils/container_utils.h>

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/i_job_info_io.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/variable.h>

#include <gtest/gtest.h>

using namespace oac_tree_gui::domainconstants;

namespace oac_tree_gui::test
{

/**
 * @brief Tests for helper methods in domain_helper_tests.h
 */
class DomainHelperTest : public ::testing::Test
{
};

//! An attempt to catch if our duplication of domain constants diverges from the domain.
TEST_F(DomainHelperTest, DomainInstructionTypeConstants)
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
TEST_F(DomainHelperTest, SequencerPluginEpicsDomainInstructionTypeConstants)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::vector<std::string> domain_types = {
      kChannelAccessReadInstructionType, kChannelAccessWriteInstructionType,
      kPvAccessReadInstructionType,      kPvAccessWriteInstructionType,
      kRPCClientInstructionType,         kLogInstructionType};
  for (const auto& domain_type : domain_types)
  {
    EXPECT_EQ(CreateDomainInstruction(domain_type)->GetType(), domain_type);
  }
}

//! An attempt to catch if our duplication of domain constants diverges from the domain.
TEST_F(DomainHelperTest, DomainVariableTypeConstants)
{
  const std::vector<std::string> domain_types = {kFileVariableType, kLocalVariableType};

  for (const auto& domain_type : domain_types)
  {
    EXPECT_EQ(CreateDomainVariable(domain_type)->GetType(), domain_type);
  }
}

//! An attempt to catch if our duplication of domain constants diverges from the domain.
//! Testing variables in sup-sequencer-epics-plugin.
TEST_F(DomainHelperTest, SequencerPluginEpicsDomainVariableTypeConstants)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::vector<std::string> domain_types = {
      kChannelAccessVariableType, kPvAccessClientVariableType, kPvAccessServerVariableType};

  for (const auto& domain_type : domain_types)
  {
    EXPECT_EQ(CreateDomainVariable(domain_type)->GetType(), domain_type);
  }
}

TEST_F(DomainHelperTest, GetBasicPluginList)
{
  EXPECT_TRUE(mvvm::utils::Contains(GetBasicPluginList(), domainconstants::kEpicsCAPluginName));
}

TEST_F(DomainHelperTest, GetSupPluginList)
{
  EXPECT_TRUE(mvvm::utils::Contains(GetSupPluginList(), domainconstants::kSupConfigPluginName));
}

TEST_F(DomainHelperTest, GetTitleTextFromMetadata)
{
  auto valid_metadata = sup::oac_tree::CreateUserChoiceMetadata();
  valid_metadata.AddMember(sup::oac_tree::Constants::USER_CHOICES_TEXT_NAME,
                           sup::dto::AnyValue{"Sample Text"});
  valid_metadata.AddMember(
      sup::oac_tree::Constants::USER_CHOICES_DIALOG_TYPE_NAME,
      sup::dto::AnyValue{sup::dto::AnyType(sup::dto::UnsignedInteger32Type), 1});
  valid_metadata.AddMember(sup::oac_tree::Constants::USER_CHOICES_MODAL_NAME,
                           sup::dto::AnyValue{true});
  valid_metadata.AddMember(sup::oac_tree::Constants::USER_CHOICES_TITLE_NAME,
                           sup::dto::AnyValue{"Sample Title"});
  valid_metadata.AddMember(sup::oac_tree::Constants::USER_CHOICES_INFORMATIVE_TEXT_NAME,
                           sup::dto::AnyValue{"Informative Text"});
  valid_metadata.AddMember(sup::oac_tree::Constants::USER_CHOICES_DETAILS_NAME,
                           sup::dto::AnyValue{"Details"});

  EXPECT_EQ(GetMainTextFromMetadata(valid_metadata), "Sample Text");
}

TEST_F(DomainHelperTest, DialogMetagadata)
{
  const sup::dto::AnyValue anyvalue;
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

TEST_F(DomainHelperTest, IsValidInstructionIndex)
{
  EXPECT_TRUE(IsValidInstructionIndex(0));
  EXPECT_TRUE(IsValidInstructionIndex(42));
  EXPECT_FALSE(IsValidInstructionIndex(sup::oac_tree::kInvalidInstructionIndex));
}

TEST_F(DomainHelperTest, GetPluginFileName)
{
  EXPECT_EQ(GetPluginFileName(""), "");

  if (mvvm::IsLinuxHost())
  {
    // main usage
    EXPECT_EQ(GetPluginFileName("MyPlugin"), "libMyPlugin.so");

    EXPECT_EQ(GetPluginFileName("libMyPlugin"), "libMyPlugin.so");
    EXPECT_EQ(GetPluginFileName("MyPlugin.so"), "libMyPlugin.so");
    EXPECT_EQ(GetPluginFileName("libMyPlugin.so"), "libMyPlugin.so");

    // MacOs should be converted to Linux
    EXPECT_EQ(GetPluginFileName("libMyPlugin.dylib"), "libMyPlugin.so");

    // For the moment we ignore path
    EXPECT_EQ(GetPluginFileName("/home/user/libMyPlugin.so"), "/home/user/libMyPlugin.so");
    EXPECT_EQ(GetPluginFileName("/home/user/MyPlugin"), "/home/user/MyPlugin");
  }

  if (mvvm::IsMacHost())
  {
    // main usage
    EXPECT_EQ(GetPluginFileName("MyPlugin"), "libMyPlugin.dylib");

    EXPECT_EQ(GetPluginFileName("libMyPlugin"), "libMyPlugin.dylib");
    EXPECT_EQ(GetPluginFileName("MyPlugin.dylib"), "libMyPlugin.dylib");
    EXPECT_EQ(GetPluginFileName("libMyPlugin.dylib"), "libMyPlugin.dylib");

    // Linux should be converted to MacOs
    EXPECT_EQ(GetPluginFileName("libMyPlugin.so"), "libMyPlugin.dylib");

    // For the moment we ignore path
    EXPECT_EQ(GetPluginFileName("/home/user/libMyPlugin.dylib"), "/home/user/libMyPlugin.dylib");
    EXPECT_EQ(GetPluginFileName("/home/user/MyPlugin"), "/home/user/MyPlugin");
  }
}

TEST_F(DomainHelperTest, GetPluginNameFromFileName)
{
  if (mvvm::IsLinuxHost())
  {
    // main usage
    EXPECT_EQ(GetPluginNameFromFileName("libMyPlugin.so"), "MyPlugin");
    EXPECT_EQ(GetPluginNameFromFileName("/some/path/libMyPlugin.so"), "MyPlugin");
    EXPECT_EQ(GetPluginNameFromFileName("MyPlugin"), "MyPlugin");
    EXPECT_EQ(GetPluginNameFromFileName("/some/path/MyPlugin"), "MyPlugin");
  }

  if (mvvm::IsMacHost())
  {
    EXPECT_EQ(GetPluginNameFromFileName("libMyPlugin.dylib"), "MyPlugin");
    EXPECT_EQ(GetPluginNameFromFileName("/some/path/libMyPlugin.dylib"), "MyPlugin");
    EXPECT_EQ(GetPluginNameFromFileName("MyPlugin"), "MyPlugin");
    EXPECT_EQ(GetPluginNameFromFileName("/some/path/MyPlugin"), "MyPlugin");
  }
}

TEST_F(DomainHelperTest, GetDescriptionForInstructionType)
{
  EXPECT_TRUE(GetDescriptionForInstructionType("non-existing-type").empty());
  EXPECT_FALSE(
      GetDescriptionForInstructionType(oac_tree_gui::domainconstants::kSequenceInstructionType)
          .empty());
}

}  // namespace oac_tree_gui::test
