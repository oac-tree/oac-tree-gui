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

#include "in_memory_settings_storage.h"

namespace oac_tree_gui
{

std::optional<mvvm::variant_t> InMemorySettingsStorage::Read(const std::string& key) const
{
  auto iter = m_values.find(key);
  if (iter == m_values.end())
  {
    return {};
  }
  return iter->second;
}

void InMemorySettingsStorage::Write(const std::string& key, const mvvm::variant_t& value)
{
  m_values[key] = value;
}

std::vector<std::string> InMemorySettingsStorage::GetKeys() const
{
  std::vector<std::string> result;
  result.reserve(m_values.size());
  for (const auto& [key, value] : m_values)
  {
    result.push_back(key);
  }
  return result;
}

}  // namespace oac_tree_gui
