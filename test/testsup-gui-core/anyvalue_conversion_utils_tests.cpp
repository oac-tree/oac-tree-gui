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

#include "sup/gui/model/anyvalue_conversion_utils.h"

#include <gtest/gtest.h>

#include <sup/dto/anytype.h>

using namespace sup::gui;

class AnyValueConversionUtilsTest : public ::testing::Test
{
public:
};

TEST_F(AnyValueConversionUtilsTest, GetTypeCode)
{
  using sup::dto::TypeCode;

  EXPECT_EQ(GetTypeCode(kStructTypeName), TypeCode::Struct);
  EXPECT_EQ(GetTypeCode(kArrayTypeName), TypeCode::Array);

  EXPECT_EQ(GetTypeCode(sup::dto::kEmptyTypeName), TypeCode::Empty);
  EXPECT_EQ(GetTypeCode(sup::dto::kBooleanTypeName), TypeCode::Bool);
  EXPECT_EQ(GetTypeCode(sup::dto::kChar8TypeName), TypeCode::Char8);
  EXPECT_EQ(GetTypeCode(sup::dto::kInt8TypeName), TypeCode::Int8);
  EXPECT_EQ(GetTypeCode(sup::dto::kUInt8TypeName), TypeCode::UInt8);
  EXPECT_EQ(GetTypeCode(sup::dto::kInt16TypeName), TypeCode::Int16);
  EXPECT_EQ(GetTypeCode(sup::dto::kUInt16TypeName), TypeCode::UInt16);
  EXPECT_EQ(GetTypeCode(sup::dto::kInt32TypeName), TypeCode::Int32);
  EXPECT_EQ(GetTypeCode(sup::dto::kUInt32TypeName), TypeCode::UInt32);
  EXPECT_EQ(GetTypeCode(sup::dto::kInt64TypeName), TypeCode::Int64);
  EXPECT_EQ(GetTypeCode(sup::dto::kUInt64TypeName), TypeCode::UInt64);
  EXPECT_EQ(GetTypeCode(sup::dto::kFloat32TypeName), TypeCode::Float32);
  EXPECT_EQ(GetTypeCode(sup::dto::kFloat64TypeName), TypeCode::Float64);
  EXPECT_EQ(GetTypeCode(sup::dto::kStringTypeName), TypeCode::String);

  EXPECT_THROW(GetTypeCode("non-existing"), std::runtime_error);
}

//! Checking function to get variant_t from sup::dto type names.

TEST_F(AnyValueConversionUtilsTest, GetVariantForAnyTypeName)
{
  EXPECT_TRUE(
      std::holds_alternative<bool>(GetVariantForAnyValueTypeName(sup::dto::kBooleanTypeName)));

  // FIXME How to edit char8 in UI widgets?
  //  EXPECT_TRUE(std::holds_alternative<char>(GetVariantForAnyValueTypeName(kChar8TypeName)));

  // We use `int` to edit all integers in the UI. It is assumed, that allowed minimum, and maximum
  // of values will be handled on the level of cell editors.
  EXPECT_TRUE(std::holds_alternative<int>(GetVariantForAnyValueTypeName(sup::dto::kInt8TypeName)));
  EXPECT_TRUE(std::holds_alternative<int>(GetVariantForAnyValueTypeName(sup::dto::kUInt8TypeName)));
  EXPECT_TRUE(std::holds_alternative<int>(GetVariantForAnyValueTypeName(sup::dto::kInt16TypeName)));
  EXPECT_TRUE(
      std::holds_alternative<int>(GetVariantForAnyValueTypeName(sup::dto::kUInt16TypeName)));
  EXPECT_TRUE(std::holds_alternative<int>(GetVariantForAnyValueTypeName(sup::dto::kInt32TypeName)));
  EXPECT_TRUE(
      std::holds_alternative<int>(GetVariantForAnyValueTypeName(sup::dto::kUInt32TypeName)));
  EXPECT_TRUE(std::holds_alternative<int>(GetVariantForAnyValueTypeName(sup::dto::kInt64TypeName)));
  EXPECT_TRUE(
      std::holds_alternative<int>(GetVariantForAnyValueTypeName(sup::dto::kUInt64TypeName)));

  // We use `double` to edit float values in UI.
  EXPECT_TRUE(
      std::holds_alternative<double>(GetVariantForAnyValueTypeName(sup::dto::kFloat32TypeName)));
  EXPECT_TRUE(
      std::holds_alternative<double>(GetVariantForAnyValueTypeName(sup::dto::kFloat64TypeName)));

  EXPECT_TRUE(std::holds_alternative<std::string>(
      GetVariantForAnyValueTypeName(sup::dto::kStringTypeName)));
}

TEST_F(AnyValueConversionUtilsTest, DefaultVariantValuesConstructedFromAnyTypeName)
{
  EXPECT_EQ(std::get<bool>(GetVariantForAnyValueTypeName(sup::dto::kBooleanTypeName)), false);

  // We use `int` to edit all integers in the UI. It is assumed, that allowed minimum, and maximum
  // of values will be handled on the level of cell editors.
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(sup::dto::kInt8TypeName)), 0);
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(sup::dto::kUInt8TypeName)), 0);
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(sup::dto::kInt16TypeName)), 0);
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(sup::dto::kUInt16TypeName)), 0);
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(sup::dto::kInt32TypeName)), 0);
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(sup::dto::kUInt32TypeName)), 0);
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(sup::dto::kInt64TypeName)), 0);
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(sup::dto::kUInt64TypeName)), 0);

  // We use `double` to edit float values in UI.
  EXPECT_EQ(std::get<double>(GetVariantForAnyValueTypeName(sup::dto::kFloat32TypeName)), 0);
  EXPECT_EQ(std::get<double>(GetVariantForAnyValueTypeName(sup::dto::kFloat64TypeName)), 0);

  EXPECT_TRUE(std::holds_alternative<std::string>(
      GetVariantForAnyValueTypeName(sup::dto::kStringTypeName)));
}

//! Testing IsScalarTypeName utility function.

TEST_F(AnyValueConversionUtilsTest, IsScalarTypeName)
{
  EXPECT_FALSE(IsScalarTypeName(std::string("")));

  EXPECT_FALSE(IsScalarTypeName(kStructTypeName));
  EXPECT_FALSE(IsScalarTypeName(kArrayTypeName));
  EXPECT_FALSE(IsScalarTypeName(kScalarTypeName));

  EXPECT_TRUE(IsScalarTypeName(sup::dto::kInt8TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kUInt8TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kInt16TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kUInt16TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kInt32TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kUInt32TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kInt64TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kUInt64TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kFloat32TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kFloat64TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kStringTypeName));
}

//! Testing IsStructTypeName utility function.

TEST_F(AnyValueConversionUtilsTest, IsStructTypeName)
{
  EXPECT_FALSE(IsStructTypeName(std::string("")));

  EXPECT_TRUE(IsStructTypeName(kStructTypeName));
  EXPECT_FALSE(IsStructTypeName(kArrayTypeName));
  EXPECT_FALSE(IsStructTypeName(kScalarTypeName));

  EXPECT_FALSE(IsStructTypeName(sup::dto::kInt8TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kUInt8TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kInt16TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kUInt16TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kInt32TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kUInt32TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kInt64TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kUInt64TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kFloat32TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kFloat64TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kStringTypeName));
}
