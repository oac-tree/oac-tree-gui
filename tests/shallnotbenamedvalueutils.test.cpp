/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/model/shallnotbenamedvalueutils.h"

#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/domain_constants.h"

#include <gtest/gtest.h>

using namespace sequi;

//! Testing methods from ShallNotBeNamedValueUtils

class ShallNotBeNamedValueUtilsTest : public ::testing::Test
{
public:

};

//! Importing xml Procedure containing a single instruction.

TEST_F(ShallNotBeNamedValueUtilsTest, GetJsonString)
{
  auto variable = DomainUtils::CreateDomainVariable(DomainConstants::kLocalVariableType);
  variable->AddAttribute(DomainConstants::kTypeAttribute, R"({"type":"uint32"})");
  variable->AddAttribute(DomainConstants::kValueAttribute, "42");

  ::ccs::types::AnyValue any_value;
  EXPECT_TRUE(variable->GetValue(any_value));
  ::ccs::types::uint32 value = any_value;
  EXPECT_EQ(value, 42);

  EXPECT_TRUE(variable->GetValue(any_value));

  EXPECT_EQ(DomainUtils::GetJsonString(&any_value), "42");
}
