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

#include "custom_children_strategies.h"

#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/session_item.h>
#include <mvvm/utils/container_utils.h>

#include <algorithm>

namespace
{

/**
 * @brief Returns copy of container, without elements in the exclude list.
 */

template <typename T, typename F>
T FilterElements(const T &container, const F &to_exclude)
{
  T result;

  auto not_in_exclude_list = [&to_exclude](const auto &x)
  { return !mvvm::utils::Contains(to_exclude, x); };

  std::copy_if(container.begin(), container.end(), std::back_inserter(result), not_in_exclude_list);

  return result;
}

}  // namespace

namespace sequencergui
{

//! ---------------------------------------------------------------------------
//! VariableChildrenStrategy
//! ---------------------------------------------------------------------------

std::vector<mvvm::SessionItem *> VariableChildrenStrategy::GetChildren(
    const mvvm::SessionItem *item) const
{
  if (!item)
  {
    return {};
  }

  const std::vector<mvvm::SessionItem *> to_exclude = {GetIsAvailableItem(*item)};

  auto result = FilterElements(item->GetAllItems(), to_exclude);

  return result;
}

//! ---------------------------------------------------------------------------
//! VariableTableChildrenStrategy
//! ---------------------------------------------------------------------------

std::vector<mvvm::SessionItem *> VariableTableChildrenStrategy::GetChildren(
    const mvvm::SessionItem *item) const
{
  static const std::vector<std::string> allowed_types = {
      WorkspaceItem::Type, sup::gui::AnyValueStructItem::Type, sup::gui::AnyValueArrayItem::Type};

  // for items from list we return all their children
  if (mvvm::utils::Contains(allowed_types, item->GetType()))
  {
    return item->GetAllItems();
  }

  // items not from the list are variables, let's allow them to show struct and arrays beneath
  static const std::vector<std::string> allowed_variable_children_types = {
      sup::gui::AnyValueStructItem::Type, sup::gui::AnyValueArrayItem::Type};

  std::vector<mvvm::SessionItem *> result;
  auto children = item->GetAllItems();
  auto is_correct_type = [](auto child)
  { return mvvm::utils::Contains(allowed_variable_children_types, child->GetType()); };
  std::copy_if(std::begin(children), std::end(children), std::back_inserter(result),
               is_correct_type);
  return result;
}

}  // namespace sequencergui
