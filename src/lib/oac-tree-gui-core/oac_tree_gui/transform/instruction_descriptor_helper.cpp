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

#include "instruction_descriptor_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/transform/instruction_attribute_presets.h>

#include <sup/gui/model/scalar_conversion_utils.h>

#include <sup/dto/anytype_helper.h>
#include <sup/oac-tree/instruction_info.h>

namespace oac_tree_gui
{

namespace
{

//! Creates an attribute descriptor and applies the preset.
AttributeDescriptor CreateAttributeDescriptor(const std::string& domain_type,
                                              const std::string& name,
                                              const mvvm::variant_t& default_value, bool mandatory,
                                              sup::oac_tree::AttributeCategory category)
{
  AttributeDescriptor result{name, default_value, {}, mandatory, mandatory, category};
  if (name == domainconstants::kIsRootAttribute)
  {
    result.is_set = true;
  }
  if (auto preset = FindAttributePreset(domain_type, name); preset)
  {
    result.default_value = preset->value;
    result.is_set = result.is_set || preset->exposed;
  }
  return result;
}

//! Returns the default-initialized scalar for the given attribute definition.
mvvm::variant_t GetDefaultValue(const std::string& domain_type,
                                const attribute_definition_t& definition)
{
  const auto anytype = definition.GetType();
  if (!sup::dto::IsScalarType(anytype))
  {
    throw RuntimeException("Attribute [" + definition.GetName() + "] of instruction [" + domain_type
                           + "] has non-scalar type [" + anytype.GetTypeName() + "]");
  }
  return sup::gui::GetVariantFromScalarTypeName(anytype.GetTypeName());
}

}  // namespace

InstructionDescriptor CreateInstructionDescriptor(const instruction_t& instruction)
{
  const auto domain_type = instruction.GetType();
  InstructionDescriptor result{domain_type, instruction.GetCategory(), {}};
  for (const auto& definition : instruction.GetAttributeDefinitions())
  {
    result.attributes.push_back(CreateAttributeDescriptor(
        domain_type, definition.GetName(), GetDefaultValue(domain_type, definition),
        definition.IsMandatory(), definition.GetCategory()));
  }
  return result;
}

InstructionDescriptor CreateInstructionDescriptor(const sup::oac_tree::InstructionInfo& info)
{
  const auto domain_type = info.GetType();
  InstructionDescriptor result{domain_type, info.GetCategory(), {}};
  result.attributes.push_back(
      CreateAttributeDescriptor(domain_type, domainconstants::kNameAttribute, std::string{}, false,
                                sup::oac_tree::AttributeCategory::kLiteral));
  result.attributes.push_back(
      CreateAttributeDescriptor(domain_type, domainconstants::kIsRootAttribute, false, false,
                                sup::oac_tree::AttributeCategory::kLiteral));
  if (info.GetCategory() != sup::oac_tree::Instruction::kAction)
  {
    result.attributes.push_back(
        CreateAttributeDescriptor(domain_type, domainconstants::kShowCollapsedAttribute, false,
                                  false, sup::oac_tree::AttributeCategory::kLiteral));
  }
  return result;
}

}  // namespace oac_tree_gui
