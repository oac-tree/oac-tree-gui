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

#ifndef OAC_TREE_GUI_CORE_FACTORY_H_
#define OAC_TREE_GUI_CORE_FACTORY_H_

#include <algorithm>
#include <map>
#include <stdexcept>
#include <vector>

namespace oac_tree_gui
{

template <typename Key, typename Value>
class Factory
{
public:
  std::vector<Key> GetKeys() const
  {
    std::vector<Key> result;
    std::transform(m_object_map.begin(), m_object_map.end(), std::back_inserter(result),
                   [](auto it) { return it.first; });
    return result;
  }

  bool Contains(const Key& key)
  {
    auto it = m_object_map.find(key);
    return it != m_object_map.end();
  }

  void Register(const Key& key, const Value& value)
  {
    auto it = m_object_map.find(key);
    if (it != m_object_map.end())
    {
      throw std::runtime_error("Duplicated key");
    }
    m_object_map.insert(it, {key, value});
  }

  Value GetValue(const Key& key)
  {
    auto it = m_object_map.find(key);
    if (it == m_object_map.end())
    {
      throw std::runtime_error("No such key");
    }
    return it->second;
  }

private:
  std::map<Key, Value> m_object_map;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_CORE_FACTORY_H_
