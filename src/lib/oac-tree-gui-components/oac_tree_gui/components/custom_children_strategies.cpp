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

#include "custom_children_strategies.h"

#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/utils/container_utils.h>

#include <algorithm>

namespace oac_tree_gui
{

std::vector<mvvm::SessionItem *> VariableTableChildrenStrategy::GetChildren(
    const mvvm::SessionItem *item) const
{
  // workspace shows all its variables
  if (item->GetType() == WorkspaceItem::GetStaticType())
  {
    return item->GetAllItems();
  }

  // struct and arrays shows all children's of AnyValueItem base type
  if (item->GetType() == sup::gui::AnyValueStructItem::GetStaticType())
  {
    return mvvm::utils::SinglePropertyItems(*item);
  }

  if (item->GetType() == sup::gui::AnyValueArrayItem::GetStaticType())
  {
    return mvvm::utils::SinglePropertyItems(*item);
  }

  // remaining items are variables, let's allow them to show struct and arrays beneath
  static const std::vector<std::string> allowed_variable_children_types = {
      sup::gui::AnyValueStructItem::GetStaticType(), sup::gui::AnyValueArrayItem::GetStaticType()};

  std::vector<mvvm::SessionItem *> result;
  auto children = item->GetAllItems();
  auto is_correct_type = [](auto child)
  { return mvvm::utils::Contains(allowed_variable_children_types, child->GetType()); };
  std::copy_if(std::begin(children), std::end(children), std::back_inserter(result),
               is_correct_type);
  return result;
}

}  // namespace oac_tree_gui
