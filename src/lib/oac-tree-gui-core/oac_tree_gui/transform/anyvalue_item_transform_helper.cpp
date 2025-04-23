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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalue_item_transform_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_preamble_items.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/model/variable_item.h>

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/model/item_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/attribute_definition.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/procedure_preamble.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/workspace.h>

namespace oac_tree_gui
{

void SetAnyValue(const anyvalue_t &anyvalue, VariableItem &variable_item)
{
  // in current implementation we remove old AnyValueItem, if it exists
  if (auto prev_item = variable_item.GetAnyValueItem(); prev_item)
  {
    mvvm::utils::RemoveItem(*prev_item);
  }

  mvvm::utils::InsertItem(sup::gui::CreateAnyValueItem(anyvalue), &variable_item,
                          mvvm::TagIndex::First());
}

void SetAnyValue(const anyvalue_t &anyvalue, InstructionItem &item)
{
  if (!mvvm::utils::HasTag(item, oac_tree_gui::itemconstants::kAnyValueTag))
  {
    throw LogicErrorException("This instruction is not inteded for storing AnyValueItem");
  }

  if (auto prev_item = GetAnyValueItem(item); prev_item)
  {
    mvvm::utils::RemoveItem(*prev_item);
  }

  mvvm::utils::InsertItem(sup::gui::CreateAnyValueItem(anyvalue), &item, mvvm::TagIndex::First());
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
      auto temp_anyvalue_item = sup::gui::CreateAnyValueItem(anyvalue);
      sup::gui::UpdateAnyValueItemData(*temp_anyvalue_item, *existing_anyvalue_item);
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

void PopulateProcedurePreamble(const ProcedurePreambleItem &item, preamble_t &preamble)
{
  using sup::oac_tree::TypeRegistrationInfo;

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

sup::dto::AnyValue GetAnyValue(const VariableItem &item)
{
  if (auto anyvalue_item = item.GetAnyValueItem(); anyvalue_item)
  {
    return CreateAnyValue(*item.GetAnyValueItem());
  }

  throw RuntimeException("Given VariableItem doesn't contain AnyValueItem on board");
}

sup::dto::AnyValue GetAnyValue(const std::string &var_name,
                               const sup::oac_tree::Workspace &workspace)
{
  sup::dto::AnyValue result;
  if (!workspace.GetValue(var_name, result))
  {
    throw RuntimeException("Can't get the value of the variable +[" + var_name
                           + "] from domain workspace");
  }
  return result;
}

}  // namespace oac_tree_gui
