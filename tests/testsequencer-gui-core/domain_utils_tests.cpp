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

#include "sequencergui/domain/domain_utils.h"

#include <gtest/gtest.h>

#include <sup/sequencer/instruction.h>

#include <chrono>
#include <future>

using namespace sequencergui;
using namespace sequencergui::domainconstants;

class DomainUtilsTest : public ::testing::Test
{
};

//! An attempt to catch if our duplication of domain constants diverges from the domain.

TEST_F(DomainUtilsTest, DomainTypeConstants)
{
  std::vector<std::string> domain_types = {
      kConditionInstructionType, kCopyInstructionType,         kEqualsInstructionType,
      kFallbackInstructionType,  kForceSuccessInstructionType, kIncludeInstructionType,
      kInputInstructionType,     kInverterInstructionType,     kListenInstructionType,
      kMessageInstructionType,   kOutputInstructionType,       kParallelInstructionType,
      kRepeatInstructionType,    kSequenceInstructionType,     kUserChoiceInstructionType,
      kWaitInstructionType};

  for (const auto& domain_type : domain_types)
  {
    EXPECT_EQ(DomainUtils::CreateDomainInstruction(domain_type)->GetType(), domain_type);
  }
}

TEST_F(DomainUtilsTest, IsRootInstruction)
{
  auto item = DomainUtils::CreateDomainInstruction(kWaitInstructionType);
  EXPECT_FALSE(DomainUtils::IsRootInstruction(item.get()));

  item->AddAttribute(kIsRootAttribute, "true");
  EXPECT_TRUE(DomainUtils::IsRootInstruction(item.get()));

  item->SetAttribute(kIsRootAttribute, "yEs");
  EXPECT_TRUE(DomainUtils::IsRootInstruction(item.get()));

  item->SetAttribute(kIsRootAttribute, "false");
  EXPECT_FALSE(DomainUtils::IsRootInstruction(item.get()));
}
