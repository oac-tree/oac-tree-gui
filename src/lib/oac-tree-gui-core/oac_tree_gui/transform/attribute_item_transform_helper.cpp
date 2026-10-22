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

#include "attribute_item_transform_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/attribute_item.h>

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/gui/model/scalar_conversion_utils.h>


#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/attribute_definition.h>
#include <sup/oac-tree/attribute_utils.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/variable.h>

namespace oac_tree_gui
{

namespace
{
/**
 * @brief Returns true if given attribute name and value should appear as domain attributes.
 */
bool IsSuitableForDomainAttribute(const std::string& attribute_string,
                                  const std::string& attribute_value)
{
  const bool not_empty = !attribute_value.empty();
  const bool not_isroot_false =
      !(attribute_string == oac_tree_gui::domainconstants::kIsRootAttribute
        && attribute_value == "false");
  return not_empty && not_isroot_false;
}

}  // namespace

bool IsPlaceholderAttribute(const std::string& attribute_value)
{
  return attribute_value.find_first_of('$') == 0;
}

bool IsReferenceAttribute(const std::string& attribute_value)
{
  return attribute_value.find_first_of('@') == 0;
}

void SetAttributeFromString(const std::string& attribute_string, AttributeItem& item)
{
  if (item.IsTextAllowed()
      && (IsPlaceholderAttribute(attribute_string) || IsReferenceAttribute(attribute_string)))
  {
    (void)item.SetText(attribute_string);
    return;
  }

  const auto any_type = sup::gui::GetAnyValueFromScalar(item.GetDefaultValue()).GetType();
  auto [success, value] = sup::oac_tree::utils::ParseAttributeString(any_type, attribute_string);
  if (success)
  {
    (void)item.SetValue(sup::gui::GetVariantFromScalar(value));
  }
}

std::string GetAttributeString(const AttributeItem& item)
{
  if (item.GetState() == AttributeState::kText)
  {
    return item.GetText();
  }

  if (item.GetState() == AttributeState::kNotSet)
  {
    return {};
  }

  auto [success, result] =
      sup::oac_tree::utils::CreateAttributeString(sup::gui::GetAnyValueFromScalar(item.Data()));
  if (!success)
  {
    throw LogicErrorException("Can't create an attribute string from attribute item");
  }
  return result;
}

template <typename T>
void SetDomainAttribute(const AttributeItem& item, const std::string& attribute_name, T& domain)
{
  if (const auto attribute_string = GetAttributeString(item);
      IsSuitableForDomainAttribute(attribute_name, attribute_string))
  {
    (void)domain.AddAttribute(attribute_name, attribute_string);
  }
}

template void SetDomainAttribute<instruction_t>(const AttributeItem& item,
                                                const std::string& attribute_name,
                                                instruction_t& domain);
template void SetDomainAttribute<variable_t>(const AttributeItem& item,
                                             const std::string& attribute_name, variable_t& domain);

template <typename DomainT>
bool HasAttributeDefinition(const DomainT& domain, const std::string& definition_name)
{
  auto definitions = domain.GetAttributeDefinitions();
  auto on_element = [&definition_name](const auto& elem)
  { return elem.GetName() == definition_name; };
  return std::any_of(std::begin(definitions), std::end(definitions), on_element);
}

template bool HasAttributeDefinition<variable_t>(const variable_t& domain,
                                                 const std::string& definition_name);

template <typename DomainT>
void SetJsonAttributesFromItem(const sup::gui::AnyValueItem& item, DomainT& domain)
{
  auto anyvalue = sup::gui::CreateAnyValue(item);
  if (sup::dto::IsEmptyValue(anyvalue))
  {
    return;
  }

  if (HasAttributeDefinition(domain, domainconstants::kTypeAttribute))
  {
    (void)domain.AddAttribute(domainconstants::kTypeAttribute,
                              sup::gui::AnyTypeToJSONString(anyvalue));
  }
  if (HasAttributeDefinition(domain, domainconstants::kValueAttribute))
  {
    (void)domain.AddAttribute(domainconstants::kValueAttribute,
                              sup::gui::ValuesToJSONString(anyvalue));
  }
}

template void SetJsonAttributesFromItem<variable_t>(const sup::gui::AnyValueItem& item,
                                                    variable_t& domain);
template void SetJsonAttributesFromItem<instruction_t>(const sup::gui::AnyValueItem& item,
                                                       instruction_t& domain);

}  // namespace oac_tree_gui
