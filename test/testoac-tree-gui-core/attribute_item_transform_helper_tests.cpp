/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/transform/attribute_item_transform_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/attribute_item.h>
#include <oac_tree_gui/model/standard_variable_items.h>

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/scalar_conversion_utils.h>

#include <mvvm/model/compound_item.h>
#include <mvvm/model/item_utils.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/attribute_definition.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Tests for functions from variable_item_transform_utils.h

class AttributeItemTransformHelperTest : public ::testing::Test
{
};

TEST_F(AttributeItemTransformHelperTest, IsPlaceholderAttribute)
{
  EXPECT_TRUE(IsPlaceholderAttribute("$par"));
  EXPECT_FALSE(IsPlaceholderAttribute("@par"));
  EXPECT_FALSE(IsPlaceholderAttribute("par"));
  EXPECT_FALSE(IsPlaceholderAttribute(""));
}

TEST_F(AttributeItemTransformHelperTest, IsReferenceAttribute)
{
  EXPECT_FALSE(IsReferenceAttribute("$par"));
  EXPECT_TRUE(IsReferenceAttribute("@par"));
  EXPECT_FALSE(IsReferenceAttribute("par"));
  EXPECT_FALSE(IsReferenceAttribute(""));
}

//! Testing AddPropertyFromDefinition helper method.

TEST_F(AttributeItemTransformHelperTest, AddPropertyFromDefinition)
{
  const std::string attribute_name("attr");
  const sup::oac_tree::AttributeDefinition attr(attribute_name, sup::dto::SignedInteger32Type);

  mvvm::CompoundItem item;
  auto property = AddPropertyFromDefinition(attr, item);
  ASSERT_NE(property, nullptr);

  // validating that CompoundItem got new property item
  EXPECT_TRUE(mvvm::utils::HasTag(item, attribute_name));

  const std::vector<std::string> expected_tags({attribute_name});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);

  auto property_item = item.FindItem(attribute_name);
  ASSERT_EQ(property_item, property);
  EXPECT_EQ(property_item->GetDisplayName(), attribute_name);
  auto attribute_item = dynamic_cast<sup::gui::AnyValueScalarItem*>(property_item);
  ASSERT_NE(attribute_item, nullptr);
  EXPECT_FALSE(GetAttributeExposedFlag(*attribute_item));

  EXPECT_EQ(property_item->Data<int>(), 0);

  // Checking that we can convert AnyValueScalarItem property back to AnyValue
  const sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type};

  auto& anyvalue_item = item.GetItem<sup::gui::AnyValueScalarItem>(attr.GetName());
  auto any_value = sup::gui::GetAnyValueFromScalar(anyvalue_item.Data());

  EXPECT_EQ(expected_anyvalue, any_value);
}

//! Testing AddPropertyFromDefinition helper method. The difference with previous test is mandatory
//! flag.

TEST_F(AttributeItemTransformHelperTest, AddMandatoryPropertyFromDefinition)
{
  const std::string attribute_name("attr");
  sup::oac_tree::AttributeDefinition attr(attribute_name, sup::dto::SignedInteger32Type);
  attr.SetMandatory(true);

  mvvm::CompoundItem item;
  auto property =
      dynamic_cast<sup::gui::AnyValueScalarItem*>(AddPropertyFromDefinition(attr, item));
  ASSERT_NE(property, nullptr);

  EXPECT_EQ(property->GetDisplayName(), attribute_name);
  EXPECT_TRUE(GetAttributeExposedFlag(*property));
}

//! Testing SetPropertyFromDomainAttribute method.

