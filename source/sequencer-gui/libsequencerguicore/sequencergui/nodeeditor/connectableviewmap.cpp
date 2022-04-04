/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/nodeeditor/connectableviewmap.h"

#include "sequencergui/core/exceptions.h"

namespace sequencergui
{

ConnectableViewMap::ConnectableViewMap() = default;

void ConnectableViewMap::Insert(const InstructionItem *item, ConnectableView *view_item)
{
  auto it = m_item_to_view.find(item);
  if (it != m_item_to_view.end())
  {
    throw ExistingKeyException("Connectable view already exists for given instruction");
  }
  m_item_to_view.insert(it, {item, view_item});
}

void ConnectableViewMap::InsertOrAssign(const InstructionItem *item, ConnectableView *view_item)
{
  m_item_to_view.insert_or_assign(item, view_item);
}

ConnectableView *ConnectableViewMap::FindView(const InstructionItem *item)
{
  auto it = m_item_to_view.find(item);
  return it == m_item_to_view.end() ? nullptr : it->second;
}

//! Removes view from map corresponding to given instruction.

void ConnectableViewMap::Remove(const InstructionItem *item)
{
  auto it = m_item_to_view.find(item);
  if (it != m_item_to_view.end())
  {
    m_item_to_view.erase(it);
  }
  else
  {
    throw NotFoundKeyException("No connectable view found for given instruction item");
  }
}

//! Removes view from the map and return it to the user
ConnectableView *ConnectableViewMap::TakeView(const InstructionItem *item)
{
  auto it = m_item_to_view.find(item);
  if (it != m_item_to_view.end())
  {
    m_item_to_view.erase(it);
  }
  else
  {
    throw NotFoundKeyException("No connectable view found for given instruction item");
  }

  return it->second;
}

void ConnectableViewMap::Clear()
{
  m_item_to_view.clear();
}

}  // namespace sequencergui
