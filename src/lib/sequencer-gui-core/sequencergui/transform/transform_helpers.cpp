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

#include "transform_helpers.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/variable_item.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/gui/model/scalar_conversion_utils.h>

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/item_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/attribute_handler.h>
#include <sup/sequencer/attribute_utils.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/variable.h>

namespace sequencergui
{

void SetAnyValue(const anyvalue_t &anyvalue, VariableItem &variable_item)
{
  // we will be acting through the model, if it exists, to allow signaling
  auto model = variable_item.GetModel();

  // in current implementation we remove old AnyValueItem, if it exists
  if (auto prev_item = variable_item.GetAnyValueItem(); prev_item)
  {
    if (model)
    {
      model->RemoveItem(prev_item);
    }
    else
    {
      variable_item.TakeItem(variable_item.TagIndexOfItem(prev_item));
    }
  }

  // Inserting new AnyValueItem
  auto anyvalue_item = sup::gui::CreateItem(anyvalue);
  if (anyvalue_item->IsScalar())
  {
    anyvalue_item->SetDisplayName("value");
    anyvalue_item->SetToolTip(anyvalue_item->GetAnyTypeName());
  }

  if (model)
  {
    model->InsertItem(std::move(anyvalue_item), &variable_item, {});
  }
  else
  {
    variable_item.InsertItem(std::move(anyvalue_item), {});
  }
}

void SetAnyValueFromJsonType(const std::string &json_type, VariableItem &variable_item)
{
  auto anytype = ::sup::gui::AnyTypeFromJSONString(json_type);
  const sup::dto::AnyValue anyvalue(anytype);
  SetAnyValue(anyvalue, variable_item);
}

void SetAnyValueFromDomainVariable(const variable_t &variable, VariableItem &variable_item)
{
  if (variable.HasAttribute(domainconstants::kTypeAttribute))
  {
    auto anytype = sup::gui::AnyTypeFromJSONString(
        variable.GetAttributeString(domainconstants::kTypeAttribute));

    auto get_anyvalue = [&anytype, &variable]()
    {
      if (variable.HasAttribute(domainconstants::kValueAttribute))
      {
        return sup::gui::AnyValueFromJSONString(
            anytype, variable.GetAttributeString(domainconstants::kValueAttribute));
      }
      else
      {
        return sup::dto::AnyValue(anytype);
      }
    };

    sup::dto::AnyValue anyvalue = get_anyvalue();  // executing lambda at initialisation
    SetAnyValue(anyvalue, variable_item);
  }
}

void UpdateAnyValue(const anyvalue_t &anyvalue, VariableItem &variable_item)
{
  if (auto existing_anyvalue_item = variable_item.GetAnyValueItem(); existing_anyvalue_item)
  {
    // If AnyValueItem already exists, we assume that its layout coincide with AnyValue.

    // updating existing AnyValueItem using temporary AnyValueItem
    try
    {
      auto temp_anyvalue_item = sup::gui::CreateItem(anyvalue);
      sup::gui::UpdateAnyValueItemData(*temp_anyvalue_item.get(), *existing_anyvalue_item);
      return;
    }
    catch (const std::exception &ex)
    {
      // Update fails if the old AnyValueItem has a different layout than the new AnyValueItem. This
      // can happen in two cases: 1) ResetVariableInstruction is in charge and it just reset
      // LocalVariable with some new value. 2) Something wrong is going on and the GUI
      // AnyValueItem's type went out-of-sync with the domain.

      // We don't know how to distinguish these two cases. For the moment we choose to regenerate
      // AnyValueItem from scratch using new domain value.
    }
  }

  // Create brand new AnyValueItem using AnyValue provided
  SetAnyValue(anyvalue, variable_item);
}

void AddNonEmptyAttribute(const std::string &attribute_name, const std::string &attribute_value,
                          variable_t &variable)
{
  if (!attribute_value.empty())
  {
    variable.AddAttribute(attribute_name, attribute_value);
  }
}

void SetJsonTypeAttribute(const VariableItem &item, variable_t &variable)
{
  if (auto anyvalue_item = item.GetAnyValueItem(); anyvalue_item)
  {
    // if AnyValueItem is defined, jenerate JSON TYPE attribute from it
    auto anyvalue = sup::gui::CreateAnyValue(*anyvalue_item);

    AddNonEmptyAttribute(domainconstants::kTypeAttribute, sup::gui::AnyTypeToJSONString(anyvalue),
                         variable);
  }
  else
  {
    std::string error_message = "Can't setup JSON type for variable name [" + item.GetName()
                                + "] if AnyValueItem is absent";
    throw LogicErrorException(error_message);
  }
}

void SetJsonValueAttribute(const VariableItem &item, variable_t &variable)
{
  if (auto anyvalue_item = item.GetAnyValueItem(); anyvalue_item)
  {
    // if AnyValueItem is defined, jenerate JSON TYPE attribute from it
    auto anyvalue = sup::gui::CreateAnyValue(*anyvalue_item);

    AddNonEmptyAttribute(domainconstants::kValueAttribute, sup::gui::ValuesToJSONString(anyvalue),
                         variable);
  }
  else
  {
    std::string error_message = "Can't setup JSON value for variable name [" + item.GetName()
                                + "] if AnyValueItem is absent";
    throw LogicErrorException(error_message);
  }
}

void AddNonEmptyAttribute(const std::string &attribute_name, const std::string &attribute_value,
                          instruction_t &instruction)
{
  if (!attribute_value.empty())
  {
    instruction.AddAttribute(attribute_name, attribute_value);
  }
}

sup::gui::AnyValueItem *AddPropertyFromDefinition(const attribute_definition_t &attr,
                                                  mvvm::CompoundItem &item)
{
  // In the absence of other sources of the information we can only use attribute name
  // for both, display name and tag name of the new property item.
  auto property = item.AddProperty<sup::gui::AnyValueScalarItem>(attr.GetName());
  property->SetAnyTypeName(attr.GetType().GetTypeName());
  property->SetDisplayName(attr.GetName());

  auto default_anyvalue = sup::dto::AnyValue(attr.GetType());
  sup::gui::SetDataFromScalar(default_anyvalue, *property);

  return property;
}

template <typename T>
void SetPropertyFromDomainAttribute(const T &domain, const std::string &attribute_name,
                                    sup::gui::AnyValueScalarItem &item)
{
  try
  {
    auto anyvalue = domain.template GetAttributeValue<sup::dto::AnyValue>(attribute_name);
    sup::gui::SetDataFromScalar(anyvalue, item);
  }
  catch (const std::exception)
  {
    // It means that object wasn't set.
  }
}

template void SetPropertyFromDomainAttribute<variable_t>(const variable_t &domain,
                                                         const std::string &attribute_name,
                                                         sup::gui::AnyValueScalarItem &item);
template void SetPropertyFromDomainAttribute<instruction_t>(const instruction_t &domain,
                                                            const std::string &attribute_name,
                                                            sup::gui::AnyValueScalarItem &item);

template <typename T>
void SetDomainAttribute(const sup::gui::AnyValueScalarItem &item, const std::string &attribute_name,
                        T &domain)
{
  auto anyvalue = sup::gui::GetAnyValueFromScalar(item);
  auto [success, attribute_string] = sup::sequencer::utils::CreateAttributeString(anyvalue);
  if (!success)
  {
    throw LogicErrorException("Can't create an attribute string from item property");
  }
  if (!attribute_string.empty())
  {
    domain.AddAttribute(attribute_name, attribute_string);
  }
}

template void SetDomainAttribute<variable_t>(const sup::gui::AnyValueScalarItem &item,
                                             const std::string &attribute_name, variable_t &domain);
template void SetDomainAttribute<instruction_t>(const sup::gui::AnyValueScalarItem &item,
                                                const std::string &attribute_name,
                                                instruction_t &domain);

void RegisterChildrenTag(const instruction_t &instruction, mvvm::CompoundItem &item)
{
  if (instruction.GetCategory() == sup::sequencer::Instruction::kCompound)
  {
    item.RegisterTag(mvvm::TagInfo::CreateUniversalTag(itemconstants::kChildInstructions),
                     /*as_default*/ true);
  }
  else if (instruction.GetCategory() == sup::sequencer::Instruction::kDecorator)
  {
    item.RegisterTag(mvvm::TagInfo(itemconstants::kChildInstructions, 0, 1, {}),
                     /*as_default*/ true);
  }
}

}  // namespace sequencergui
