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

#include "anyvalueeditor/anyvalue_item.h"

#include <anyvalueeditor/anyvalue_utils.h>
#include <gtest/gtest.h>
#include <sup/dto/anytype.h>

namespace
{
const int kAnyTypeNameRole = 10;  // defined in anyvalue_item.cpp
}

using namespace anyvalueeditor;

class AnyValueItemTest : public ::testing::Test
{
public:
};

TEST_F(AnyValueItemTest, InitialState)
{
  {  // AnyValueItem
    AnyValueItem item("test");
    EXPECT_FALSE(item.IsScalar());
    EXPECT_FALSE(item.IsStruct());
    EXPECT_FALSE(item.IsArray());
    EXPECT_TRUE(item.GetAnyTypeName().empty());
    EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
    EXPECT_FALSE(item.HasData(kAnyTypeNameRole));
  }

  {  // AnyValueScalarItem
    AnyValueScalarItem item;
    EXPECT_TRUE(item.IsScalar());
    EXPECT_FALSE(item.IsStruct());
    EXPECT_FALSE(item.IsArray());
    EXPECT_TRUE(item.GetAnyTypeName().empty());
    EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
    EXPECT_FALSE(item.HasData(kAnyTypeNameRole));
  }

  {  // AnyValueStructItem
    AnyValueStructItem item;
    EXPECT_FALSE(item.IsScalar());
    EXPECT_TRUE(item.IsStruct());
    EXPECT_FALSE(item.IsArray());
    EXPECT_EQ(item.GetAnyTypeName(), std::string());
    EXPECT_EQ(item.GetDisplayName(), kStructTypeName);
    EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
  }

  {  // AnyValueArrayItem
    AnyValueArrayItem item;
    EXPECT_FALSE(item.IsScalar());
    EXPECT_FALSE(item.IsStruct());
    EXPECT_TRUE(item.IsArray());
    EXPECT_EQ(item.GetAnyTypeName(), std::string());
    EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
    EXPECT_FALSE(item.HasData(kAnyTypeNameRole));
  }
}

TEST_F(AnyValueItemTest, SetAnyTypeName)
{
  {  // AnyValueScalarItem
    AnyValueScalarItem item;

    item.SetAnyTypeName(sup::dto::kInt8TypeName);

    EXPECT_TRUE(item.IsScalar());
    EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);
    EXPECT_EQ(item.Data<int>(), 0);
    EXPECT_TRUE(mvvm::utils::IsValid(item.Data()));
    EXPECT_TRUE(item.HasData(mvvm::DataRole::kData));
    EXPECT_TRUE(item.HasData(kAnyTypeNameRole));
  }

  {  // AnyValueStructItem
    AnyValueStructItem item;

    item.SetAnyTypeName("abc");

    EXPECT_FALSE(item.IsScalar());
    EXPECT_TRUE(item.IsStruct());
    EXPECT_EQ(item.GetAnyTypeName(), std::string("abc"));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
  }
}

TEST_F(AnyValueItemTest, AddScalarField)
{
  AnyValueStructItem item;
  auto scalar = item.AddScalarField("signed", sup::dto::kInt32TypeName, 42);

  EXPECT_EQ(scalar->GetDisplayName(), "signed");
  EXPECT_EQ(scalar->GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(scalar->Data<int>(), 42);
}
