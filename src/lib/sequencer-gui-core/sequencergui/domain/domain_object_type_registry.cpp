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

#include "domain_object_type_registry.h"

namespace sequencergui
{

void DomainObjectTypeRegistry::Update(const std::string &plugin_name,
                                      const std::vector<std::string> &object_names)
{
  for (const auto &name : object_names)
  {
    auto iter = m_object_name_to_plugin_name.find(name);
    if (iter == m_object_name_to_plugin_name.end())
    {
      m_object_name_to_plugin_name.insert({name, plugin_name});
    }
  }
}

std::optional<std::string> DomainObjectTypeRegistry::GetPluginName(
    const std::string &object_name) const
{
  auto iter = m_object_name_to_plugin_name.find(object_name);
  return iter == m_object_name_to_plugin_name.end() ? std::optional<std::string>() : iter->second;
}

DomainObjectTypeRegistry &GlobalDomainObjectTypeRegistry()
{
  static DomainObjectTypeRegistry registry;
  return registry;
}

}  // namespace sequencergui
