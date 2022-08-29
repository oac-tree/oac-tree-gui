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

#include "anyvalueeditor/anyvalue_utils.h"

#include <gtest/gtest.h>
#include <sup/dto/anytype.h>

using namespace anyvalueeditor;

class AnyValueItemUtilsTest : public ::testing::Test
{
public:
};

//! Checking function to get variant_t from sup::dto type names.

TEST_F(AnyValueItemUtilsTest, GetVariantForAnyTypeName)
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

TEST_F(AnyValueItemUtilsTest, DefaultVariantValuesConstructedFromAnyTypeName)
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
