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

#include "sequencergui/domain/domain_automation_helper.h"

#include <sequencergui/domain/domain_constants.h>

#include <sup/auto-server/instruction_info.h>
#include <sup/sequencer/instruction.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests of helper methods from domain_automation_helper.h

class DomainAutomationHelperTest : public ::testing::Test
{
};

TEST_F(DomainAutomationHelperTest, CreateDomainInstruction)
{
  const size_t instruction_id{0};
  sup::auto_server::InstructionInfo info(sequencergui::domainconstants::kWaitInstructionType,
                                         instruction_id,
                                         {{domainconstants::kTimeoutAttribute, "42"}});

  auto instruction = CreateDomainInstruction(info);

  EXPECT_EQ(instruction->GetType(), domainconstants::kWaitInstructionType);
  EXPECT_TRUE(instruction->HasAttribute(domainconstants::kTimeoutAttribute));
  EXPECT_EQ(instruction->GetAttributeString(domainconstants::kTimeoutAttribute), "42");
}
