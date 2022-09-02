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

#include "anyvalueeditor/domain_anyvalue_builder.h"

#include <anyvalueeditor/anyvalue_item.h>
#include <gtest/gtest.h>
#include <sup/dto/anyvalue.h>

using namespace anyvalueeditor;

class DomainAnyValueBuilderTest : public ::testing::Test
{
public:
  sup::dto::AnyValue CreateAnyValue(const AnyValueItem& item)
  {
    DomainAnyValueBuilder builder(item);
    return builder.GetAnyValue();
  }
};

//! Build from empty AnyValueItem.

TEST_F(DomainAnyValueBuilderTest, EmptyValue)
{
  AnyValueItem item("test");

  auto any_value = CreateAnyValue(item);
  EXPECT_TRUE(sup::dto::IsEmptyValue(any_value));
}

//! Build AnyValue from AnyValueItem representing a scalar.
//! More similar tests in ScalarConversionUtilsTests::GetAnyValueFromScalar.

TEST_F(DomainAnyValueBuilderTest, FromScalar)
{
  {  // from bool
    AnyValueScalarItem item;
    item.SetAnyTypeName(sup::dto::kBooleanTypeName);
    item.SetData(true);
    sup::dto::AnyValue expected_anyvalue{sup::dto::BooleanType, true};
    auto any_value = CreateAnyValue(item);
    EXPECT_EQ(any_value, expected_anyvalue);
  }

  {  // from int32
    AnyValueScalarItem item;
    item.SetAnyTypeName(sup::dto::kInt32TypeName);
    item.SetData(42);
    sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 42};
    auto any_value = CreateAnyValue(item);
    EXPECT_EQ(any_value, expected_anyvalue);
  }
}

TEST_F(DomainAnyValueBuilderTest, FromEmptyStruct)
{
  {  // empty struct
    AnyValueStructItem item;
    sup::dto::AnyValue expected_anyvalue{::sup::dto::EmptyStruct()};
    auto any_value = CreateAnyValue(item);
    EXPECT_EQ(any_value, expected_anyvalue);
  }

  {  // empty named struct
    AnyValueStructItem item;
    item.SetAnyTypeName("my_type");
    sup::dto::AnyValue expected_anyvalue{::sup::dto::EmptyStruct("my_type")};
    auto any_value = CreateAnyValue(item);
    EXPECT_EQ(any_value, expected_anyvalue);
  }
}

TEST_F(DomainAnyValueBuilderTest, FromStructWithSingleScalar)
{
  AnyValueStructItem item;
  item.AddScalarField("signed", sup::dto::kInt32TypeName, 42);

  sup::dto::AnyValue expected_anyvalue = {{{"signed", {sup::dto::SignedInteger32Type, 42}}}};
  auto any_value = CreateAnyValue(item);
  EXPECT_EQ(any_value, expected_anyvalue);
}

TEST_F(DomainAnyValueBuilderTest, FromStructWithTwoScalars)
{
  AnyValueStructItem item;
  item.SetAnyTypeName("my_struct");
  item.AddScalarField("signed", sup::dto::kInt32TypeName, 42);
  item.AddScalarField("bool", sup::dto::kBooleanTypeName, true);

  sup::dto::AnyValue expected_anyvalue = {
      {{"signed", {sup::dto::SignedInteger32Type, 42}}, {"bool", {sup::dto::BooleanType, true}}}, "my_struct"};

  auto any_value = CreateAnyValue(item);
  EXPECT_EQ(any_value, expected_anyvalue);
}
