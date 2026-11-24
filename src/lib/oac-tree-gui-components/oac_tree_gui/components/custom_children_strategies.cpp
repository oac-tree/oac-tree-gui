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

#include "custom_children_strategies.h"

#include "component_helper.h"

#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>

#include <algorithm>

namespace oac_tree_gui
{

namespace
{

//! Returns the parent of the item, or the parent of the attribute container holding the item.
mvvm::SessionItem* GetParentSkippingAttributes(mvvm::SessionItem* item)
{
  auto* parent = item->GetParent();
  return ((parent != nullptr) && (parent->GetTagIndex().GetTag() == itemconstants::kAttributes))
             ? parent->GetParent()
             : parent;
}

}  // namespace

std::vector<mvvm::SessionItem*> VariableTableChildrenStrategy::GetChildren(
    mvvm::SessionItem* item) const
{
  // workspace shows all its variables
  if (item->GetType() == mvvm::GetTypeName<WorkspaceItem>())
  {
    return item->GetAllItems();
  }

  // struct and arrays shows all children's of AnyValueItem base type
  if (item->GetType() == mvvm::GetTypeName<sup::gui::AnyValueStructItem>())
  {
    return mvvm::utils::SinglePropertyItems(*item);
  }

  if (item->GetType() == mvvm::GetTypeName<sup::gui::AnyValueArrayItem>())
  {
    return mvvm::utils::SinglePropertyItems(*item);
  }

  // variables show struct and arrays beneath, unless the value is shown in the variable row
  if (auto* variable = dynamic_cast<VariableItem*>(item); variable != nullptr)
  {
    auto* anyvalue_item = variable->GetAnyValueItem();
    if ((anyvalue_item != nullptr) && (anyvalue_item->IsStruct() || anyvalue_item->IsArray())
        && (GetInlineValueItem(*variable) == nullptr))
    {
      return {anyvalue_item};
    }
  }

  return {};
}

std::vector<mvvm::SessionItem*> WorkspaceEditorChildrenStrategy::GetChildren(
    mvvm::SessionItem* item) const
{
  if (item == nullptr)
  {
    return {};
  }

  auto is_visible = [](const auto* child) { return child->IsVisible(); };

  std::vector<mvvm::SessionItem*> result;
  for (auto* child : item->GetAllItems())
  {
    if (!child->IsVisible())
    {
      continue;
    }

    if (child->GetTagIndex().GetTag() == itemconstants::kAttributes)
    {
      auto attributes = child->GetAllItems();
      (void)std::copy_if(attributes.begin(), attributes.end(), std::back_inserter(result),
                         is_visible);
    }
    else
    {
      result.push_back(child);
    }
  }
  return result;
}

mvvm::SessionItem* WorkspaceEditorChildrenStrategy::GetViewParent(mvvm::SessionItem* item) const
{
  return GetParentSkippingAttributes(item);
}

std::vector<mvvm::SessionItem*> InstructionNodeChildrenStrategy::GetChildren(
    mvvm::SessionItem* item) const
{
  if (auto* instruction_item = dynamic_cast<InstructionItem*>(item); instruction_item != nullptr)
  {
    if (!IsCollapsed(*instruction_item))
    {
      std::vector<mvvm::SessionItem*> result;
      auto children = instruction_item->GetInstructions();
      (void)std::copy(children.begin(), children.end(), std::back_inserter(result));
      return result;
    }
    return {};
  }

  // instruction container
  return mvvm::utils::TopLevelItems(*item);
}

ScopedInstructionNodeChildrenStrategy::ScopedInstructionNodeChildrenStrategy(
    std::set<std::string> hidden_identifiers)
    : mvvm::IChildrenStrategy()
    , m_hidden_identifiers(std::move(hidden_identifiers))
    , m_instruction_strategy()
{
}

std::vector<mvvm::SessionItem*> ScopedInstructionNodeChildrenStrategy::GetChildren(
    mvvm::SessionItem* item) const
{
  if (auto* instruction_item = dynamic_cast<InstructionItem*>(item); instruction_item != nullptr)
  {
    const auto* top_level = GetTopLevelInstruction(*instruction_item);
    if ((top_level != nullptr) && IsHidden(*top_level))
    {
      return {};
    }
    return m_instruction_strategy.GetChildren(item);
  }

  // instruction container
  std::vector<mvvm::SessionItem*> result;
  const auto children = m_instruction_strategy.GetChildren(item);
  (void)std::copy_if(children.begin(), children.end(), std::back_inserter(result),
                     [this](const mvvm::SessionItem* child) { return !IsHidden(*child); });
  return result;
}

bool ScopedInstructionNodeChildrenStrategy::IsHidden(const mvvm::SessionItem& item) const
{
  return m_hidden_identifiers.find(item.GetIdentifier()) != m_hidden_identifiers.end();
}

std::vector<mvvm::SessionItem*> InstructionAttributeChildrenStrategy::GetChildren(
    mvvm::SessionItem* item) const
{
  if (item == nullptr)
  {
    return {};
  }

  auto visible_properties = [](mvvm::SessionItem& parent)
  {
    std::vector<mvvm::SessionItem*> result;
    for (auto* child : parent.GetAllItems())
    {
      if (child->IsVisible() && child->HasFlag(mvvm::kProperty)
          && child->GetTagIndex().GetTag() != itemconstants::kAttributes)
      {
        result.push_back(child);
      }
    }
    return result;
  };

  auto* instruction = dynamic_cast<InstructionItem*>(item);
  if ((instruction == nullptr) || (instruction->GetAttributes() == nullptr))
  {
    return visible_properties(*item);
  }

  // domain attributes, followed by GUI properties
  auto result = visible_properties(*instruction->GetAttributes());
  auto gui_properties = visible_properties(*instruction);
  (void)result.insert(result.end(), gui_properties.begin(), gui_properties.end());
  return result;
}

mvvm::SessionItem* InstructionAttributeChildrenStrategy::GetViewParent(
    mvvm::SessionItem* item) const
{
  return GetParentSkippingAttributes(item);
}

}  // namespace oac_tree_gui
