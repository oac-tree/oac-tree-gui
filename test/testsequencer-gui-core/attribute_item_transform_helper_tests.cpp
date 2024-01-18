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

#include "sequencergui/transform/attribute_item_transform_helper.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/attribute_item.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sup/gui/model/scalar_conversion_utils.h>

#include <mvvm/model/compound_item.h>
#include <mvvm/model/item_utils.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/sequencer/attribute_definition.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>

using namespace sequencergui;

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
  const sup::sequencer::AttributeDefinition attr(attribute_name, sup::dto::SignedInteger32Type);

  mvvm::CompoundItem item;
  auto property = AddPropertyFromDefinition(attr, item);
  ASSERT_NE(property, nullptr);

  // validating that CompoundItem got new property item
  EXPECT_TRUE(mvvm::utils::HasTag(item, attribute_name));

  std::vector<std::string> expected_tags({attribute_name});
  EXPECT_EQ(mvvm::utils::RegisteredTags(item), expected_tags);

  auto property_item = item.GetItem({attribute_name});
  ASSERT_EQ(property_item, property);
  EXPECT_EQ(property_item->GetDisplayName(), attribute_name);
  EXPECT_NE(dynamic_cast<AttributeItem*>(property_item), nullptr);

  EXPECT_EQ(property_item->Data<int>(), 0);

  // Checking that we can convert AnyValueScalarItem property back to AnyValue
  sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type};

  auto anyvalue_item = item.GetItem<AttributeItem>(attr.GetName());
  auto any_value = sup::gui::GetAnyValueFromScalar(anyvalue_item->Data());

  EXPECT_EQ(expected_anyvalue, any_value);
}

//! Testing SetPropertyFromDomainAttribute method.

TEST_F(AttributeItemTransformHelperTest, SetPropertyFromDomainAttribute)
{
  workspace_t ws;
  {  // case when variable was setup
    auto domain_variable = CreateDomainVariable(domainconstants::kLocalVariableType);
    domain_variable->AddAttribute(domainconstants::kNameAttribute, "abc");
    domain_variable->Setup(ws);

    AttributeItem item;
    item.SetAnyTypeName(sup::dto::kStringTypeName);

    SetPropertyFromDomainAttribute(*domain_variable, domainconstants::kNameAttribute, item);
    EXPECT_EQ(item.Data<std::string>(), std::string("abc"));
  }

  {  // case when variable wasn't setup
    auto domain_variable = CreateDomainVariable(domainconstants::kLocalVariableType);
    domain_variable->AddAttribute(domainconstants::kNameAttribute, "abc");

    AttributeItem item;
    item.SetAnyTypeName(sup::dto::kStringTypeName);

    EXPECT_NO_THROW(
        SetPropertyFromDomainAttribute(*domain_variable, domainconstants::kNameAttribute, item));

    EXPECT_EQ(item.Data<std::string>(), std::string("abc"));
  }
}

//! Validating SetPropertyFromDomainAttribute helper method for the case when domain attribute
//! contains $par attribute.

TEST_F(AttributeItemTransformHelperTest, SetPropertyFromDomainAttributePlaceholderCase)
{
  // domain Wait instruction constructed by the factory doesn't have attributes
  auto instruction = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  EXPECT_TRUE(instruction->GetStringAttributes().empty());

  AttributeItem item;
  item.SetAnyTypeName(sup::dto::kInt32TypeName);
  EXPECT_TRUE(std::holds_alternative<mvvm::int32>(item.Data()));

  // Seting property from the domain containing `$` sign
  instruction->AddAttribute(domainconstants::kTimeoutAttribute, "$par1");
  EXPECT_EQ(instruction->GetAttributeString(domainconstants::kTimeoutAttribute), "$par1");
  SetPropertyFromDomainAttribute(*instruction, domainconstants::kTimeoutAttribute, item);

  // current convention in AttributeItem::SetAttributeAsString is to keep original
  // AnyTypeName after setting attribute as a string

  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_TRUE(std::holds_alternative<std::string>(item.Data()));
  EXPECT_EQ(item.Data<std::string>(), "$par1");
}

//! Validating SetPropertyFromDomainAttribute helper method for the case when domain attribute
//! contains @par attribute.

TEST_F(AttributeItemTransformHelperTest, SetPropertyFromDomainAttributeReferenceCase)
{
  // domain Wait instruction constructed by the factory doesn't have attributes
  auto instruction = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  EXPECT_TRUE(instruction->GetStringAttributes().empty());

  AttributeItem item;
  item.SetAnyTypeName(sup::dto::kInt32TypeName);
  EXPECT_TRUE(std::holds_alternative<mvvm::int32>(item.Data()));

  // Seting property from the domain containing `@` sign
  instruction->AddAttribute(domainconstants::kTimeoutAttribute, "@par1");
  EXPECT_EQ(instruction->GetAttributeString(domainconstants::kTimeoutAttribute), "@par1");
  SetPropertyFromDomainAttribute(*instruction, domainconstants::kTimeoutAttribute, item);

  // current convention in AttributeItem::SetAttributeAsString is to keep original
  // AnyTypeName after setting attribute as a string

  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_TRUE(std::holds_alternative<std::string>(item.Data()));
  EXPECT_EQ(item.Data<std::string>(), "@par1");
}

//! Testing SetDomainAttribute method.

TEST_F(AttributeItemTransformHelperTest, SetDomainAttribute)
{
  auto domain_variable = CreateDomainVariable(domainconstants::kLocalVariableType);

  AttributeItem item;
  item.SetAnyTypeName(sup::dto::kStringTypeName);
  item.SetData("abc");

  SetDomainAttribute(item, domainconstants::kNameAttribute, *domain_variable);
  EXPECT_EQ(domain_variable->GetAttributeString(domainconstants::kNameAttribute),
            std::string("abc"));
}

//! Testing SetDomainAttribute method when attribute is marked as unsetd

TEST_F(AttributeItemTransformHelperTest, SetDomainAttributeWhenUnset)
{
  auto instruction = CreateDomainInstruction(domainconstants::kWaitInstructionType);

  AttributeItem item;
  item.SetAnyTypeName(sup::dto::kFloat64TypeName);
  item.SetData(5.0);
  item.SetPresentFlag(false);

  SetDomainAttribute(item, domainconstants::kTimeoutAttribute, *instruction);
  EXPECT_FALSE(instruction->HasAttribute(domainconstants::kTimeoutAttribute));
}
