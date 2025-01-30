/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <oac-tree-gui/core/exceptions.h>
#include <oac-tree-gui/domain/domain_helper.h>
#include <oac-tree-gui/model/item_constants.h>
#include <oac-tree-gui/model/procedure_preamble_items.h>
#include <oac-tree-gui/model/sequencer_model.h>
#include <oac-tree-gui/model/standard_instruction_items.h>
#include <oac-tree-gui/model/standard_variable_items.h>
#include <oac-tree-gui/model/universal_item_helper.h>

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>
#include <sup/gui/model/anyvalue_item_utils.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item_container.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/test/mock_model_listener.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anytype_registry.h>
#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/procedure_preamble.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

#include "oac-tree-gui/transform/anyvalue_item_transform_helper.h"

namespace oac_tree_gui::test
{

//! Tests for functions from anyvalue_item_transform_helper.h

class AnyValueItemTransformHelperTest : public ::testing::Test
{
};

//! Checking SetAnyValue function.

TEST_F(AnyValueItemTransformHelperTest, SetAnyValueFromScalar)
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
  EXPECT_EQ(anyvalue_item->GetDisplayName(), sup::gui::constants::kScalarTypeName);
  EXPECT_EQ(anyvalue_item->GetToolTip(), sup::dto::kInt32TypeName);

  EXPECT_EQ(anyvalue, GetAnyValue(item));

  // Updating again. In current implementation underlying AnyValueItem gets simply regenerated.
  auto prev_anyvalue_item = item.GetAnyValueItem();
  SetAnyValue(anyvalue, item);
  EXPECT_NE(item.GetAnyValueItem(), prev_anyvalue_item);
}

//! Same as above, except that LocalVariableItem is a part of the model.

TEST_F(AnyValueItemTransformHelperTest, UpdateAnyValueSignaling)
{
  SequencerModel model;
  auto item = model.InsertItem<LocalVariableItem>();

  mvvm::test::MockModelListener listener(&model);

  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);

  const mvvm::TagIndex tag_index{itemconstants::kAnyValueTag, 0};

  {
    const ::testing::InSequence seq;
    mvvm::AboutToInsertItemEvent expected_event1{item, tag_index};
    mvvm::ItemInsertedEvent expected_event2{item, tag_index};
    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event2)).Times(1);
  }

  SetAnyValue(anyvalue, *item);
}

//! Checking UpdateAnyValue function.

TEST_F(AnyValueItemTransformHelperTest, UpdateAnyValueFromScalar)
{
  // LocalVariableItem doesn't have AnyValuteItem at the beginning
  LocalVariableItem item;
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);

  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 0);

  // Create and set AnyValueItem representing anyvalue
  SetAnyValue(anyvalue, item);
  EXPECT_EQ(anyvalue, GetAnyValue(item));

  const sup::dto::AnyValue new_anyvalue(sup::dto::SignedInteger32Type, 42);
  UpdateAnyValue(new_anyvalue, item);

  EXPECT_EQ(new_anyvalue, GetAnyValue(item));
}

//! Checking UpdateAnyValue function on changing type of AnyValue.
//! Current implementation allows to change AnyValueItem, this might change in the future.

TEST_F(AnyValueItemTransformHelperTest, UpdateAnyValueFromScalarWithTypeChange)
{
  // LocalVariableItem doesn't have AnyValuteItem at the beginning
  LocalVariableItem item;
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);

  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 0);

  // Create and set AnyValueItem representing anyvalue
  SetAnyValue(anyvalue, item);
  EXPECT_EQ(anyvalue, GetAnyValue(item));

  const sup::dto::AnyValue new_anyvalue(sup::dto::StringType, "abc");

  EXPECT_NO_THROW(UpdateAnyValue(new_anyvalue, item));

  EXPECT_EQ(new_anyvalue, GetAnyValue(item));
}

//! Setting AnyValue to instruction.

TEST_F(AnyValueItemTransformHelperTest, SetInstructionAnyValueFromScalar)
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
  EXPECT_EQ(anyvalue_item->GetDisplayName(), sup::gui::constants::kScalarTypeName);
  EXPECT_EQ(anyvalue_item->GetToolTip(), sup::dto::kInt32TypeName);

  auto stored_anyvalue = CreateAnyValue(*GetAnyValueItem(item));
  EXPECT_EQ(anyvalue, stored_anyvalue);

  // Updating again. In current implementation underlying AnyValueItem gets simply regenerated.
  auto prev_anyvalue_item = GetAnyValueItem(item);
  SetAnyValue(anyvalue, item);
  EXPECT_NE(GetAnyValueItem(item), prev_anyvalue_item);
}

