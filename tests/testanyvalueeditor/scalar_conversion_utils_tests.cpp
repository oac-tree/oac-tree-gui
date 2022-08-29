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

#include "anyvalueeditor/scalar_conversion_utils.h"

#include <sup/dto/anyvalue.h>
#include <sup/dto/basic_scalar_types.h>
#include <anyvalueeditor/anyvalue_item.h>

#include <mvvm/core/variant.h>

#include <gtest/gtest.h>

using namespace anyvalueeditor;

class ScalarConversionUtilsTests : public ::testing::Test
{
};

TEST_F(ScalarConversionUtilsTests, BoolScalarToItem)
{
  sup::dto::AnyValue anyvalue{sup::dto::BooleanType};
  anyvalue = true;
  AnyValueItem item;
  SetDataFromScalar(anyvalue, item);
  EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kBoolTypeName);
  EXPECT_TRUE(item.Data<bool>());
  EXPECT_EQ(item.GetTotalItemCount(), 0);
}

TEST_F(ScalarConversionUtilsTests, IntScalarToItem)
{
  sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type};
  anyvalue = 42;
  AnyValueItem item;
  SetDataFromScalar(anyvalue, item);
  EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kIntTypeName);
  EXPECT_EQ(item.Data<int>(), 42);
  EXPECT_EQ(item.GetTotalItemCount(), 0);
}
