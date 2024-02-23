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

#include "sequencergui/transform/transform_helpers.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/procedure_preamble_items.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/universal_item_helper.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/model/sessionitem_container.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/test/mock_model_listener.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anytype_registry.h>
#include <sup/dto/anyvalue.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure_preamble.h>
#include <sup/sequencer/variable.h>

#include <gtest/gtest.h>

using namespace sequencergui;
using ::testing::_;

//! Tests for functions from variable_item_transform_utils.h

class TransformHelpersTest : public ::testing::Test
{
};

//! Checking SetAnyValue function.

TEST_F(TransformHelpersTest, SetAnyValueFromScalar)
{
  // LocalVariableItem doesn't have AnyValuteItem at the beginning
  LocalVariableItem item;
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);

  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);

  // After update it receives AnyValueItem representing anyvalue
  SetAnyValue(anyvalue, item);

  // checking constructed AnyValueItem and its visual properties
  auto anyvalue_item = item.GetAnyValueItem();
  EXPECT_NE(anyvalue_item, nullptr);
  EXPECT_EQ(anyvalue_item->GetDisplayName(), itemconstants::kAnyValueDefaultDisplayName);
  EXPECT_EQ(anyvalue_item->GetToolTip(), sup::dto::kInt32TypeName);

  auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
  EXPECT_EQ(anyvalue, stored_anyvalue);

  // Updating again. In current implementation underlying AnyValueItem gets simply regenerated.
  auto prev_anyvalue_item = item.GetAnyValueItem();
  SetAnyValue(anyvalue, item);
  EXPECT_NE(item.GetAnyValueItem(), prev_anyvalue_item);
}

//! Same as above, except that LocalVariableItem is a part of the model.

TEST_F(TransformHelpersTest, UpdateAnyValueSignaling)
{
  SequencerModel model;
  auto item = model.InsertItem<LocalVariableItem>();

  mvvm::test::MockModelListener listener(&model);

  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);

  const mvvm::TagIndex tag_index{itemconstants::kAnyValueTag, 0};

  {
    const ::testing::InSequence seq;
    auto expected_event1 = mvvm::event_variant_t(mvvm::AboutToInsertItemEvent{item, tag_index});
    auto expected_event2 = mvvm::event_variant_t(mvvm::ItemInsertedEvent{item, tag_index});
    EXPECT_CALL(listener, OnEvent(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnEvent(expected_event2)).Times(1);
  }

  SetAnyValue(anyvalue, *item);
}

//! Checking UpdateAnyValue function.

TEST_F(TransformHelpersTest, UpdateAnyValueFromScalar)
{
  // LocalVariableItem doesn't have AnyValuteItem at the beginning
  LocalVariableItem item;
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);

  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 0);

  // Create and set AnyValueItem representing anyvalue
  SetAnyValue(anyvalue, item);
  auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
  EXPECT_EQ(anyvalue, stored_anyvalue);

  const sup::dto::AnyValue new_anyvalue(sup::dto::SignedInteger32Type, 42);
  UpdateAnyValue(new_anyvalue, item);

  auto stored_anyvalue2 = CreateAnyValue(*item.GetAnyValueItem());
  EXPECT_EQ(new_anyvalue, stored_anyvalue2);
}

//! Checking UpdateAnyValue function on changing type of AnyValue.
//! Current implementation allows to change AnyValueItem, this might change in the future.

TEST_F(TransformHelpersTest, UpdateAnyValueFromScalarWithTypeChange)
{
  // LocalVariableItem doesn't have AnyValuteItem at the beginning
  LocalVariableItem item;
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);

  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 0);

  // Create and set AnyValueItem representing anyvalue
  SetAnyValue(anyvalue, item);
  auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
  EXPECT_EQ(anyvalue, stored_anyvalue);

  const sup::dto::AnyValue new_anyvalue(sup::dto::StringType, "abc");

  EXPECT_NO_THROW(UpdateAnyValue(new_anyvalue, item));

  auto stored_anyvalue2 = CreateAnyValue(*item.GetAnyValueItem());
  EXPECT_EQ(new_anyvalue, stored_anyvalue2);
}

//! Setting AnyValue to instruction.

