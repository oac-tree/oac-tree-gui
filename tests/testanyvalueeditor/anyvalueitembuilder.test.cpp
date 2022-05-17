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

#include "anyvalueeditor/anyvalueitembuilder.h"

#include "sup/dto/AnyValue.h"
#include "sup/dto/AnyValueHelper.h"
#include "anyvalueeditor/anyvalueitem.h"

#include <gtest/gtest.h>

using namespace anyvalueeditor;

class AnyValueItemBuilderTest : public ::testing::Test
{
public:
  void ProcessValue(const sup::dto::AnyValue& value, AnyValueItem& item)
  {
    AnyValueItemBuilder m_builder(&item);
    sup::dto::SerializeAnyValue(value, m_builder);
  }
};

//! Building AnyValueItem from scalar based AnyValues.
//! More similar tests in transformutils.test.cpp

TEST_F(AnyValueItemBuilderTest, ScalarValues)
{
  {  // bool
    AnyValueItem item;
    sup::dto::AnyValue anyvalue{sup::dto::Boolean};
    anyvalue = true;
    ProcessValue(anyvalue, item);
    EXPECT_EQ(item.GetTotalItemCount(), 0);
    EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kBoolTypeName);
    EXPECT_TRUE(item.Data<bool>());
  }

  {  // int
    AnyValueItem item;
    sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32};
    anyvalue = 42;
    ProcessValue(anyvalue, item);
    EXPECT_EQ(item.GetTotalItemCount(), 0);
    EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kIntTypeName);
    EXPECT_EQ(item.Data<int>(), 42);
  }
}

//! Building AnyValueItem from AnyValue with two named scalars.

TEST_F(AnyValueItemBuilderTest, TwoScalars)
{
  AnyValueItem item;

  sup::dto::AnyValue anyvalue = {
      {{"signed", {sup::dto::SignedInteger32, 42}}, {"bool", {sup::dto::Boolean, true}}}};

  ProcessValue(anyvalue, item);
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
