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

#include "attribute_item_transform_helper.h"

#include <oac-tree-gui/core/exceptions.h>
#include <oac-tree-gui/domain/domain_constants.h>

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/gui/model/scalar_conversion_utils.h>

#include <mvvm/model/item_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/attribute_definition.h>
#include <sup/oac-tree/attribute_utils.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/variable.h>

/**
 * @brief Returns true if given attribute name and value should appear as domain attributes.
 */
bool IsSuitableForDomainAttribute(const std::string &attribute_string,
                                  const std::string &attribute_value)
{
  const bool not_empty = !attribute_value.empty();
  const bool not_isroot_false =
      !(attribute_string == oac_tree_gui::domainconstants::kIsRootAttribute
        && attribute_value == "false");
  return not_empty && not_isroot_false;
}

/**
 * @brief Returns is exposed flag for given attribute definition
 */
bool GetIsExposedFlag(const attribute_definition_t &attr)
{
  // list of domain attributes that should be always marked as present
  static const std::vector<std::string> kAlwaysExposedAttributeList = {
      oac_tree_gui::domainconstants::kNameAttribute};
  return attr.IsMandatory() || mvvm::utils::Contains(kAlwaysExposedAttributeList, attr.GetName());
}

namespace oac_tree_gui
{

bool IsPlaceholderAttribute(const std::string &attribute_value)
{
  return attribute_value.find_first_of('$') == 0;
}

bool IsReferenceAttribute(const std::string &attribute_value)
{
  return attribute_value.find_first_of('@') == 0;
}

bool GetAttributeExposedFlag(const sup::gui::AnyValueItem &attribute_item)
{
  return attribute_item.IsEditable() && attribute_item.IsEnabled();
}

void SetAttributeExposedFlag(bool value, sup::gui::AnyValueItem &attribute_item)
{
  attribute_item.SetEditable(value);
  attribute_item.SetEnabled(value);
}

void SetAttributeAsString(const std::string &value, sup::gui::AnyValueItem &attribute_item)
{
  // current convention is to keep original AnyTypeName after setting attribute as a string
  mvvm::utils::ReplaceData(attribute_item, mvvm::variant_t(value), mvvm::DataRole::kData);
}

void SetAttributeFromTypeName(sup::gui::AnyValueItem &attribute_item)
{
  mvvm::utils::ReplaceData(attribute_item,
                           sup::gui::GetVariantFromScalarTypeName(attribute_item.GetAnyTypeName()),
                           mvvm::DataRole::kData);
}

sup::gui::AnyValueItem *AddPropertyFromDefinition(const attribute_definition_t &attr,
                                                  mvvm::CompoundItem &item)
{
  // Use attribute name for display name and tag name of the new property item.
  auto &property = item.AddProperty<sup::gui::AnyValueScalarItem>(attr.GetName());
  property.SetAnyTypeName(attr.GetType().GetTypeName());  // will set default value too
  property.SetDisplayName(attr.GetName());
  SetAttributeExposedFlag(GetIsExposedFlag(attr), property);
  return &property;
}

template <typename T>
void SetPropertyFromDomainAttribute(const T &domain, const std::string &attribute_name,
                                    sup::gui::AnyValueItem &item)
{
  if (!domain.HasAttribute(attribute_name))
  {
    return;
  }
  SetAttributeExposedFlag(true, item);

  auto attribute_string = domain.GetAttributeString(attribute_name);
  if (IsPlaceholderAttribute(attribute_string) || IsReferenceAttribute(attribute_string))
  {
    SetAttributeAsString(attribute_string, item);
    return;
  }

  auto type_code = sup::gui::GetScalarTypeCode(item.GetAnyTypeName());
  auto any_type = sup::dto::AnyType(type_code);
  auto result = sup::oac_tree::utils::ParseAttributeString(any_type, attribute_string);
  if (result.first)
  {
    item.SetData(sup::gui::GetVariantFromScalar(result.second));
  }
}

template void SetPropertyFromDomainAttribute<variable_t>(const variable_t &domain,
                                                         const std::string &attribute_name,
                                                         sup::gui::AnyValueItem &item);
template void SetPropertyFromDomainAttribute<instruction_t>(const instruction_t &domain,
                                                            const std::string &attribute_name,
                                                            sup::gui::AnyValueItem &item);

template <typename T>
void SetDomainAttribute(const sup::gui::AnyValueItem &item, const std::string &attribute_name,
                        T &domain)
{
  if (!GetAttributeExposedFlag(item))
  {
    return;
  }

  auto anyvalue = sup::gui::GetAnyValueFromScalar(item.Data());
  auto [success, attribute_string] = sup::oac_tree::utils::CreateAttributeString(anyvalue);
  if (!success)
  {
    throw LogicErrorException("Can't create an attribute string from item property");
  }
  if (IsSuitableForDomainAttribute(attribute_name, attribute_string))
  {
    domain.AddAttribute(attribute_name, attribute_string);
  }
}

template void SetDomainAttribute<variable_t>(const sup::gui::AnyValueItem &item,
                                             const std::string &attribute_name, variable_t &domain);
template void SetDomainAttribute<instruction_t>(const sup::gui::AnyValueItem &item,
                                                const std::string &attribute_name,
                                                instruction_t &domain);

template <typename DomainT>
bool HasAttributeDefinition(const DomainT &domain, const std::string &definition_name)
{
  auto definitions = domain.GetAttributeDefinitions();
  auto on_element = [&definition_name](const auto &elem)
  { return elem.GetName() == definition_name; };
  return std::any_of(std::begin(definitions), std::end(definitions), on_element);
}

template bool HasAttributeDefinition<variable_t>(const variable_t &domain,
                                                 const std::string &definition_name);
template bool HasAttributeDefinition<instruction_t>(const instruction_t &domain,
                                                    const std::string &definition_name);

template <typename DomainT>
void SetJsonAttributesFromItem(const sup::gui::AnyValueItem &item, DomainT &domain)
{
  auto anyvalue = sup::gui::CreateAnyValue(item);
  if (sup::dto::IsEmptyValue(anyvalue))
  {
    return;
  }

  if (HasAttributeDefinition(domain, domainconstants::kTypeAttribute))
  {
    domain.AddAttribute(domainconstants::kTypeAttribute, sup::gui::AnyTypeToJSONString(anyvalue));
  }
  if (HasAttributeDefinition(domain, domainconstants::kValueAttribute))
  {
    domain.AddAttribute(domainconstants::kValueAttribute, sup::gui::ValuesToJSONString(anyvalue));
  }
}

template void SetJsonAttributesFromItem<variable_t>(const sup::gui::AnyValueItem &item,
                                                    variable_t &domain);
template void SetJsonAttributesFromItem<instruction_t>(const sup::gui::AnyValueItem &item,
                                                       instruction_t &domain);

}  // namespace oac_tree_gui
