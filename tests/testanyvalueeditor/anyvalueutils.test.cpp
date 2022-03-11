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

#include "anyvalueeditor/anyvalueutils.h"

#include "AnyValue.h"
#include "AnyValueHelper.h"
#include "anyvalueeditor/anyvalueitem.h"

#include <gtest/gtest.h>

using namespace anyvalueeditor;

class AnyValueItemUtilsTest : public ::testing::Test
{
public:
};

//! Checking function to get variant_t from sup::dto type names.

TEST_F(AnyValueItemUtilsTest, GetVariantForAnyTypeName)
{
  EXPECT_TRUE(std::holds_alternative<bool>(GetVariantForAnyValueTypeName(kBooleanTypeName)));

  // FIXME How to edit char8 in UI widgets?
  //  EXPECT_TRUE(std::holds_alternative<char>(GetVariantForAnyValueTypeName(kChar8TypeName)));

  // We use `int` to edit all integers in the UI. It is assumed, that allowed minimum, and maximum
  // of values will be handled on the level of cell editors.
  EXPECT_TRUE(std::holds_alternative<int>(GetVariantForAnyValueTypeName(kInt8TypeName)));
  EXPECT_TRUE(std::holds_alternative<int>(GetVariantForAnyValueTypeName(kUInt8TypeName)));
  EXPECT_TRUE(std::holds_alternative<int>(GetVariantForAnyValueTypeName(kInt16TypeName)));
  EXPECT_TRUE(std::holds_alternative<int>(GetVariantForAnyValueTypeName(kUInt16TypeName)));
  EXPECT_TRUE(std::holds_alternative<int>(GetVariantForAnyValueTypeName(kInt32TypeName)));
  EXPECT_TRUE(std::holds_alternative<int>(GetVariantForAnyValueTypeName(kUint32TypeName)));
  EXPECT_TRUE(std::holds_alternative<int>(GetVariantForAnyValueTypeName(kInt64TypeName)));
  EXPECT_TRUE(std::holds_alternative<int>(GetVariantForAnyValueTypeName(kUint64TypeName)));

  // We use `double` to edit float values in UI.
  EXPECT_TRUE(std::holds_alternative<double>(GetVariantForAnyValueTypeName(kFloat32TypeName)));
  EXPECT_TRUE(std::holds_alternative<double>(GetVariantForAnyValueTypeName(kFloat64TypeName)));

  EXPECT_TRUE(std::holds_alternative<std::string>(GetVariantForAnyValueTypeName(kStringTypeName)));
}

TEST_F(AnyValueItemUtilsTest, DefaultVariantValuesConstructedFromAnyTypeName)
{
  EXPECT_EQ(std::get<bool>(GetVariantForAnyValueTypeName(kBooleanTypeName)), false);

  // We use `int` to edit all integers in the UI. It is assumed, that allowed minimum, and maximum
  // of values will be handled on the level of cell editors.
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(kInt8TypeName)), 0);
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(kUInt8TypeName)), 0);
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(kInt16TypeName)), 0);
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(kUInt16TypeName)), 0);
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(kInt32TypeName)), 0);
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(kUint32TypeName)), 0);
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(kInt64TypeName)), 0);
  EXPECT_EQ(std::get<int>(GetVariantForAnyValueTypeName(kUint64TypeName)), 0);

  // We use `double` to edit float values in UI.
  EXPECT_EQ(std::get<double>(GetVariantForAnyValueTypeName(kFloat32TypeName)), 0);
  EXPECT_EQ(std::get<double>(GetVariantForAnyValueTypeName(kFloat64TypeName)), 0);

  EXPECT_TRUE(std::holds_alternative<std::string>(GetVariantForAnyValueTypeName(kStringTypeName)));
}