TEST_F(TransformHelpersTest, SetInstructionAnyValueFromScalar)
{
  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  WaitItem item;

  // WaitItem is not intended for storing AnyValueItem
  EXPECT_THROW(SetAnyValue(anyvalue, item), LogicErrorException);

  // Let's create a corresponding tag
  item.RegisterTag(sup::gui::CreateAnyValueTag(itemconstants::kAnyValueTag), true);

  EXPECT_EQ(GetAnyValueItem(item), nullptr);
  SetAnyValue(anyvalue, item);

  // checking constructed AnyValueItem and its visual properties
  auto anyvalue_item = GetAnyValueItem(item);
  ASSERT_NE(anyvalue_item, nullptr);
  EXPECT_EQ(anyvalue_item->GetDisplayName(), itemconstants::kAnyValueDefaultDisplayName);
  EXPECT_EQ(anyvalue_item->GetToolTip(), sup::dto::kInt32TypeName);

  auto stored_anyvalue = CreateAnyValue(*GetAnyValueItem(item));
  EXPECT_EQ(anyvalue, stored_anyvalue);

  // Updating again. In current implementation underlying AnyValueItem gets simply regenerated.
  auto prev_anyvalue_item = GetAnyValueItem(item);
  SetAnyValue(anyvalue, item);
  EXPECT_NE(GetAnyValueItem(item), prev_anyvalue_item);
}

TEST_F(TransformHelpersTest, AddNonEmptyAttributeToVariable)
{
  auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);

  AddNonEmptyAttribute("custom_attribute_name", "", *variable);
  EXPECT_FALSE(variable->HasAttribute("custom_attribute_name"));

  AddNonEmptyAttribute("custom_attribute_name", "abc", *variable);
  EXPECT_TRUE(variable->HasAttribute("custom_attribute_name"));
}

TEST_F(TransformHelpersTest, AddNonEmptyAttributeToInstruction)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  auto instruction = CreateDomainInstruction(domainconstants::kChannelAccessReadInstructionType);

  AddNonEmptyAttribute("custom_attribute_name", "", *instruction);
  EXPECT_FALSE(instruction->HasAttribute("custom_attribute_name"));

  AddNonEmptyAttribute("custom_attribute_name", "abc", *instruction);
  EXPECT_TRUE(instruction->HasAttribute("custom_attribute_name"));
}

//! Validate SetAnyValueFromDomainVariable helper method.
//! Domain sequencer variable with json type and value attributes is used to set AnyValueItem on
//! board of VariableItem.

TEST_F(TransformHelpersTest, SetAnyValueFromDomainVariable)
{
  {  // when domain variable has type attribute
    auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);
    variable->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"int32"})RAW");

    const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 0);

    LocalVariableItem item;
    SetAnyValueFromDomainVariable(*variable, item);

    ASSERT_NE(item.GetAnyValueItem(), nullptr);
    auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
    EXPECT_EQ(expected_anyvalue, stored_anyvalue);
  }

  {  // when domain variable has type and value attribute
    auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);
    variable->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"int32"})RAW");
    variable->AddAttribute(domainconstants::kValueAttribute, "42");

    const sup::dto::AnyValue expected_anyvalue(
        sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

    LocalVariableItem item;
    SetAnyValueFromDomainVariable(*variable, item);

    ASSERT_NE(item.GetAnyValueItem(), nullptr);
    auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
    EXPECT_EQ(expected_anyvalue, stored_anyvalue);
  }

  {  // when domain variable has only value attribute
    auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);
    variable->AddAttribute(domainconstants::kValueAttribute, "42");

    LocalVariableItem item;
    SetAnyValueFromDomainVariable(*variable, item);

    EXPECT_EQ(item.GetAnyValueItem(), nullptr);
  }
}

//! Validate SetAnyValueFromDomainVariable helper method. Json type is preregistered.

TEST_F(TransformHelpersTest, SetAnyValueFromDomainVariableWithRegistry)
{
  sup::dto::AnyTypeRegistry registry;
  std::string one_scalar_name = "OneScalar";
  sup::dto::AnyType one_scalar{{{"value", sup::dto::SignedInteger32Type}}, one_scalar_name};
  registry.RegisterType(one_scalar);

  auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);
  variable->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"OneScalar"})RAW");
  variable->AddAttribute(domainconstants::kValueAttribute, R"RAW({"value":42})RAW");

  LocalVariableItem item;
  SetAnyValueFromDomainVariable(*variable, item, &registry);

  sup::dto::AnyValue expected_anyvalue = {{{"value", {sup::dto::SignedInteger32Type, 42}}},
                                          one_scalar_name};

  ASSERT_NE(item.GetAnyValueItem(), nullptr);
  auto stored_anyvalue = CreateAnyValue(*item.GetAnyValueItem());
  EXPECT_EQ(expected_anyvalue, stored_anyvalue);
}

