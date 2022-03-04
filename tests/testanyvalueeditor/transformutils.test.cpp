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

#include "anyvalueeditor/transformutils.h"

#include "AnyValue.h"
#include "BasicScalarTypes.h"
#include "anyvalueeditor/anyvalueitem.h"

#include "mvvm/core/variant.h"

#include <gtest/gtest.h>

using namespace anyvalueeditor;

class TransformUtilsTest : public ::testing::Test
{
};

TEST_F(TransformUtilsTest, TransformScalar)
{
  {  // bool
    sup::dto::AnyValue anyvalue_value{sup::dto::Boolean};
    anyvalue_value = true;
    AnyValueItem item;
    ScalarToItem(anyvalue_value, item);
    EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kBoolTypeName);
    EXPECT_TRUE(item.Data<bool>());
    EXPECT_EQ(item.GetTotalItemCount(), 0);
  }

  {  // int32
    sup::dto::AnyValue anyvalue_value{sup::dto::SignedInteger32};
    anyvalue_value = 42;
    AnyValueItem item;
    ScalarToItem(anyvalue_value, item);
    EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kIntTypeName);
    EXPECT_EQ(item.Data<int>(), 42);
    EXPECT_EQ(item.GetTotalItemCount(), 0);
  }
}

//! Building AnyValueItem from AnyValue with two named scalars.
//! More tests in anyvalueitembuilder.test.cpp

TEST_F(TransformUtilsTest, TwoScalars)
{
  AnyValueItem item;

  sup::dto::AnyValue anyvalue = {
      {{"signed", {sup::dto::SignedInteger32, 42}}, {"bool", {sup::dto::Boolean, true}}}};

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

TEST_F(TransformUtilsTest, NestedStruct)
{
  AnyValueItem item;

  sup::dto::AnyValue two_scalars = {
      {{"signed", {sup::dto::SignedInteger8, 1}}, {"bool", {sup::dto::Boolean, 12}}}};
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
