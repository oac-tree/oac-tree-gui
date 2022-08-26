/******************************************************************************
 *
 * Project       : Supervision and automation system EPICS interface
 *
 * Description   : Library of SUP components for EPICS network protocol
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

#include "anyvalueeditor/anyvalue_build_adapter.h"

#include <gtest/gtest.h>
#include <sup/dto/anytype_helper.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>

using namespace anyvalueeditor;

class AnyValueBuildAdapterTests : public ::testing::Test
{
};

//! Checking that the builder returns an empty value.

TEST_F(AnyValueBuildAdapterTests, InitialState)
{
  AnyValueBuildAdapter builder;
  auto value = builder.MoveAnyValue();

  EXPECT_TRUE(::sup::dto::IsEmptyValue(value));
}

//! Creation of AnyValue scalar.

TEST_F(AnyValueBuildAdapterTests, Scalar)
{
  AnyValueBuildAdapter builder;

  // By passing an empty member name we tell the builder that this will be the scalar.
  builder.Int32("", 42);

  auto value = builder.MoveAnyValue();
  EXPECT_EQ(value.GetType(), sup::dto::SignedInteger32Type);
  EXPECT_TRUE(::sup::dto::IsScalarValue(value));
  EXPECT_EQ(value.As<sup::dto::int32>(), 42);
}

//! Creation of AnyValue containing a struct with single field.

TEST_F(AnyValueBuildAdapterTests, StructWithSingleField)
{
  sup::dto::AnyType expected_anytype = {{"signed", {sup::dto::SignedInteger32Type}}};

  AnyValueBuildAdapter builder;

  builder.StartStruct();
  builder.Int32("signed", 42);
  builder.EndStruct();

  auto value = builder.MoveAnyValue();
  EXPECT_EQ(value.GetType(), expected_anytype);
  EXPECT_TRUE(::sup::dto::IsStructValue(value));
  EXPECT_EQ(value["signed"].As<sup::dto::int32>(), 42);
}

//! Attempt to create a struct using empty string as a name.

TEST_F(AnyValueBuildAdapterTests, StructWithMissedFieldName)
{
  sup::dto::AnyType expected_anytype = {{"signed", {sup::dto::SignedInteger32Type}}};

  AnyValueBuildAdapter builder;

  builder.StartStruct();
  EXPECT_THROW(builder.Int32("", 42), std::runtime_error);
}

//! Creation of AnyValue containing a struct with two fields.

TEST_F(AnyValueBuildAdapterTests, StructWithTwoFields)
{
  sup::dto::AnyType expected_anytype = {{"signed", {sup::dto::SignedInteger32Type}},
                                        {"bool", {sup::dto::BooleanType}}};

  AnyValueBuildAdapter builder;

  builder.StartStruct();
  builder.Int32("signed", 42);
  builder.Bool("bool", true);
  builder.EndStruct();

  auto value = builder.MoveAnyValue();
  EXPECT_EQ(value.GetType(), expected_anytype);
  EXPECT_TRUE(::sup::dto::IsStructValue(value));
  EXPECT_EQ(value["signed"].As<sup::dto::int32>(), 42);
  EXPECT_EQ(value["bool"].As<sup::dto::boolean>(), true);
}

//! Creation of AnyValue containing a struct with two fields.
//! Same as before using method AddScalar and pre-constructed AnyValue scalars

TEST_F(AnyValueBuildAdapterTests, StructWithTwoFieldsViaAddScalar)
{
  sup::dto::AnyType expected_anytype = {{"signed", {sup::dto::SignedInteger32Type}},
                                        {"bool", {sup::dto::BooleanType}}};

  AnyValueBuildAdapter builder;

  builder.StartStruct();

  auto value1 = ::sup::dto::AnyValue(::sup::dto::SignedInteger32Type);
  value1 = 42;
  builder.AddMember("signed", value1);
  auto value2 = ::sup::dto::AnyValue(::sup::dto::BooleanType);
  value2 = true;
  builder.AddMember("bool", value2);
  builder.EndStruct();

  auto value = builder.MoveAnyValue();
  EXPECT_EQ(value.GetType(), expected_anytype);
  EXPECT_EQ(value.NumberOfMembers(), 2);
  EXPECT_TRUE(::sup::dto::IsStructValue(value));
  EXPECT_EQ(value["signed"].As<sup::dto::int32>(), 42);
  EXPECT_EQ(value["bool"].As<sup::dto::boolean>(), true);
}

//! Creation of AnyValue containing a struct with another struct in it.

TEST_F(AnyValueBuildAdapterTests, StructWithNestedStructWithField)
{
  sup::dto::AnyType two_scalars = {{"signed", {sup::dto::SignedInteger32Type}},
                                   {"bool", {sup::dto::BooleanType}}};
  sup::dto::AnyType expected_anytype = {{"scalars", two_scalars}};

  AnyValueBuildAdapter builder;

  builder.StartStruct();
  builder.StartStruct();
  builder.Int32("signed", 42);
  builder.Bool("bool", true);
  builder.EndStruct("scalars");
  builder.EndStruct();

  auto value = builder.MoveAnyValue();
  EXPECT_EQ(value.GetType(), expected_anytype);
  EXPECT_TRUE(::sup::dto::IsStructValue(value));
  EXPECT_TRUE(::sup::dto::IsStructValue(value["scalars"]));
  EXPECT_EQ(value["scalars.signed"].As<sup::dto::int32>(), 42);
  EXPECT_EQ(value["scalars.bool"].As<sup::dto::boolean>(), true);
}

//! Creation of AnyValue containing two structures.

TEST_F(AnyValueBuildAdapterTests, StructWithTwoNestedStructs)
{
  const std::string struct_name = "struct_name";
  sup::dto::AnyType two_scalars = {
      {{"signed", {sup::dto::SignedInteger32Type}}, {"bool", {sup::dto::BooleanType}}},
      "internal_struct"};

  sup::dto::AnyType expected_anytype{
      {{"struct1", two_scalars},
       {"struct2",
        {{"first", {sup::dto::SignedInteger8Type}}, {"second", {sup::dto::UnsignedInteger8Type}}}}},
      struct_name};

  AnyValueBuildAdapter builder;

  builder.StartStruct(struct_name);

  builder.StartStruct("internal_struct");
  builder.Int32("signed", 42);
  builder.Bool("bool", true);
  builder.EndStruct("struct1");

  builder.StartStruct();
  builder.Int8("first", -43);
  builder.UInt8("second", 44);
  builder.EndStruct("struct2");

  builder.EndStruct();

  auto value = builder.MoveAnyValue();
  EXPECT_EQ(value.GetType(), expected_anytype);
  EXPECT_TRUE(::sup::dto::IsStructValue(value));
  EXPECT_TRUE(::sup::dto::IsStructValue(value["struct1"]));
  EXPECT_TRUE(::sup::dto::IsStructValue(value["struct2"]));
  EXPECT_EQ(value["struct1.signed"].As<sup::dto::int32>(), 42);
  EXPECT_EQ(value["struct1.bool"].As<sup::dto::boolean>(), true);
  EXPECT_EQ(value["struct2.first"].As<sup::dto::int8>(), -43);
  EXPECT_EQ(value["struct2.second"].As<sup::dto::uint8>(), 44);
}

//! Same test as before when internal structs are added via AddMember method

TEST_F(AnyValueBuildAdapterTests, StructWithTwoNestedStructsViaAddMembers)
{
  const std::string struct_name = "struct_name";
  sup::dto::AnyType two_scalars = {
      {{"signed", {sup::dto::SignedInteger32Type}}, {"bool", {sup::dto::BooleanType}}},
      "internal_struct"};

  sup::dto::AnyType expected_anytype{
      {{"struct1", two_scalars},
       {"struct2",
        {{"first", {sup::dto::SignedInteger8Type}}, {"second", {sup::dto::UnsignedInteger8Type}}}}},
      struct_name};

  sup::dto::AnyValue struct1 = {
      {{"signed", {sup::dto::SignedInteger32Type, 42}}, {"bool", {sup::dto::BooleanType, true}}},
      "internal_struct"};

  sup::dto::AnyValue struct2 = {{"first", {sup::dto::SignedInteger8Type, -43}},
                                {"second", {sup::dto::UnsignedInteger8Type, 44}}};

  AnyValueBuildAdapter builder;

  builder.StartStruct(struct_name);

  builder.AddMember("struct1", struct1);
  builder.AddMember("struct2", struct2);

  builder.EndStruct();

  auto value = builder.MoveAnyValue();
  EXPECT_EQ(value.GetType(), expected_anytype);
  EXPECT_TRUE(::sup::dto::IsStructValue(value));
  EXPECT_TRUE(::sup::dto::IsStructValue(value["struct1"]));
  EXPECT_TRUE(::sup::dto::IsStructValue(value["struct2"]));
  EXPECT_EQ(value["struct1.signed"].As<sup::dto::int32>(), 42);
  EXPECT_EQ(value["struct1.bool"].As<sup::dto::boolean>(), true);
  EXPECT_EQ(value["struct2.first"].As<sup::dto::int8>(), -43);
  EXPECT_EQ(value["struct2.second"].As<sup::dto::uint8>(), 44);
}
