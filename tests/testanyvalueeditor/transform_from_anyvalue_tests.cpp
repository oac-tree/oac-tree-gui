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

#include "mvvm/core/variant.h"

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

TEST_F(TransformFromAnyValueTest, PopulateFromNestedStruct)
{
  AnyValueItem item;

  sup::dto::AnyValue two_scalars = {
      {{"signed", {sup::dto::SignedInteger8Type, 1}}, {"bool", {sup::dto::BooleanType, 12}}}};
  sup::dto::AnyValue anyvalue{{
      {"scalars", two_scalars},
  }};

  PopulateItem(&anyvalue, &item);
  EXPECT_EQ(item.GetTotalItemCount(), 1);
  EXPECT_EQ(item.GetDisplayName(), "AnyValue");
  EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));

  auto child = item.GetItem("", 0);
  EXPECT_EQ(child->GetTotalItemCount(), 2);
  EXPECT_EQ(child->GetDisplayName(), "scalars");
  EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));

  auto grandchild0 = child->GetItem("", 0);
  EXPECT_EQ(grandchild0->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild0->GetDisplayName(), "signed");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild0->Data()), mvvm::constants::kIntTypeName);

  auto grandchild1 = child->GetItem("", 1);
  EXPECT_EQ(grandchild1->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild1->GetDisplayName(), "bool");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild1->Data()), mvvm::constants::kBoolTypeName);
}

TEST_F(TransformFromAnyValueTest, PopulateFromTwoNestedStruct)
{
  AnyValueItem item;

  const std::string nested_name = "nested_struct";
  sup::dto::AnyValue two_scalars = {
      {{"signed", {sup::dto::SignedInteger8Type, 1}}, {"unsigned", {sup::dto::UnsignedInteger8Type, 12}}}};
  sup::dto::AnyValue anyvalue{
      {{"scalars", two_scalars},
       {"single",
        {{"first", {sup::dto::SignedInteger8Type, 0}}, {"second", {sup::dto::SignedInteger8Type, 5}}}}},
      nested_name};

  PopulateItem(&anyvalue, &item);
  EXPECT_EQ(item.GetTotalItemCount(), 2);
  EXPECT_EQ(item.GetDisplayName(), "AnyValue");
  EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));

  // first branch
  auto child0 = item.GetItem("", 0);
  EXPECT_EQ(child0->GetTotalItemCount(), 2);
  EXPECT_EQ(child0->GetDisplayName(), "scalars");
  EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));

  auto grandchild0 = child0->GetItem("", 0);
  EXPECT_EQ(grandchild0->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild0->GetDisplayName(), "signed");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild0->Data()), mvvm::constants::kIntTypeName);

  auto grandchild1 = child0->GetItem("", 1);
  EXPECT_EQ(grandchild1->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild1->GetDisplayName(), "unsigned");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild1->Data()), mvvm::constants::kIntTypeName);

  // second branch
  auto child1 = item.GetItem("", 1);
  EXPECT_EQ(child1->GetTotalItemCount(), 2);
  EXPECT_EQ(child1->GetDisplayName(), "single");
  EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));

  grandchild0 = child1->GetItem("", 0);
  EXPECT_EQ(grandchild0->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild0->GetDisplayName(), "first");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild0->Data()), mvvm::constants::kIntTypeName);

  grandchild1 = child1->GetItem("", 1);
  EXPECT_EQ(grandchild1->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild1->GetDisplayName(), "second");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild1->Data()), mvvm::constants::kIntTypeName);
}

TEST_F(TransformFromAnyValueTest, PopulateFromArrayOfIntegers)
{
  AnyValueItem item;

  sup::dto::AnyValue anyvalue =
      sup::dto::ArrayValue({{sup::dto::SignedInteger64Type, 1}, 2}, "my_array_t");

  PopulateItem(&anyvalue, &item);

  EXPECT_EQ(item.GetTotalItemCount(), 2);
  EXPECT_EQ(item.GetDisplayName(), "AnyValue");
  EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));

  // first branch
  auto child0 = item.GetItem("", 0);
  EXPECT_EQ(child0->GetTotalItemCount(), 0);
  EXPECT_EQ(child0->GetDisplayName(), "index0");
  EXPECT_EQ(mvvm::utils::TypeName(child0->Data()), mvvm::constants::kIntTypeName);
  EXPECT_EQ(child0->Data<int>(), 1);

  // second branch
  auto child1 = item.GetItem("", 1);
  EXPECT_EQ(child1->GetTotalItemCount(), 0);
  EXPECT_EQ(child1->GetDisplayName(), "index1");
  EXPECT_EQ(mvvm::utils::TypeName(child0->Data()), mvvm::constants::kIntTypeName);
  EXPECT_EQ(child1->Data<int>(), 2);

}
