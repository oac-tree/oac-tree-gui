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
  }

  {  // int32
    sup::dto::AnyValue anyvalue_value{sup::dto::SignedInteger32};
    anyvalue_value = 42;
    AnyValueItem item;
    ScalarToItem(anyvalue_value, item);
    EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kIntTypeName);
    EXPECT_EQ(item.Data<int>(), 42);
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
