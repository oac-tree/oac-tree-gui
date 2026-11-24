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

#include "sub_tree_history.h"

#include <oac_tree_gui/core/exceptions.h>

namespace oac_tree_gui
{

void SubTreeHistory::Clear()
{
  m_entries.clear();
  m_current_index = 0;
}

void SubTreeHistory::Reset(const SubTreeHistoryEntry& entry)
{
  m_entries = {entry};
  m_current_index = 0;
}

void SubTreeHistory::Push(const SubTreeHistoryEntry& entry)
{
  if (!m_entries.empty())
  {
    m_entries.resize(m_current_index + 1);
  }
  m_entries.push_back(entry);
  m_current_index = m_entries.size() - 1;
}

const std::vector<SubTreeHistoryEntry>& SubTreeHistory::GetEntries() const&
{
  return m_entries;
}

std::size_t SubTreeHistory::GetCurrentIndex() const
{
  return m_current_index;
}

void SubTreeHistory::SetCurrentIndex(std::size_t index)
{
  if (index >= m_entries.size())
  {
    throw RuntimeException("History index is out of range");
  }
  m_current_index = index;
}

std::optional<std::size_t> SubTreeHistory::FindPrevious(const predicate_t& predicate) const
{
  for (auto index = m_current_index; index > 0; --index)
  {
    if (predicate(m_entries[index - 1]))
    {
      return index - 1;
    }
  }
  return std::nullopt;
}

std::optional<std::size_t> SubTreeHistory::FindNext(const predicate_t& predicate) const
{
  const std::size_t entry_count = m_entries.size();
  for (auto index = m_current_index + 1; index < entry_count; ++index)
  {
    if (predicate(m_entries[index]))
    {
      return index;
    }
  }
  return std::nullopt;
}

std::size_t SubTreeHistory::GetTrailStart() const
{
  for (auto index = m_current_index; index > 0; --index)
  {
    if (m_entries[index].include_identifier.empty())
    {
      return index;
    }
  }
  return 0;
}

}  // namespace oac_tree_gui
