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

#include "anyvalueeditor/anyvalue_utils.h"

#include <gtest/gtest.h>

using namespace anyvalueeditor;

class AnyValueItemTest : public ::testing::Test
{
public:
};

TEST_F(AnyValueItemTest, InitialState)
{
  AnyValueItem item;

  EXPECT_TRUE(item.GetAnyTypeName().empty());
  EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
  EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
  const int kAnyTypeNameRole = 10;  // defined in anyvalue_item.cpp
  EXPECT_FALSE(item.HasData(kAnyTypeNameRole));
}

TEST_F(AnyValueItemTest, SetAnyTypeName)
{
  AnyValueItem item;

  item.SetAnyTypeName(kInt8TypeName);

  EXPECT_EQ(item.GetAnyTypeName(), kInt8TypeName);
  EXPECT_EQ(item.Data<int>(), 0);
  EXPECT_TRUE(mvvm::utils::IsValid(item.Data()));
  EXPECT_TRUE(item.HasData(mvvm::DataRole::kData));
  const int kAnyTypeNameRole = 10;  // defined in anyvalue_item.cpp
  EXPECT_TRUE(item.HasData(kAnyTypeNameRole));
}
