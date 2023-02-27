/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_variable_items.h>
#include <testutils/mock_model_listener.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/dto/conversion_utils.h>

using namespace sequencergui;
using ::testing::_;

//! Tests for functions from variable_item_transform_utils.h

class VariableItemTransformUtilsTests : public ::testing::Test
{
};

//! Checking UpdateAnyValue function.

TEST_F(VariableItemTransformUtilsTests, SetAnyValue)
{
  // LocalVariableItem doesn't have AnyValuteItem at the beginning
  LocalVariableItem item;
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);

  const sup::dto::AnyValue anyvalue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  // After update it receives AnyValueItem representing anyvalue
  SetAnyValue(anyvalue, item);
  EXPECT_NE(item.GetAnyValueItem(), nullptr);
  auto prev_anyvalue_item = item.GetAnyValueItem();

  auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
  EXPECT_EQ(anyvalue, stored_anyvalue);

  // Updating again. In current implementation underlying AnyValueItem gets simply regenerated.
  // FIXME Provide update if AnyValueItem exists and layout of it corresponds to AnyValue.
  SetAnyValue(anyvalue, item);
  EXPECT_NE(item.GetAnyValueItem(), prev_anyvalue_item);
}

//! Same as above, except that LocalVariableItem is a part of the model.

TEST_F(VariableItemTransformUtilsTests, UpdateAnyValueSignaling)
{
  SequencerModel model;
  auto item = model.InsertItem<LocalVariableItem>();

  testutils::MockModelListener listener(&model);

  const sup::dto::AnyValue anyvalue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  const mvvm::TagIndex tag_index{"kAnyValueTag", 0};

  {
    const ::testing::InSequence seq;
    auto expected_event1 = mvvm::event_variant_t(mvvm::AboutToInsertItemEvent{item, tag_index});
    auto expected_event2 = mvvm::event_variant_t(mvvm::ItemInsertedEvent{item, tag_index});
    EXPECT_CALL(listener, OnEvent(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnEvent(expected_event2)).Times(1);
  }

  SetAnyValue(anyvalue, *item);
}

TEST_F(VariableItemTransformUtilsTests, SetAnyValueFromJsonType)
{
  LocalVariableItem item;
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);

  SetAnyValueFromJsonType(R"RAW({"type":"int32"})RAW", item);

  auto anyvalue_item = item.GetAnyValueItem();
  ASSERT_TRUE(anyvalue_item != nullptr);

  const sup::dto::AnyValue expected_anyvalue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0});

  auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
  EXPECT_EQ(expected_anyvalue, stored_anyvalue);
}
