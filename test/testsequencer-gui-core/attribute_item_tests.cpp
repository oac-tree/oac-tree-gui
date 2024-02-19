/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/attribute_item.h"

#include <sequencergui/model/item_constants.h>
#include <sequencergui/transform/attribute_item_transform_helper.h>

#include <sup/dto/anytype.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

using namespace sequencergui;

namespace
{
const int kAnyTypeNameRole = 10;  // defined in attribute_item.cpp
}

class AttributeItemTest : public ::testing::Test
{
};

TEST_F(AttributeItemTest, InitialState)
{
  const AttributeItem item;
  EXPECT_EQ(item.GetDisplayName(), AttributeItem::Type);
  EXPECT_EQ(item.GetAnyTypeName(), std::string());
  EXPECT_TRUE(item.HasFlag(mvvm::Appearance::kProperty));
  EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
}

TEST_F(AttributeItemTest, SetAnyTypeName)
{
  AttributeItem item;

  item.SetAnyTypeName(sup::dto::kInt8TypeName);

  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_TRUE(mvvm::utils::IsValid(item.Data()));
  EXPECT_TRUE(item.HasData(mvvm::DataRole::kData));
  EXPECT_TRUE(item.HasData(kAnyTypeNameRole));
}

TEST_F(AttributeItemTest, SetAttributeAsString)
{
  AttributeItem item;

  item.SetAnyTypeName(sup::dto::kInt8TypeName);
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);

  item.SetAttributeAsString("abc");
  EXPECT_EQ(item.Data<std::string>(), std::string("abc"));
  // current convention is to keep original AnyTypeName after setting attribute as a string
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);

  item.SetAttributeAsString("def");
  // current convention is to keep original AnyTypeName after setting attribute as a string
  EXPECT_EQ(item.Data<std::string>(), std::string("def"));
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);
}

TEST_F(AttributeItemTest, SetPresentFlag)
{
  AttributeItem item;

  item.SetAnyTypeName(sup::dto::kInt8TypeName);
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);
  
  EXPECT_TRUE(IsAttributePresent(item));
  
  SetAttributePresentFlag(false, item);
  EXPECT_FALSE(IsAttributePresent(item));
  EXPECT_FALSE(item.IsEditable());
  EXPECT_FALSE(item.IsEnabled());
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);
}

TEST_F(AttributeItemTest, SetAttributeFromTypeName)
{
  AttributeItem item;

  item.SetAnyTypeName(sup::dto::kInt8TypeName);
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);
  
  SetAttributePresentFlag(false, item);
  EXPECT_FALSE(IsAttributePresent(item));
  EXPECT_FALSE(item.IsEditable());
  EXPECT_FALSE(item.IsEnabled());
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);

  item.SetAttributeFromTypeName();
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);

  // SetAttributeFromTypeName do not change present flags
  EXPECT_FALSE(item.IsEditable());
  EXPECT_FALSE(item.IsEnabled());
}