TEST_F(AttributeItemTransformHelperTest, SetPropertyFromDomainAttribute)
{
  const workspace_t ws;
  {  // case when variable was setup
    auto domain_variable = CreateDomainVariable(domainconstants::kLocalVariableType);
    domain_variable->AddAttribute(domainconstants::kNameAttribute, "abc");
    domain_variable->Setup(ws);

    sup::gui::AnyValueScalarItem item;
    item.SetAnyTypeName(sup::dto::kStringTypeName);

    SetPropertyFromDomainAttribute(*domain_variable, domainconstants::kNameAttribute, item);
    EXPECT_TRUE(GetAttributeExposedFlag(item));
    EXPECT_EQ(item.Data<std::string>(), std::string("abc"));
  }

  {  // case when variable wasn't setup
    auto domain_variable = CreateDomainVariable(domainconstants::kLocalVariableType);
    domain_variable->AddAttribute(domainconstants::kNameAttribute, "abc");

    sup::gui::AnyValueScalarItem item;
    item.SetAnyTypeName(sup::dto::kStringTypeName);

    EXPECT_NO_THROW(
        SetPropertyFromDomainAttribute(*domain_variable, domainconstants::kNameAttribute, item));

    EXPECT_TRUE(GetAttributeExposedFlag(item));
    EXPECT_EQ(item.Data<std::string>(), std::string("abc"));
  }
}

//! SetPropertyFromAttributeString stores a value with `$` prefix as a string.
TEST_F(AttributeItemTransformHelperTest, SetPropertyFromAttributeStringPlaceholderCase)
{
  sup::gui::AnyValueScalarItem item;
  item.SetAnyTypeName(sup::dto::kInt32TypeName);
  EXPECT_TRUE(std::holds_alternative<mvvm::int32>(item.Data()));

  SetPropertyFromAttributeString("$par1", item);

  // current convention in AttributeItem::SetAttributeAsString is to keep original
  // AnyTypeName after setting attribute as a string

  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_TRUE(std::holds_alternative<std::string>(item.Data()));
  EXPECT_EQ(item.Data<std::string>(), "$par1");
  EXPECT_TRUE(GetAttributeExposedFlag(item));
}

//! SetPropertyFromAttributeString stores a value with `@` prefix as a string.
TEST_F(AttributeItemTransformHelperTest, SetPropertyFromAttributeStringReferenceCase)
{
  sup::gui::AnyValueScalarItem item;
  item.SetAnyTypeName(sup::dto::kInt32TypeName);
  EXPECT_TRUE(std::holds_alternative<mvvm::int32>(item.Data()));

  SetPropertyFromAttributeString("@par1", item);

  // current convention in AttributeItem::SetAttributeAsString is to keep original
  // AnyTypeName after setting attribute as a string

  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_TRUE(std::holds_alternative<std::string>(item.Data()));
  EXPECT_EQ(item.Data<std::string>(), "@par1");
  EXPECT_TRUE(GetAttributeExposedFlag(item));
}

//! Testing SetDomainAttribute method.

TEST_F(AttributeItemTransformHelperTest, SetDomainAttribute)
{
  auto domain_variable = CreateDomainVariable(domainconstants::kLocalVariableType);

  sup::gui::AnyValueScalarItem item;
  item.SetAnyTypeName(sup::dto::kStringTypeName);
  item.SetData("abc");

  SetDomainAttribute(item, domainconstants::kNameAttribute, *domain_variable);
  EXPECT_EQ(domain_variable->GetAttributeString(domainconstants::kNameAttribute),
            std::string("abc"));
}

//! Testing SetDomainAttribute method when attribute is marked as unset

TEST_F(AttributeItemTransformHelperTest, SetDomainAttributeWhenUnset)
{
  auto domain_variable = CreateDomainVariable(domainconstants::kLocalVariableType);

  sup::gui::AnyValueScalarItem item;
  item.SetAnyTypeName(sup::dto::kStringTypeName);
  item.SetData("abc");
  SetAttributeExposedFlag(false, item);

  SetDomainAttribute(item, domainconstants::kNameAttribute, *domain_variable);
  EXPECT_FALSE(domain_variable->HasAttribute(domainconstants::kNameAttribute));
}

TEST_F(AttributeItemTransformHelperTest, SetAttributeAsString)
{
  sup::gui::AnyValueScalarItem item;

  item.SetAnyTypeName(sup::dto::kInt8TypeName);
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);

  SetAttributeAsString("abc", item);
  EXPECT_EQ(item.Data<std::string>(), std::string("abc"));
  // current convention is to keep original AnyTypeName after setting attribute as a string
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);

  SetAttributeAsString("def", item);
  // current convention is to keep original AnyTypeName after setting attribute as a string
  EXPECT_EQ(item.Data<std::string>(), std::string("def"));
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);
}

