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

#include "sup/gui/dto/scalar_conversion_utils.h"

#include <gtest/gtest.h>

#include <mvvm/core/variant.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/basic_scalar_types.h>
#include <sup/gui/dto/anyvalue_item.h>
#include <sup/gui/dto/conversion_utils.h>

using namespace sup::gui;

class ScalarConversionUtilsTests : public ::testing::Test
{
};

//! Testing SetDataFromScalar method: setting AnyValueItem's data from AnyValue containing a
//! boolean.

TEST_F(ScalarConversionUtilsTests, SetDataFromScalarBool)
{
  sup::dto::AnyValue anyvalue{sup::dto::BooleanType};
  anyvalue = true;
  AnyValueScalarItem item;
  SetDataFromScalar(anyvalue, item);
  EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kBoolVariantName);
  EXPECT_TRUE(item.Data<bool>());
  EXPECT_EQ(item.GetTotalItemCount(), 0);
}

//! Testing SetDataFromScalar method: setting AnyValueItem's data from AnyValue containing an
//! integer.

TEST_F(ScalarConversionUtilsTests, SetDataFromScalarInt)
{
  sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type};
  anyvalue = 42;
  AnyValueScalarItem item;
  SetDataFromScalar(anyvalue, item);
  EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kIntVariantName);
  EXPECT_EQ(item.Data<int>(), 42);
  EXPECT_EQ(item.GetTotalItemCount(), 0);
}

//! Testing GetAnyValueFromScalar method. Creating scalar AnyValue from scalar-like AnyValueItem
//! containing various scalars.

TEST_F(ScalarConversionUtilsTests, GetAnyValueFromScalar)
{
  {  // boolean
    AnyValueScalarItem item;
    item.SetAnyTypeName(sup::dto::kBooleanTypeName);
    item.SetData(true);

    auto any_value = GetAnyValueFromScalar(item);
    EXPECT_EQ(any_value.GetType(), sup::dto::BooleanType);
    EXPECT_EQ(any_value, true);
  }

  {  // int32
    AnyValueScalarItem item;
    item.SetAnyTypeName(sup::dto::kInt32TypeName);
    item.SetData(42);

    auto any_value = GetAnyValueFromScalar(item);
    EXPECT_EQ(any_value.GetType(), sup::dto::SignedInteger32Type);
    EXPECT_EQ(any_value, 42);
  }

  {  // double
    AnyValueScalarItem item;
    item.SetAnyTypeName(sup::dto::kFloat64TypeName);
    item.SetData(42.1);

    auto any_value = GetAnyValueFromScalar(item);
    EXPECT_EQ(any_value.GetType(), sup::dto::Float64Type);
    EXPECT_EQ(any_value, 42.1);
  }

  {  // attempt to construct a scalar from the struct
    AnyValueStructItem item;
    EXPECT_THROW(GetAnyValueFromScalar(item), std::runtime_error);
  }
}
