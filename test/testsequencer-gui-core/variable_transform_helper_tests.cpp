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

#include "sequencergui/transform/variable_transform_helper.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/variable.h>

#include <gtest/gtest.h>
#include <testutils/mock_model_listener.h>

using namespace sequencergui;
using ::testing::_;

//! Tests for functions from variable_item_transform_utils.h

class VariableTransformHelperTests : public ::testing::Test
{
};

//! Checking UpdateAnyValue function.

TEST_F(VariableTransformHelperTests, SetAnyValueFromScalar)
{
  // LocalVariableItem doesn't have AnyValuteItem at the beginning
  LocalVariableItem item;
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);

  const sup::dto::AnyValue anyvalue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  // After update it receives AnyValueItem representing anyvalue
  SetAnyValue(anyvalue, item);

  // checking constructed AnyValueItem and its visual properties
  auto anyvalue_item = item.GetAnyValueItem();
  EXPECT_NE(anyvalue_item, nullptr);
  EXPECT_EQ(anyvalue_item->GetDisplayName(), std::string("value"));
  EXPECT_EQ(anyvalue_item->GetToolTip(), sup::dto::kInt32TypeName);

  auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
  EXPECT_EQ(anyvalue, stored_anyvalue);

  // Updating again. In current implementation underlying AnyValueItem gets simply regenerated.
  auto prev_anyvalue_item = item.GetAnyValueItem();
  SetAnyValue(anyvalue, item);
  EXPECT_NE(item.GetAnyValueItem(), prev_anyvalue_item);
}

//! Same as above, except that LocalVariableItem is a part of the model.

TEST_F(VariableTransformHelperTests, UpdateAnyValueSignaling)
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

TEST_F(VariableTransformHelperTests, SetAnyValueFromJsonType)
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

//! Checking UpdateAnyValue function.

TEST_F(VariableTransformHelperTests, UpdateAnyValueFromScalar)
{
  // LocalVariableItem doesn't have AnyValuteItem at the beginning
  LocalVariableItem item;
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);

  const sup::dto::AnyValue anyvalue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0});

  // After update it receives AnyValueItem representing anyvalue
  SetAnyValue(anyvalue, item);
  auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
  EXPECT_EQ(anyvalue, stored_anyvalue);

  const sup::dto::AnyValue new_anyvalue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  UpdateAnyValue(new_anyvalue, item);

  auto stored_anyvalue2 = CreateAnyValue(*item.GetAnyValueItem());
  EXPECT_EQ(new_anyvalue, stored_anyvalue2);
}

TEST_F(VariableTransformHelperTests, AddNonEmptyAttribute)
{
  auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);

  AddNonEmptyAttribute("custom_attribute_name", "", *variable);
  EXPECT_FALSE(variable->HasAttribute("custom_attribute_name"));

  AddNonEmptyAttribute("custom_attribute_name", "abc", *variable);
  EXPECT_TRUE(variable->HasAttribute("custom_attribute_name"));
}

TEST_F(VariableTransformHelperTests, SetJsonTypeAttribute)
{
  {  // case when property is defined
    LocalVariableItem item;
    item.SetJsonType("json_type");

    auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);
    SetJsonTypeAttribute(item, *variable);

    EXPECT_TRUE(variable->HasAttribute(domainconstants::kTypeAttribute));
    EXPECT_EQ(variable->GetAttribute(domainconstants::kTypeAttribute), "json_type");
  }

  {  // case when AnyValue is defined
    sup::dto::AnyValue anyvalue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

    LocalVariableItem item;
    SetAnyValue(anyvalue, item);

    auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);
    SetJsonTypeAttribute(item, *variable);

    EXPECT_TRUE(variable->HasAttribute(domainconstants::kTypeAttribute));
    EXPECT_EQ(variable->GetAttribute(domainconstants::kTypeAttribute), R"RAW({"type":"int32"})RAW");
  }
}

TEST_F(VariableTransformHelperTests, SetJsonValueAttribute)
{
  {  // case when property is defined
    LocalVariableItem item;
    item.SetJsonValue("json_value");

    auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);
    SetJsonValueAttribute(item, *variable);

    EXPECT_TRUE(variable->HasAttribute(domainconstants::kValueAttribute));
    EXPECT_EQ(variable->GetAttribute(domainconstants::kValueAttribute), "json_value");
  }

  {  // case when AnyValue is defined
    sup::dto::AnyValue anyvalue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

    LocalVariableItem item;
    SetAnyValue(anyvalue, item);

    auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);
    SetJsonValueAttribute(item, *variable);

    EXPECT_TRUE(variable->HasAttribute(domainconstants::kValueAttribute));
    EXPECT_EQ(variable->GetAttribute(domainconstants::kValueAttribute), "42");
  }
}
