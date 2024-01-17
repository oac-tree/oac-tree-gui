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

#include "attribute_item_transform_helper.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/attribute_item.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/scalar_conversion_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/attribute_definition.h>
#include <sup/sequencer/attribute_utils.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/variable.h>

/**
 * @brief Returns true if given attribute name and value should appear as domain attributes.
 */
bool IsSuitableForDomainAttribute(const std::string &attribute_string,
                                  const std::string &attribute_value)
{
  const bool not_empty = !attribute_value.empty();
  const bool not_isroot_false =
      !(attribute_string == sequencergui::domainconstants::kIsRootAttribute
        && attribute_value == "false");
  return not_empty && not_isroot_false;
}

namespace sequencergui
{

bool IsPlaceholderAttribute(const std::string &attribute_value)
{
  return attribute_value.find_first_of('$') == 0;
}

bool IsReferenceAttribute(const std::string &attribute_value)
{
  return attribute_value.find_first_of('@') == 0;
}

AttributeItem *AddPropertyFromDefinition(const attribute_definition_t &attr,
                                         mvvm::CompoundItem &item)
{
  // Use attribute name for display name and tag name of the new property item.
  auto &property = item.AddProperty<AttributeItem>(attr.GetName());
  property.SetAnyTypeName(attr.GetType().GetTypeName());  // will set default value too
  property.SetDisplayName(attr.GetName());
  return &property;
}

template <typename T>
void SetPropertyFromDomainAttribute(const T &domain, const std::string &attribute_name,
                                    AttributeItem &item)
{
  if (!domain.HasAttribute(attribute_name))
  {
    return;
  }

  auto attribute_string = domain.GetAttributeString(attribute_name);
  if (IsPlaceholderAttribute(attribute_string) || IsReferenceAttribute(attribute_string))
  {
    item.SetAttributeAsString(attribute_string);
    return;
  }

  auto type_code = sup::gui::GetScalarTypeCode(item.GetAnyTypeName());
  auto any_type = sup::dto::AnyType(type_code);
  auto result = sup::sequencer::utils::ParseAttributeString(any_type, attribute_string);
  if (result.first)
  {
    item.SetData(sup::gui::GetVariantFromScalar(result.second));
  }
}

template void SetPropertyFromDomainAttribute<variable_t>(const variable_t &domain,
                                                         const std::string &attribute_name,
                                                         AttributeItem &item);
template void SetPropertyFromDomainAttribute<instruction_t>(const instruction_t &domain,
                                                            const std::string &attribute_name,
                                                            AttributeItem &item);

template <typename T>
void SetDomainAttribute(const AttributeItem &item, const std::string &attribute_name, T &domain)
{
  if (item.IsUnset())
  {
    return;
  }

  auto anyvalue = sup::gui::GetAnyValueFromScalar(item.Data());
  auto [success, attribute_string] = sup::sequencer::utils::CreateAttributeString(anyvalue);
  if (!success)
  {
    throw LogicErrorException("Can't create an attribute string from item property");
  }
  if (IsSuitableForDomainAttribute(attribute_name, attribute_string))
  {
    domain.AddAttribute(attribute_name, attribute_string);
  }
}

template void SetDomainAttribute<variable_t>(const AttributeItem &item,
                                             const std::string &attribute_name, variable_t &domain);
template void SetDomainAttribute<instruction_t>(const AttributeItem &item,
                                                const std::string &attribute_name,
                                                instruction_t &domain);

}  // namespace sequencergui
