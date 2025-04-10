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

#include "connectable_view_map.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/nodeeditor/connectable_view.h>

#include <stack>

namespace oac_tree_gui
{

ConnectableViewMap::ConnectableViewMap() = default;

//! Insert instruction item and corresponding view in a map.
//! Will throw if such instruction already exist. Doesn't take an ownership.

void ConnectableViewMap::Insert(const InstructionItem *item, ConnectableView *view_item)
{
  auto it = m_item_to_view.find(item);
  if (it != m_item_to_view.end())
  {
    throw RuntimeException("Connectable view already exists for given instruction");
  }
  m_item_to_view.insert(it, {item, view_item});
}

//! Insert instruction item and corresponding view in a map.
//! If such item already exists, will replace corresponding view with the given one.

void ConnectableViewMap::InsertOrAssign(const InstructionItem *item, ConnectableView *view_item)
{
  m_item_to_view.insert_or_assign(item, view_item);
}

//! Find view for given item.

ConnectableView *ConnectableViewMap::FindView(const InstructionItem *item)
{
  auto it = m_item_to_view.find(item);
  return it == m_item_to_view.end() ? nullptr : it->second;
}

//! Removes item and its corresponding view from the map.
//! Will throw if no item exists.

void ConnectableViewMap::Remove(const InstructionItem *item)
{
  auto it = m_item_to_view.find(item);
  if (it != m_item_to_view.end())
  {
    m_item_to_view.erase(it);
  }
  else
  {
    throw RuntimeException("No connectable view found for given instruction item");
  }
}

//! Removes view and its corresponding item from the map.

void ConnectableViewMap::Remove(ConnectableView *view)
{
  auto it = std::find_if(m_item_to_view.begin(), m_item_to_view.end(),
                         [view](auto it) { return it.second == view; });

  if (it != m_item_to_view.end())
  {
    m_item_to_view.erase(it);
  }
  else
  {
    throw RuntimeException("No connectable view found");
  }
}

//! Removes item and its corresponding view from the map.
//! View will be returned to the user.

ConnectableView *ConnectableViewMap::TakeView(const InstructionItem *item)
{
  auto it = m_item_to_view.find(item);
  if (it != m_item_to_view.end())
  {
    m_item_to_view.erase(it);
  }
  else
  {
    throw RuntimeException("No connectable view found for given instruction item");
  }

  return it->second;
}

//! Clears the map.

void ConnectableViewMap::Clear()
{
  m_item_to_view.clear();
}

//! Returns vector of all views related to given item.
//! These are the view of the item, and views of all teim's children.

std::vector<ConnectableView *> ConnectableViewMap::FindRelatedViews(const InstructionItem *item)
{
  std::vector<ConnectableView *> result;
  std::stack<const InstructionItem *> stack;

  stack.push(item);
  while (!stack.empty())
  {
    auto instruction = stack.top();
    stack.pop();
    if (auto view = FindView(instruction); view)
    {
      result.push_back(view);
    }
    auto children = instruction->GetInstructions();
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      stack.push(*it);
    }
  }
  return result;
}

int ConnectableViewMap::GetSize() const
{
  return static_cast<int>(m_item_to_view.size());
}

}  // namespace oac_tree_gui
