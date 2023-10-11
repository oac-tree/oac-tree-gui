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

#include "attribute_item_transform_helper.h"

#include <sequencergui/model/attribute_item.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/scalar_conversion_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/attribute_definition.h>
#include <sup/sequencer/attribute_utils.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/variable.h>

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

AttributeItem *AddPropertyFromDefinitionV2(const attribute_definition_t &attr,
                                           mvvm::CompoundItem &item)
{
  // Use attribute name for display name and tag name of the new property item.
  auto property = item.AddProperty<AttributeItem>(attr.GetName());
  property->SetAnyTypeName(attr.GetType().GetTypeName());  // will set default value too
  property->SetDisplayName(attr.GetName());
  return property;
}

template <typename T>
void SetPropertyFromDomainAttributeV2(const T &domain, const std::string &attribute_name,
                                      AttributeItem &item)
{
  auto attribute_string = domain.GetAttributeString(attribute_name);
  if (IsPlaceholderAttribute(attribute_string) || IsReferenceAttribute(attribute_string))
  {
    item.SetAttributeAsString(attribute_string);
    return;
  }

  auto type_code = sup::gui::GetScalarTypeCode(item.GetAnyTypeName());
  auto any_type = sup::dto::AnyType(sup::gui::GetScalarTypeCode(item.GetAnyTypeName()));
  auto result = sup::sequencer::utils::ParseAttributeString(any_type, attribute_string);
  if (result.first)
  {
    item.SetData(sup::gui::GetVariantFromScalar(result.second));
  }
}

template void SetPropertyFromDomainAttributeV2<variable_t>(const variable_t &domain,
                                                           const std::string &attribute_name,
                                                           AttributeItem &item);
template void SetPropertyFromDomainAttributeV2<instruction_t>(const instruction_t &domain,
                                                              const std::string &attribute_name,
                                                              AttributeItem &item);

}  // namespace sequencergui
