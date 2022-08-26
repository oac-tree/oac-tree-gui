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

#include "anyvalueeditor/transform_from_anyvalue.h"

#include "sup/dto/anyvalue.h"
#include "sup/dto/basic_scalar_types.h"
#include "anyvalueeditor/anyvalue_item.h"

#include <mvvm/core/variant.h>

#include <gtest/gtest.h>

using namespace anyvalueeditor;

class TransformFromAnyValueTest : public ::testing::Test
{
};

TEST_F(TransformFromAnyValueTest, BoolScalarToItem)
{
  sup::dto::AnyValue anyvalue{sup::dto::BooleanType};
  anyvalue = true;
  AnyValueItem item;
  ScalarToItem(anyvalue, item);
  EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kBoolTypeName);
  EXPECT_TRUE(item.Data<bool>());
  EXPECT_EQ(item.GetTotalItemCount(), 0);
}

TEST_F(TransformFromAnyValueTest, IntScalarToItem)
{
  sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type};
  anyvalue = 42;
  AnyValueItem item;
  ScalarToItem(anyvalue, item);
  EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kIntTypeName);
  EXPECT_EQ(item.Data<int>(), 42);
  EXPECT_EQ(item.GetTotalItemCount(), 0);
}

TEST_F(TransformFromAnyValueTest, PopulateFromIntScalar)
{
  sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type};
  anyvalue = 42;
  AnyValueItem item;

  PopulateItem(&anyvalue, &item);

  EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kIntTypeName);
  EXPECT_EQ(item.Data<int>(), 42);
  EXPECT_EQ(item.GetTotalItemCount(), 0);
}

//! Building AnyValueItem from AnyValue with two named scalars.
//! More tests in anyvalue_item_builder_tests.cpp

TEST_F(TransformFromAnyValueTest, PopulateFromTwoScalars)
{
  AnyValueItem item;

  sup::dto::AnyValue anyvalue = {
      {{"signed", {sup::dto::SignedInteger32Type, 42}}, {"bool", {sup::dto::BooleanType, true}}}};

  PopulateItem(&anyvalue, &item);
  EXPECT_EQ(item.GetTotalItemCount(), 2);
  EXPECT_EQ(item.GetDisplayName(), "AnyValue");
  EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));

  auto child = item.GetItem("", 0);
  EXPECT_EQ(child->GetTotalItemCount(), 0);
  EXPECT_EQ(child->GetDisplayName(), "signed");
  EXPECT_EQ(mvvm::utils::TypeName(child->Data()), mvvm::constants::kIntTypeName);

  child = item.GetItem("", 1);
  EXPECT_EQ(child->GetTotalItemCount(), 0);
  EXPECT_EQ(child->GetDisplayName(), "bool");
  EXPECT_EQ(mvvm::utils::TypeName(child->Data()), mvvm::constants::kBoolTypeName);
}
