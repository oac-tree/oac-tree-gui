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

#include <string>
#include <vector>

namespace oac_tree_gui
{

//! Settings group of plugin settings.
constexpr auto kPluginSettingsGroup = "Settings/Plugins";

/**
 * @brief The PluginSettingsInfo struct holds settings of plugin load.
 */
struct PluginSettingsInfo
{
  bool use_dir_list{false};              //! load plugins from directories
  std::vector<std::string> dir_list;     //! directories with plugins
  bool use_plugin_list{false};           //! load plugins from the list of names
  std::vector<std::string> plugin_list;  //! plugin names or file names
};

template <typename Archive>
void Visit(Archive& archive, PluginSettingsInfo& info)
{
  archive("use_dir_list", info.use_dir_list);
  archive("dir_list", info.dir_list);
  archive("use_plugin_list", info.use_plugin_list);
  archive("plugin_list", info.plugin_list);
}

bool operator==(const PluginSettingsInfo& lhs, const PluginSettingsInfo& rhs);
bool operator!=(const PluginSettingsInfo& lhs, const PluginSettingsInfo& rhs);

/**
 * @brief Creates plugin settings with defaults of the current platform.
 *
 * On CODAC, plugins are loaded from the CODAC plugin directory. Otherwise, from the default list
 * of plugin names.
 */
PluginSettingsInfo CreateDefaultPluginSettingsInfo();

/**
 * @brief The PluginSettingsItem class holds settings related to plugin load.
 */
class PluginSettingsItem : public mvvm::CompoundItem
{
public:
  PluginSettingsItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Returns plugin settings.
   */
  PluginSettingsInfo GetPluginSettingsInfo() const;

  /**
   * @brief Sets plugin settings.
   */
  void SetPluginSettingsInfo(const PluginSettingsInfo& info);
};

/**
 * @brief Returns the list of plugin file names to load.
 *
 * Contains shared libraries found in the directories and file names of listed plugins, if enabled.
 */
std::vector<std::string> GetPluginFileNames(const PluginSettingsInfo& info);

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