TEST_F(TransformHelpersTest, SetAnyValueFromDomainInstruction)
{
  // We use Wait instruction for test event if it is not intended for storing json type and value
  // attributes.
  auto instruction = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  instruction->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"int32"})RAW");
  instruction->AddAttribute(domainconstants::kValueAttribute, "42");

  const sup::dto::AnyValue expected_anyvalue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  WaitItem item;
  // registering tag to make possible adding AnyValueItem
  item.RegisterTag(sup::gui::CreateAnyValueTag(itemconstants::kAnyValueTag), true);

  SetAnyValueFromDomainInstruction(*instruction, item);

  ASSERT_NE(GetAnyValueItem(item), nullptr);
  auto stored_anyvalue = CreateAnyValue(*GetAnyValueItem(item));
  EXPECT_EQ(expected_anyvalue, stored_anyvalue);
}

TEST_F(TransformHelpersTest, RegisterChildrenTag)
{
  {  // case when variable has no children
    auto instruction = CreateDomainInstruction(domainconstants::kWaitInstructionType);
    mvvm::CompoundItem item;
    RegisterChildrenTag(*instruction, item);
    EXPECT_FALSE(mvvm::utils::HasTag(item, itemconstants::kChildInstructions));
  }

  {  // case when variable has no children
    auto instruction = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
    mvvm::CompoundItem item;
    RegisterChildrenTag(*instruction, item);
    EXPECT_TRUE(mvvm::utils::HasTag(item, itemconstants::kChildInstructions));

    auto taginfo = item.GetTaggedItems()->ContainerAt(0).GetTagInfo();
    EXPECT_EQ(taginfo.GetMax(), -1);
    EXPECT_EQ(taginfo.GetMin(), 0);
  }

  {  // case when variable has no children
    auto instruction = CreateDomainInstruction(domainconstants::kInverterInstructionType);
    mvvm::CompoundItem item;
    RegisterChildrenTag(*instruction, item);
    EXPECT_TRUE(mvvm::utils::HasTag(item, itemconstants::kChildInstructions));

    auto taginfo = item.GetTaggedItems()->ContainerAt(0).GetTagInfo();
    EXPECT_EQ(taginfo.GetMax(), 1);
    EXPECT_EQ(taginfo.GetMin(), 0);
  }
}

TEST_F(TransformHelpersTest, PopulateProcedurePreambleFromItem)
{
  {  // empty
    sup::sequencer::ProcedurePreamble preamble;
    ProcedurePreambleItem item;
    PopulateProcedurePreamble(item, preamble);

    EXPECT_TRUE(preamble.GetPluginPaths().empty());
    EXPECT_TRUE(preamble.GetTypeRegistrations().empty());
  }

  {  // preamble
    using sup::sequencer::TypeRegistrationInfo;

    ProcedurePreambleItem item;
    item.AddPluginPath("abc");
    item.AddPluginPath("def");
    item.AddTypeRegistration(1, "aaa");

    sup::sequencer::ProcedurePreamble preamble;
    PopulateProcedurePreamble(item, preamble);

    std::vector<std::string> expected_paths{"abc", "def"};
    EXPECT_EQ(preamble.GetPluginPaths(), expected_paths);
    ASSERT_EQ(preamble.GetTypeRegistrations().size(), 1);
    EXPECT_EQ(preamble.GetTypeRegistrations().at(0).GetRegistrationMode(), 1);
    EXPECT_EQ(preamble.GetTypeRegistrations().at(0).GetString(), "aaa");
  }

  {  // attempt to add in non-empty preamble
    using sup::sequencer::TypeRegistrationInfo;

    ProcedurePreambleItem item;

    sup::sequencer::ProcedurePreamble preamble;
    preamble.AddPluginPath("abc");

    EXPECT_THROW(PopulateProcedurePreamble(item, preamble), LogicErrorException);
  }
}

TEST_F(TransformHelpersTest, HasAttributeDefinition)
{
  auto local_variable = CreateDomainVariable(domainconstants::kLocalVariableType);
  EXPECT_TRUE(HasAttributeDefinition(*local_variable, domainconstants::kTypeAttribute));
  EXPECT_FALSE(HasAttributeDefinition(*local_variable, std::string("non-existing-attribute")));
}

