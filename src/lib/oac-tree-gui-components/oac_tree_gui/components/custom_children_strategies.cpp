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

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/utils/container_utils.h>

#include <algorithm>

namespace oac_tree_gui
{

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

  // remaining items are variables, let's allow them to show struct and arrays beneath
  static const std::vector<std::string> allowed_variable_children_types = {
      mvvm::GetTypeName<sup::gui::AnyValueStructItem>(),
      mvvm::GetTypeName<sup::gui::AnyValueArrayItem>()};

  std::vector<mvvm::SessionItem*> result;
  auto children = item->GetAllItems();
  auto is_correct_type = [](const auto* child)
  { return mvvm::utils::Contains(allowed_variable_children_types, child->GetType()); };
  (void)std::copy_if(std::begin(children), std::end(children), std::back_inserter(result),
                     is_correct_type);
  return result;
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
          && child->GetTagIndex().GetTag() != domainconstants::kIsRootAttribute
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

}  // namespace oac_tree_gui
