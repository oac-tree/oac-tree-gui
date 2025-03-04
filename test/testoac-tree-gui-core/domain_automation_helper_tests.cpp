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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/domain/domain_automation_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_info.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_info.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests of helper methods from domain_automation_helper.h
 */
class DomainAutomationHelperTest : public ::testing::Test
{
};

TEST_F(DomainAutomationHelperTest, CreateDomainInstruction)
{
  const std::size_t instruction_id{0};
  const sup::oac_tree::InstructionInfo info(oac_tree_gui::domainconstants::kWaitInstructionType,
                                            instruction_id,
                                            {{domainconstants::kTimeoutAttribute, "42"}});

  auto instruction = CreateDomainInstruction(info);

  EXPECT_EQ(instruction->GetType(), domainconstants::kWaitInstructionType);
  EXPECT_TRUE(instruction->HasAttribute(domainconstants::kTimeoutAttribute));
  EXPECT_EQ(instruction->GetAttributeString(domainconstants::kTimeoutAttribute), "42");
}

TEST_F(DomainAutomationHelperTest, CreateDomainVariable)
{
  const std::size_t variable_id{0};
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  const std::vector<sup::oac_tree::AttributeInfo> attributes(
      {{domainconstants::kNameAttribute, expected_name},
       {domainconstants::kTypeAttribute, expected_type},
       {domainconstants::kValueAttribute, expected_value}});
  sup::oac_tree::VariableInfo info(oac_tree_gui::domainconstants::kLocalVariableType, variable_id,
                                   attributes);

  auto variable = CreateDomainVariable(info);

  EXPECT_EQ(variable->GetType(), domainconstants::kLocalVariableType);
  EXPECT_EQ(variable->GetAttributeString(domainconstants::kNameAttribute), expected_name);
  EXPECT_EQ(variable->GetAttributeString(domainconstants::kValueAttribute), expected_value);
  EXPECT_EQ(variable->GetAttributeString(domainconstants::kTypeAttribute), expected_type);
}

TEST_F(DomainAutomationHelperTest, GetAttribute)
{
  const std::size_t instruction_id{0};
  const sup::oac_tree::InstructionInfo info(oac_tree_gui::domainconstants::kWaitInstructionType,
                                            instruction_id,
                                            {{domainconstants::kTimeoutAttribute, "42"}});

  auto instruction = CreateDomainInstruction(info);

  EXPECT_EQ(GetAttribute(info, domainconstants::kTimeoutAttribute).value_or(""), std::string("42"));
  EXPECT_EQ(GetAttribute(info, "non-existing").value_or(""), std::string(""));
}

}  // namespace oac_tree_gui::test
