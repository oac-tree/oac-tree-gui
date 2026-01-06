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

#ifndef OAC_TREE_GUI_MODEL_PLUGIN_SETTINGS_ITEM_H_
#define OAC_TREE_GUI_MODEL_PLUGIN_SETTINGS_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace oac_tree_gui
{

/**
 * @brief The PluginSettingsItem class holds settings related to plugin load.
 */
class PluginSettingsItem : public mvvm::CompoundItem
{
public:
  PluginSettingsItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Checks if settings requires to load plugins from the list with directories.
   */
  bool UsePluginDirList() const;

  /**
   * @brief Sets whether to take into account the list of directories while loading plugins.
   */
  void SetUsePluginDirList(bool value);

  /**
   * @brief Returns the list of directories where plugins can be found.
   */
  std::vector<std::string> GetPluginDirList() const;

  /**
   * @brief Sets the list of directories where plugins can be found.
   */
  void SetPluginDirList(const std::vector<std::string>& dir_list);

  /**
   * @brief Checks if settings requires to load plugins using list of plugin names.
   */
  bool UsePluginList() const;

  /**
   * @brief Sets whether to use plugin list store on board while loading plugins.
   */
  void SetUsePluginList(bool value);

  /**
   * @brief Returns the list of plugin names.
   *
   * Plugin names can be given in mixed format:
   * 1) The plugin name can have no prefix "lib" and no suffixes ".so" or ".dylib". In this case the
   * necessary previx/suffix will be added.
   * 2) The plugin name can be the absolute path to the plugin file.
   */
  std::vector<std::string> GetPluginList() const;

  /**
   * @brief Sets the list of plugin names.
   */
  void SetPluginList(const std::vector<std::string>& dir_list);
};

/**
 * @brief Returns the list of plugin file names found in the directories specified in the item.
 */
std::vector<std::string> GetPluginFileNames(const PluginSettingsItem& item);

}  // namespace oac_tree_gui

namespace mvvm
{

template <>
struct item_traits<oac_tree_gui::PluginSettingsItem>
{
  static constexpr std::string_view type_name() noexcept { return "PluginSettings"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_PLUGIN_SETTINGS_ITEM_H_
