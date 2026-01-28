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

#ifndef OAC_TREE_GUI_DOMAIN_I_DOMAIN_PLUGIN_SERVICE_H_
#define OAC_TREE_GUI_DOMAIN_I_DOMAIN_PLUGIN_SERVICE_H_

#include <string>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief The IDomainPluginService class is an interface to load domain plugins and keep track of
 * what was loaded.
 */
class IDomainPluginService
{
public:
  IDomainPluginService() = default;
  virtual ~IDomainPluginService() = default;

  IDomainPluginService(const IDomainPluginService&) = delete;
  IDomainPluginService& operator=(const IDomainPluginService&) = delete;
  IDomainPluginService(IDomainPluginService&&) = delete;
  IDomainPluginService& operator=(IDomainPluginService&&) = delete;

  /**
   * @brief Loads plugins by their file names.
   *
   * Plugin file names can be absolute or relative paths.
   *
   * @param plugin_file_names List of plugin file names to load.
   */
  virtual void LoadPluginFiles(const std::vector<std::string>& plugin_file_names) = 0;

  /**
   * @brief Returns the list of all plugins we've tried to load, and their load success status.
   */
  virtual std::vector<std::pair<std::string, bool>> GetPluginLoadInfo() const = 0;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_DOMAIN_I_DOMAIN_PLUGIN_SERVICE_H_
