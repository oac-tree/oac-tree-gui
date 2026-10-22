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

#include "attribute_descriptor_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/transform/instruction_attribute_presets.h>

#include <sup/gui/model/scalar_conversion_utils.h>

#include <sup/dto/anytype_helper.h>

namespace oac_tree_gui
{

namespace
{

//! Returns true if the attribute accepts text. GUI attributes and strings take no text.
bool IsTextAllowed(const std::string& name, const mvvm::variant_t& default_value)
{
  return name != domainconstants::kIsRootAttribute
         && name != domainconstants::kShowCollapsedAttribute && !IsString(default_value);
}

//! Returns true if the attribute can have no value. Mandatory attributes, isRoot and strings
//! always have one.
bool IsNullable(const std::string& name, const mvvm::variant_t& default_value, bool mandatory)
{
  return !mandatory && name != domainconstants::kIsRootAttribute && !IsString(default_value);
}

//! Returns the default-initialized scalar for the given attribute definition.
mvvm::variant_t GetDefaultValue(const std::string& domain_type,
                                const attribute_definition_t& definition)
{
  const auto anytype = definition.GetType();
  if (!sup::dto::IsScalarType(anytype))
  {
    throw RuntimeException("Attribute [" + definition.GetName() + "] of [" + domain_type
                           + "] has non-scalar type [" + anytype.GetTypeName() + "]");
  }
  return sup::gui::GetVariantFromScalarTypeName(anytype.GetTypeName());
}

}  // namespace

AttributeDescriptor CreateAttributeDescriptor(const std::string& domain_type,
                                              const std::string& name,
                                              const mvvm::variant_t& default_value, bool mandatory,
                                              sup::oac_tree::AttributeCategory category)
{
  AttributeDescriptor result{name, default_value, {}, mandatory, false, category};
  result.allow_text = IsTextAllowed(name, default_value);
  result.nullable = IsNullable(name, default_value, mandatory);
  if (name == domainconstants::kIsRootAttribute)
  {
    result.visible = false;
  }
  if (auto preset = FindAttributePreset(domain_type, name); preset)
  {
    result.default_value = preset->value;
    result.has_preset = true;
  }
  return result;
}

AttributeDescriptor CreateAttributeDescriptor(const std::string& domain_type,
                                              const attribute_definition_t& definition)
{
  return CreateAttributeDescriptor(domain_type, definition.GetName(),
                                   GetDefaultValue(domain_type, definition),
                                   definition.IsMandatory(), definition.GetCategory());
}

}  // namespace oac_tree_gui
