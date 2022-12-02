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

#include "sequencergui/transform/variable_item_transform_utils.h"

#include <gtest/gtest.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sup/dto/anyvalue.h>
#include <sup/gui/dto/conversion_utils.h>

using namespace sequencergui;

//! Tests for functions from variable_item_transform_utils.h

class VariableItemTransformUtilsTests : public ::testing::Test
{
};

TEST_F(VariableItemTransformUtilsTests, UpdateAnyValue)
{
  sup::dto::AnyValue anyvalue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  LocalVariableItem item;
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);

  UpdateAnyValue(anyvalue, item);
  EXPECT_NE(item.GetAnyValueItem(), nullptr);

  auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
  EXPECT_EQ(anyvalue, stored_anyvalue);
}