//! Validate SetAnyValueFromDomainVariable helper method.
//! Domain sequencer variable with json type and value attributes is used to set AnyValueItem on
//! board of VariableItem.

TEST_F(AnyValueItemTransformHelperTest, SetAnyValueFromDomainVariable)
{
  {  // when domain variable has type attribute
    auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);
    variable->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"int32"})RAW");

    const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 0);

    LocalVariableItem item;
    SetAnyValueFromDomainVariable(*variable, item);
    EXPECT_EQ(expected_anyvalue, GetAnyValue(item));
  }

  {  // when domain variable has type and value attribute
    auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);
    variable->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"int32"})RAW");
    variable->AddAttribute(domainconstants::kValueAttribute, "42");

    const sup::dto::AnyValue expected_anyvalue(
        sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

    LocalVariableItem item;
    SetAnyValueFromDomainVariable(*variable, item);
    EXPECT_EQ(expected_anyvalue, GetAnyValue(item));
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

TEST_F(AnyValueItemTransformHelperTest, SetAnyValueFromDomainVariableWithRegistry)
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

  EXPECT_EQ(expected_anyvalue, GetAnyValue(item));
}

TEST_F(AnyValueItemTransformHelperTest, SetAnyValueFromDomainInstruction)
{
  // We use Wait instruction for test event if it is not intended for storing json type and value
  // attributes.
  auto instruction = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  instruction->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"int32"})RAW");
  instruction->AddAttribute(domainconstants::kValueAttribute, "42");

  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);

  WaitItem item;
  // registering tag to make possible adding AnyValueItem
  item.RegisterTag(sup::gui::CreateAnyValueTag(itemconstants::kAnyValueTag), true);

  SetAnyValueFromDomainInstruction(*instruction, item);

  ASSERT_NE(GetAnyValueItem(item), nullptr);
  auto stored_anyvalue = CreateAnyValue(*GetAnyValueItem(item));
  EXPECT_EQ(expected_anyvalue, stored_anyvalue);
}

TEST_F(AnyValueItemTransformHelperTest, RegisterChildrenTag)
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

TEST_F(AnyValueItemTransformHelperTest, PopulateProcedurePreambleFromItem)
{
  {  // empty
    sup::oac_tree::ProcedurePreamble preamble;
    ProcedurePreambleItem item;
    PopulateProcedurePreamble(item, preamble);

    EXPECT_TRUE(preamble.GetPluginPaths().empty());
    EXPECT_TRUE(preamble.GetTypeRegistrations().empty());
  }

  {  // preamble
    using sup::oac_tree::TypeRegistrationInfo;

    ProcedurePreambleItem item;
    item.AddPluginPath("abc");
    item.AddPluginPath("def");
    item.AddTypeRegistration(1, "aaa");

    sup::oac_tree::ProcedurePreamble preamble;
    PopulateProcedurePreamble(item, preamble);

    std::vector<std::string> expected_paths{"abc", "def"};
    EXPECT_EQ(preamble.GetPluginPaths(), expected_paths);
    ASSERT_EQ(preamble.GetTypeRegistrations().size(), 1);
    EXPECT_EQ(preamble.GetTypeRegistrations().at(0).GetRegistrationMode(), 1);
    EXPECT_EQ(preamble.GetTypeRegistrations().at(0).GetString(), "aaa");
  }

  {  // attempt to add in non-empty preamble
    using sup::oac_tree::TypeRegistrationInfo;

    ProcedurePreambleItem item;

    sup::oac_tree::ProcedurePreamble preamble;
    preamble.AddPluginPath("abc");

    EXPECT_THROW(PopulateProcedurePreamble(item, preamble), LogicErrorException);
  }
}

//! Validating GetAnyValue helper method.
TEST_F(AnyValueItemTransformHelperTest, GetStoredAnyValue)
{
  // LocalVariableItem doesn't have AnyValuteItem at the beginning
  LocalVariableItem item;

  EXPECT_THROW(GetAnyValue(item), RuntimeException);

  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(anyvalue, item);

  EXPECT_EQ(GetAnyValue(item), anyvalue);
}

//! Validating GetAnyValue method for domain worskpaces.
TEST_F(AnyValueItemTransformHelperTest, GetAnyValueFromDomainWorkspace)
{
  const std::string var_name("var");

  sup::oac_tree::Workspace workspace;
  EXPECT_THROW(GetAnyValue(var_name, workspace), RuntimeException);

  sup::dto::AnyValue initial_value(sup::dto::SignedInteger32Type, 42);
  workspace.AddVariable(var_name, test::CreateLocalVariable(var_name, initial_value));
  workspace.Setup();
  EXPECT_EQ(GetAnyValue(var_name, workspace), initial_value);
}

}  // namespace oac_tree_gui::test
