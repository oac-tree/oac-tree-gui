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

#ifndef SEQUENCERGUI_CORE_FACTORY_H_
#define SEQUENCERGUI_CORE_FACTORY_H_

#include <algorithm>
#include <map>
#include <stdexcept>
#include <vector>

namespace sequencergui
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

}  // namespace sequencergui

#endif  // SEQUENCERGUI_CORE_FACTORY_H_
