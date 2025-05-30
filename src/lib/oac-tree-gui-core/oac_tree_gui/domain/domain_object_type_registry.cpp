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

#include "domain_object_type_registry.h"

namespace oac_tree_gui
{

void DomainObjectTypeRegistry::Update(const std::string &plugin_name,
                                      const std::vector<std::string> &object_names)
{
  for (const auto &name : object_names)
  {
    auto iter = m_object_name_to_plugin_name.find(name);
    if (iter == m_object_name_to_plugin_name.end())
    {
      (void)m_object_name_to_plugin_name.insert({name, plugin_name});
    }
  }
}

std::optional<std::string> DomainObjectTypeRegistry::GetPluginName(
    const std::string &object_name) const
{
  auto iter = m_object_name_to_plugin_name.find(object_name);
  return iter == m_object_name_to_plugin_name.end() ? std::optional<std::string>() : iter->second;
}

std::vector<std::string> DomainObjectTypeRegistry::GetObjectNames(
    const std::string &plugin_name) const
{
  std::vector<std::string> result;
  for (const auto &element : m_object_name_to_plugin_name)
  {
    if (element.second == plugin_name)
    {
      result.push_back(element.first);
    }
  }
  return result;
}

DomainObjectTypeRegistry &GlobalDomainObjectTypeRegistry()
{
  static DomainObjectTypeRegistry registry;
  return registry;
}

}  // namespace oac_tree_gui
