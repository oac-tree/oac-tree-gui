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

#include "transform_helpers.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/procedure_preamble_items.h>
#include <sequencergui/model/universal_item_helper.h>
#include <sequencergui/model/variable_item.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/model/item_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/attribute_definition.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure_preamble.h>
#include <sup/sequencer/variable.h>

#include <algorithm>

namespace sequencergui
{

void SetAnyValue(const anyvalue_t &anyvalue, VariableItem &variable_item)
{
  // in current implementation we remove old AnyValueItem, if it exists
  if (auto prev_item = variable_item.GetAnyValueItem(); prev_item)
  {
    mvvm::utils::RemoveItem(*prev_item);
  }

  mvvm::utils::InsertItem(sup::gui::CreateItem(anyvalue), &variable_item, mvvm::TagIndex::First());
}

void SetAnyValue(const anyvalue_t &anyvalue, InstructionItem &item)
{
  if (!mvvm::utils::HasTag(item, sequencergui::itemconstants::kAnyValueTag))
  {
    throw LogicErrorException("This instruction is not inteded for storing AnyValueItem");
  }

  if (auto prev_item = GetAnyValueItem(item); prev_item)
  {
    mvvm::utils::RemoveItem(*prev_item);
  }

  mvvm::utils::InsertItem(sup::gui::CreateItem(anyvalue), &item, mvvm::TagIndex::First());
}

void SetAnyValueFromDomainVariable(const variable_t &variable, VariableItem &variable_item,
                                   const anytype_registry_t *registry)
{
  if (variable.HasAttribute(domainconstants::kTypeAttribute))
  {
    auto anytype = sup::gui::AnyTypeFromJSONString(
        variable.GetAttributeString(domainconstants::kTypeAttribute), registry);

    auto get_anyvalue = [&anytype, &variable]()
    {
      if (variable.HasAttribute(domainconstants::kValueAttribute))
      {
        return sup::gui::AnyValueFromJSONString(
            anytype, variable.GetAttributeString(domainconstants::kValueAttribute));
      }
      return sup::dto::AnyValue(anytype);
    };

    sup::dto::AnyValue anyvalue = get_anyvalue();  // executing lambda at initialisation
    SetAnyValue(anyvalue, variable_item);
  }
}

void SetAnyValueFromDomainInstruction(const instruction_t &instruction, InstructionItem &item)
{
  if (instruction.HasAttribute(domainconstants::kTypeAttribute)
      && instruction.HasAttribute(domainconstants::kValueAttribute))
  {
    auto anytype = sup::gui::AnyTypeFromJSONString(
        instruction.GetAttributeString(domainconstants::kTypeAttribute));

    auto anyvalue = sup::gui::AnyValueFromJSONString(
        anytype, instruction.GetAttributeString(domainconstants::kValueAttribute));

    SetAnyValue(anyvalue, item);
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

template <typename DomainT>
void AddNonEmptyAttribute(const std::string &attribute_name, const std::string &attribute_value,
                          DomainT &domain)
{
  if (!attribute_value.empty())
  {
    domain.AddAttribute(attribute_name, attribute_value);
  }
}

template void AddNonEmptyAttribute<variable_t>(const std::string &attribute_name,
                                               const std::string &attribute_value,
                                               variable_t &domain);
template void AddNonEmptyAttribute<instruction_t>(const std::string &attribute_name,
                                                  const std::string &attribute_value,
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

void PopulateProcedurePreamble(const ProcedurePreambleItem &item, preamble_t &preamble)
{
  using sup::sequencer::TypeRegistrationInfo;

  if (!preamble.GetPluginPaths().empty() || !preamble.GetTypeRegistrations().empty())
  {
    throw LogicErrorException("ProcedurePreamble must be empty");
  }

  for (const auto &str : item.GetPluginPaths())
  {
    preamble.AddPluginPath(str);
  }

  for (const auto &[mode, str] : item.GetTypeRegistrations())
  {
    preamble.AddTypeRegistration(
        TypeRegistrationInfo(static_cast<TypeRegistrationInfo::RegistrationMode>(mode), str));
  }
}

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
bool HasJsonTypeAndNameAttributes(const DomainT &domain)
{
  return HasAttributeDefinition(domain, domainconstants::kTypeAttribute)
         && HasAttributeDefinition(domain, domainconstants::kValueAttribute);
}

template bool HasJsonTypeAndNameAttributes<variable_t>(const variable_t &domain);
template bool HasJsonTypeAndNameAttributes<instruction_t>(const instruction_t &domain);

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

}  // namespace sequencergui
