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

#include "anyvalueeditor/anyvalue_build_adapter_v2.h"

#include <gtest/gtest.h>
#include <sup/dto/anytype.h>
#include <sup/dto/anytype_helper.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>

using namespace anyvalueeditor;

class AnyValueBuildAdapterV2Tests : public ::testing::Test
{
};

//! Checking that the builder returns an empty value.

TEST_F(AnyValueBuildAdapterV2Tests, InitialState)
{
  AnyValueBuildAdapterV2 builder;
  auto value = builder.MoveAnyValue();

  EXPECT_TRUE(::sup::dto::IsEmptyValue(value));
}

//! Creation of AnyValue scalar.

TEST_F(AnyValueBuildAdapterV2Tests, Scalar)
{
  AnyValueBuildAdapterV2 builder;

  builder.Int32(42);

  auto value = builder.MoveAnyValue();
  EXPECT_EQ(value.GetType(), sup::dto::SignedInteger32Type);
  EXPECT_TRUE(::sup::dto::IsScalarValue(value));
  EXPECT_EQ(value.As<sup::dto::int32>(), 42);
}

//! Creation of AnyValue scalar via AddMember method with empty name.

TEST_F(AnyValueBuildAdapterV2Tests, ScalarViaAddMember)
{
  AnyValueBuildAdapterV2 builder;

  sup::dto::AnyValue expected_anyvalue{sup::dto::StringType, std::string("abc")};

  // By passing an empty member name we tell the builder that this will be the scalar.
  builder.AddValue(expected_anyvalue);

  auto value = builder.MoveAnyValue();
  EXPECT_EQ(value, expected_anyvalue);
}

//! Empty structure.

TEST_F(AnyValueBuildAdapterV2Tests, EmptyStruct)
{
  AnyValueBuildAdapterV2 builder;

  builder.StartStruct("struct_name");
  builder.EndStruct();

  auto value = builder.MoveAnyValue();
  EXPECT_EQ(value, sup::dto::EmptyStruct("struct_name"));
}
