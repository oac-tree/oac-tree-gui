/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/shall_not_be_named_value_utils.h"

#include "Variable.h"
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/domain_utils.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Testing methods from ShallNotBeNamedValueUtils

class ShallNotBeNamedValueUtilsTest : public ::testing::Test
{
public:
};

//! Importing xml Procedure containing a single instruction.

TEST_F(ShallNotBeNamedValueUtilsTest, GetJsonString)
{
  auto variable = DomainUtils::CreateDomainVariable(domainconstants::kLocalVariableType);
  variable->AddAttribute(domainconstants::kTypeAttribute, R"({"type":"uint32"})");
  variable->AddAttribute(domainconstants::kValueAttribute, "42");
  variable->Setup();

  ::ccs::types::AnyValue any_value;
  EXPECT_TRUE(variable->GetValue(any_value));
  ::ccs::types::uint32 value = any_value;
  EXPECT_EQ(value, 42);

  EXPECT_TRUE(variable->GetValue(any_value));

  EXPECT_EQ(DomainUtils::GetJsonString(&any_value), "42");
}
