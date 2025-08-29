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

#ifndef OAC_TREE_GUI_DOMAIN_I_DOMAIN_PLUGIN_SERVICE_H_
#define OAC_TREE_GUI_DOMAIN_I_DOMAIN_PLUGIN_SERVICE_H_

#include <optional>
#include <string>
#include <vector>

namespace oac_tree_gui
{

class IDomainPluginService
{
public:
  IDomainPluginService() = default;
  virtual ~IDomainPluginService() = default;

  IDomainPluginService(const IDomainPluginService&) = delete;
  IDomainPluginService& operator=(const IDomainPluginService&) = delete;
  IDomainPluginService(IDomainPluginService&&) noexcept = default;
  IDomainPluginService& operator=(IDomainPluginService&&) noexcept = default;

  /**
   * @brief Loads plugins by their file names.
   */
  virtual void LoadPluginFiles(const std::vector<std::string>& plugin_file_names) = 0;

  /**
   * @brief Returns names of successfully loaded plugins.
   */
  virtual std::vector<std::string> GetLoadedPlugins() const = 0;

  /**
   * @brief Returns the list of all plugins we've tried to load, and their load success status.
   */
  virtual std::vector<std::pair<std::string, bool>> GetPluginLoadInfo() const = 0;

  /**
   * @brief Returns objects names registered in the given plugin name.
   */
  virtual std::vector<std::string> GetObjectNames(const std::string& plugin_name) const = 0;

  /**
   * @brief Returns the name of the plugin where the given object name is registered.
   *
   * If object name is not known, will return an empty optional.
   */
  virtual std::optional<std::string> GetPluginName(const std::string& object_name) const = 0;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_DOMAIN_I_DOMAIN_PLUGIN_SERVICE_H_