TEST_F(AttributeItemTransformHelperTest, SetExposedFlag)
{
  sup::gui::AnyValueScalarItem item;

  item.SetAnyTypeName(sup::dto::kInt8TypeName);
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);

  EXPECT_TRUE(GetAttributeExposedFlag(item));

  SetAttributeExposedFlag(false, item);
  EXPECT_FALSE(GetAttributeExposedFlag(item));
  EXPECT_FALSE(item.IsEditable());
  EXPECT_FALSE(item.IsEnabled());
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);
}

TEST_F(AttributeItemTransformHelperTest, SetAttributeFromTypeName)
{
  sup::gui::AnyValueScalarItem item;

  item.SetAnyTypeName(sup::dto::kInt8TypeName);
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);

  SetAttributeExposedFlag(false, item);
  EXPECT_FALSE(GetAttributeExposedFlag(item));
  EXPECT_FALSE(item.IsEditable());
  EXPECT_FALSE(item.IsEnabled());
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);

  SetAttributeFromTypeName(item);
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);

  // SetAttributeFromTypeName do not change present flags
  EXPECT_FALSE(item.IsEditable());
  EXPECT_FALSE(item.IsEnabled());
}

TEST_F(AttributeItemTransformHelperTest, HasAttributeDefinition)
{
  auto local_variable = CreateDomainVariable(domainconstants::kLocalVariableType);
  EXPECT_TRUE(HasAttributeDefinition(*local_variable, domainconstants::kTypeAttribute));
  EXPECT_FALSE(HasAttributeDefinition(*local_variable, std::string("non-existing-attribute")));
}

TEST_F(AttributeItemTransformHelperTest, SetLocalVariableJsonAttributesFromItem)
{
  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  auto anyvalue_item = sup::gui::CreateAnyValueItem(anyvalue);

  auto variable = CreateDomainVariable(domainconstants::kLocalVariableType);
  SetJsonAttributesFromItem(*anyvalue_item, *variable);

  EXPECT_TRUE(variable->HasAttribute(domainconstants::kTypeAttribute));
  EXPECT_EQ(variable->GetAttributeString(domainconstants::kTypeAttribute),
            R"RAW({"type":"int32"})RAW");

  EXPECT_TRUE(variable->HasAttribute(domainconstants::kValueAttribute));
  EXPECT_EQ(variable->GetAttributeString(domainconstants::kValueAttribute), "42");
}

TEST_F(AttributeItemTransformHelperTest, SetChannelAccessVariableJsonAttributesFromItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  auto anyvalue_item = sup::gui::CreateAnyValueItem(anyvalue);

  auto variable = CreateDomainVariable(domainconstants::kChannelAccessVariableType);
  SetJsonAttributesFromItem(*anyvalue_item, *variable);

  EXPECT_TRUE(variable->HasAttribute(domainconstants::kTypeAttribute));
  EXPECT_EQ(variable->GetAttributeString(domainconstants::kTypeAttribute),
            R"RAW({"type":"int32"})RAW");

  // channel access variable doesn't have value attribute
  EXPECT_FALSE(variable->HasAttribute(domainconstants::kValueAttribute));
}