TEST_F(TransformHelpersTest, HasJsonTypeAndNameAttributes)
{
  auto local_variable = CreateDomainVariable(domainconstants::kLocalVariableType);
  EXPECT_TRUE(HasJsonTypeAndNameAttributes(*local_variable));

  auto file_variable = CreateDomainVariable(domainconstants::kFileVariableType);
  EXPECT_FALSE(HasJsonTypeAndNameAttributes(*file_variable));

  auto instr = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  EXPECT_FALSE(HasJsonTypeAndNameAttributes(*instr));
}

TEST_F(TransformHelpersTest, HasJsonTypeAndNameAttributesForEpicsInstruction)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  auto instr = CreateDomainInstruction(domainconstants::kPvAccessWriteInstructionType);
  EXPECT_TRUE(HasJsonTypeAndNameAttributes(*instr));

  instr = CreateDomainInstruction(domainconstants::kPvAccessReadInstructionType);
  EXPECT_FALSE(HasJsonTypeAndNameAttributes(*instr));

  instr = CreateDomainInstruction(domainconstants::kChannelAccessWriteInstructionType);
  EXPECT_TRUE(HasJsonTypeAndNameAttributes(*instr));

  instr = CreateDomainInstruction(domainconstants::kChannelAccessReadInstructionType);
  EXPECT_FALSE(HasJsonTypeAndNameAttributes(*instr));

  instr = CreateDomainInstruction(domainconstants::kRPCClientInstructionType);
  EXPECT_TRUE(HasJsonTypeAndNameAttributes(*instr));
}

TEST_F(TransformHelpersTest, SetLocalVariableJsonAttributesFromItem)
{
  sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  auto anyvalue_item = sup::gui::CreateItem(anyvalue);

  auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);
  SetJsonAttributesFromItem(*anyvalue_item, *variable);

  EXPECT_TRUE(variable->HasAttribute(domainconstants::kTypeAttribute));
  EXPECT_EQ(variable->GetAttributeString(domainconstants::kTypeAttribute),
            R"RAW({"type":"int32"})RAW");

  EXPECT_TRUE(variable->HasAttribute(domainconstants::kValueAttribute));
  EXPECT_EQ(variable->GetAttributeString(domainconstants::kValueAttribute), "42");
}

TEST_F(TransformHelpersTest, SetChannelAccessVariableJsonAttributesFromItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  auto anyvalue_item = sup::gui::CreateItem(anyvalue);

  auto variable = CreateDomainVariable(domainconstants::kChannelAccessVariableType);
  SetJsonAttributesFromItem(*anyvalue_item, *variable);

  EXPECT_TRUE(variable->HasAttribute(domainconstants::kTypeAttribute));
  EXPECT_EQ(variable->GetAttributeString(domainconstants::kTypeAttribute),
            R"RAW({"type":"int32"})RAW");

  // channel access variable doesn't have value attribute
  EXPECT_FALSE(variable->HasAttribute(domainconstants::kValueAttribute));
}

TEST_F(TransformHelpersTest, SetInstructionJsonAttributesFromItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  auto anyvalue_item = sup::gui::CreateItem(anyvalue);

  // PvAccessWriteInstruction should get attributes
  auto instr = CreateDomainInstruction(domainconstants::kPvAccessWriteInstructionType);
  SetJsonAttributesFromItem(*anyvalue_item, *instr);

  EXPECT_TRUE(instr->HasAttribute(domainconstants::kTypeAttribute));
  EXPECT_EQ(instr->GetAttributeString(domainconstants::kTypeAttribute),
            R"RAW({"type":"int32"})RAW");
  EXPECT_TRUE(instr->HasAttribute(domainconstants::kValueAttribute));
  EXPECT_EQ(instr->GetAttributeString(domainconstants::kValueAttribute), "42");

  instr = CreateDomainInstruction(domainconstants::kPvAccessReadInstructionType);
  SetJsonAttributesFromItem(*anyvalue_item, *instr);
  EXPECT_FALSE(instr->HasAttribute(domainconstants::kTypeAttribute));
  EXPECT_FALSE(instr->HasAttribute(domainconstants::kValueAttribute));
}

TEST_F(TransformHelpersTest, SetInstructionJsonAttributesFromEmptyItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  sup::dto::AnyValue anyvalue;
  auto anyvalue_item = sup::gui::CreateItem(anyvalue);

  // PvAccessWriteInstruction should get attributes
  auto instr = CreateDomainInstruction(domainconstants::kPvAccessWriteInstructionType);
  SetJsonAttributesFromItem(*anyvalue_item, *instr);

  EXPECT_FALSE(instr->HasAttribute(domainconstants::kTypeAttribute));
  EXPECT_FALSE(instr->HasAttribute(domainconstants::kValueAttribute));
}

