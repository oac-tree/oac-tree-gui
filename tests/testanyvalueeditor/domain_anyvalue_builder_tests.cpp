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
      {{"signed", {sup::dto::SignedInteger32Type, 42}}, {"bool", {sup::dto::BooleanType, true}}},
      "my_struct"};

  auto any_value = CreateAnyValue(item);
  EXPECT_EQ(any_value, expected_anyvalue);
}

TEST_F(DomainAnyValueBuilderTest, FromNestedStruct)
{
  AnyValueStructItem item;
  auto inner_struct = item.InsertItem<AnyValueStructItem>(mvvm::TagIndex::Append());
  inner_struct->SetDisplayName("scalars");
  inner_struct->AddScalarField("signed", sup::dto::kInt8TypeName, 1);
  inner_struct->AddScalarField("bool", sup::dto::kBooleanTypeName, true);

  sup::dto::AnyValue two_scalars = {
      {{"signed", {sup::dto::SignedInteger8Type, 1}}, {"bool", {sup::dto::BooleanType, 12}}}};
  sup::dto::AnyValue expected_anyvalue{{
      {"scalars", two_scalars},
  }};

  auto any_value = CreateAnyValue(item);
  EXPECT_EQ(any_value, expected_anyvalue);
}

TEST_F(DomainAnyValueBuilderTest, FromTwoNestedStructs)
{
  AnyValueStructItem item;
  item.SetAnyTypeName("nested_struct");

  auto inner_struct0 = item.InsertItem<AnyValueStructItem>(mvvm::TagIndex::Append());
  inner_struct0->SetDisplayName("struct0");
  inner_struct0->AddScalarField("signed", sup::dto::kInt8TypeName, 1);
  inner_struct0->AddScalarField("unsigned", sup::dto::kUInt8TypeName, 12);

  auto inner_struct1 = item.InsertItem<AnyValueStructItem>(mvvm::TagIndex::Append());
  inner_struct1->SetDisplayName("struct1");
  inner_struct1->AddScalarField("first", sup::dto::kInt8TypeName, 0);
  inner_struct1->AddScalarField("second", sup::dto::kInt8TypeName, 5);

  const std::string nested_name = "nested_struct";
  sup::dto::AnyValue two_scalars = {{{"signed", {sup::dto::SignedInteger8Type, 1}},
                                     {"unsigned", {sup::dto::UnsignedInteger8Type, 12}}}};
  sup::dto::AnyValue expected_anyvalue{{{"struct0", two_scalars},
                                        {"struct1",
                                         {{"first", {sup::dto::SignedInteger8Type, 0}},
                                          {"second", {sup::dto::SignedInteger8Type, 5}}}}},
                                       nested_name};
  auto any_value = CreateAnyValue(item);
  EXPECT_EQ(any_value, expected_anyvalue);
}