TEST_F(AttributeItemTransformHelperTest, SetInstructionJsonAttributesFromItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const sup::dto::AnyValue anyvalue(sup::dto::SignedInteger32Type, 42);
  auto anyvalue_item = sup::gui::CreateAnyValueItem(anyvalue);

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

TEST_F(AttributeItemTransformHelperTest, SetInstructionJsonAttributesFromEmptyItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const sup::dto::AnyValue anyvalue;
  auto anyvalue_item = sup::gui::CreateAnyValueItem(anyvalue);

  // PvAccessWriteInstruction should get attributes
  auto instr = CreateDomainInstruction(domainconstants::kPvAccessWriteInstructionType);
  SetJsonAttributesFromItem(*anyvalue_item, *instr);

  EXPECT_FALSE(instr->HasAttribute(domainconstants::kTypeAttribute));
  EXPECT_FALSE(instr->HasAttribute(domainconstants::kValueAttribute));
}

//! Attribute item from the domain attribute string.
TEST_F(AttributeItemTransformHelperTest, SetAttributeFromString)
{
  AttributeItem item;
  item.SetAttributeType(mvvm::uint32{0}, /*is_nullable*/ true, /*allow_text*/ true);

  SetAttributeFromString("42", item);
  EXPECT_EQ(item.Data(), mvvm::variant_t{mvvm::uint32{42}});

  SetAttributeFromString("$par", item);
  EXPECT_EQ(item.Data(), mvvm::variant_t{std::string("$par")});

  SetAttributeFromString("@var", item);
  EXPECT_EQ(item.Data(), mvvm::variant_t{std::string("@var")});

  // parsing fails, the item is left unchanged
  SetAttributeFromString("abc", item);
  EXPECT_EQ(item.Data(), mvvm::variant_t{std::string("@var")});

  AttributeItem string_item;
  string_item.SetAttributeType(std::string(), /*is_nullable*/ false, /*allow_text*/ false);
  SetAttributeFromString("abc", string_item);
  EXPECT_EQ(string_item.Data(), mvvm::variant_t{std::string("abc")});

  // a string attribute holds the placeholder as its value
  SetAttributeFromString("$par", string_item);
  EXPECT_EQ(string_item.Data(), mvvm::variant_t{std::string("$par")});
  EXPECT_FALSE(string_item.HasText());
}

//! Attribute item without text: a placeholder fails to parse, the item is left unchanged.
TEST_F(AttributeItemTransformHelperTest, SetAttributeFromStringWhenTextNotAllowed)
{
  AttributeItem item;
  item.SetAttributeType(false, /*is_nullable*/ true, /*allow_text*/ false);

  SetAttributeFromString("$par", item);
  EXPECT_FALSE(item.HasData());

  SetAttributeFromString("true", item);
  EXPECT_EQ(item.Data(), mvvm::variant_t{true});

  SetAttributeFromString("@var", item);
  EXPECT_EQ(item.Data(), mvvm::variant_t{true});
}

//! Domain attribute string from the attribute item.
TEST_F(AttributeItemTransformHelperTest, GetAttributeString)
{
  AttributeItem item;
  item.SetAttributeType(1.0, /*is_nullable*/ true, /*allow_text*/ true);

  // not set
  EXPECT_TRUE(GetAttributeString(item).empty());

  EXPECT_TRUE(item.SetData(42.5));
  EXPECT_EQ(GetAttributeString(item), "42.5");

  EXPECT_TRUE(item.SetData(std::string("$par")));
  EXPECT_EQ(GetAttributeString(item), "$par");
}

//! Domain attribute from the attribute item: not set, empty string and isRoot=false are skipped.
TEST_F(AttributeItemTransformHelperTest, SetDomainAttributeFromAttributeItem)
{
  auto instruction = CreateDomainInstruction(domainconstants::kWaitInstructionType);

  AttributeItem timeout;
  timeout.SetAttributeType(0.0, /*is_nullable*/ true, /*allow_text*/ true);
  SetDomainAttribute(timeout, domainconstants::kTimeoutAttribute, *instruction);
  EXPECT_FALSE(instruction->HasAttribute(domainconstants::kTimeoutAttribute));

  EXPECT_TRUE(timeout.SetData(2.0));
  SetDomainAttribute(timeout, domainconstants::kTimeoutAttribute, *instruction);
  EXPECT_EQ(instruction->GetAttributeString(domainconstants::kTimeoutAttribute), "2.0");

  AttributeItem is_root;
  is_root.SetAttributeType(false, /*is_nullable*/ false, /*allow_text*/ false);
  SetDomainAttribute(is_root, domainconstants::kIsRootAttribute, *instruction);
  EXPECT_FALSE(instruction->HasAttribute(domainconstants::kIsRootAttribute));

  AttributeItem name;
  name.SetAttributeType(std::string(), /*is_nullable*/ false, /*allow_text*/ false);
  SetDomainAttribute(name, "custom", *instruction);
  EXPECT_FALSE(instruction->HasAttribute("custom"));
}

}  // namespace oac_tree_gui::test
