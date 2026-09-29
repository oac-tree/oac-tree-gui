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

#include "settings_manager.h"

#include <utility>
#include <vector>

namespace oac_tree_gui
{

namespace
{

//! Returns ids of entries of the given scope.
template <typename MapT>
std::vector<std::uint64_t> FindIds(const MapT& entries, SettingsScope scope)
{
  std::vector<std::uint64_t> result;
  for (const auto& [id, entry] : entries)
  {
    if (entry.scope == scope)
    {
      result.push_back(id);
    }
  }
  return result;
}

//! Returns ids of entries of the given scope and group.
template <typename MapT>
std::vector<std::uint64_t> FindIds(const MapT& entries, SettingsScope scope,
                                   const std::string& group)
{
  std::vector<std::uint64_t> result;
  for (const auto& [id, entry] : entries)
  {
    if (entry.scope == scope && entry.group == group)
    {
      result.push_back(id);
    }
  }
  return result;
}

//! Calls entries with the given ids, skipping entries removed during the loop.
template <typename MapT, typename StorageT>
void CallEntries(const MapT& entries, const std::vector<std::uint64_t>& ids, StorageT& storage)
{
  for (auto id : ids)
  {
    auto iter = entries.find(id);
    if (iter == entries.end())
    {
      continue;
    }
    // copy keeps the callback alive if it destroys its own ScopedCallback
    auto entry = iter->second;
    entry.func(storage, entry.group);
  }
}

}  // namespace

void SettingsManager::SetStorage(SettingsScope scope, ISettingsStorage* storage)
{
  m_storages[scope] = storage;
}

ISettingsStorage* SettingsManager::GetStorage(SettingsScope scope) const
{
  auto iter = m_storages.find(scope);
  return iter == m_storages.end() ? nullptr : iter->second;
}

ScopedCallback SettingsManager::AddSubscriber(const std::string& group, SettingsScope scope,
                                              settings_reader_t reader)
{
  const auto id = m_next_id++;
  (void)m_subscribers.emplace(id, Entry<settings_reader_t>{group, scope, std::move(reader)});
  return ScopedCallback([this, id]() { (void)m_subscribers.erase(id); });
}

ScopedCallback SettingsManager::AddProvider(const std::string& group, SettingsScope scope,
                                            settings_writer_t writer)
{
  const auto id = m_next_id++;
  (void)m_providers.emplace(id, Entry<settings_writer_t>{group, scope, std::move(writer)});
  return ScopedCallback([this, id]() { (void)m_providers.erase(id); });
}

void SettingsManager::NotifyGroup(const std::string& group, SettingsScope scope)
{
  if (auto storage = GetStorage(scope); storage)
  {
    CallEntries(m_subscribers, FindIds(m_subscribers, scope, group), std::as_const(*storage));
  }
}

void SettingsManager::Collect(SettingsScope scope)
{
  if (auto storage = GetStorage(scope); storage)
  {
    CallEntries(m_providers, FindIds(m_providers, scope), *storage);
  }
}

void SettingsManager::NotifyAll(SettingsScope scope)
{
  if (auto storage = GetStorage(scope); storage)
  {
    CallEntries(m_subscribers, FindIds(m_subscribers, scope), std::as_const(*storage));
  }
}

}  // namespace oac_tree_gui
