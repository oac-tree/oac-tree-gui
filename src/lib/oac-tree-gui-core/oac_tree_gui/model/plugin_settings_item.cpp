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

#include "plugin_settings_item.h"

#include "sequencer_settings_constants.h"
#include "text_edit_item.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/sequencer_settings_helper.h>

#include <mvvm/model/make_clone.h>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Returns the list of shared libraries found in the list of given folders.
 */
std::vector<std::string> FindSharedLibrariesInDirList(const std::vector<std::string>& dir_list)
{
  std::vector<std::string> result;
  for (const auto& dir : dir_list)
  {
    const auto file_names = FindSharedLibraries(dir);
    (void)result.insert(result.end(), file_names.begin(), file_names.end());
  }
  return result;
}

/**
 * @brief Creates the shared library file name from the plugin name.
 *
 * If plugin name contains a path already, will return as it is.
 */
std::vector<std::string> GetSharedLibrariesFromPluginNames(
    const std::vector<std::string>& plugin_names)
{
  std::vector<std::string> result;
  result.reserve(plugin_names.size());
  for (const auto& name : plugin_names)
  {
    result.push_back(GetPluginFileName(name));
  }
  return result;
}

}  // namespace

bool operator==(const PluginSettingsInfo& lhs, const PluginSettingsInfo& rhs)
{
  return (lhs.use_dir_list == rhs.use_dir_list) && (lhs.dir_list == rhs.dir_list)
         && (lhs.use_plugin_list == rhs.use_plugin_list) && (lhs.plugin_list == rhs.plugin_list);
}

bool operator!=(const PluginSettingsInfo& lhs, const PluginSettingsInfo& rhs)
{
  return !(lhs == rhs);
}

PluginSettingsInfo CreateDefaultPluginSettingsInfo()
{
  PluginSettingsInfo result;
  result.dir_list = GetDefaultPluginDirList();
  result.use_dir_list = !result.dir_list.empty();
  result.plugin_list = GetDefaultPluginList();
  result.use_plugin_list = !result.plugin_list.empty();
  return result;
}

PluginSettingsItem::PluginSettingsItem() : CompoundItem(mvvm::GetTypeName<PluginSettingsItem>())
{
  (void)SetDisplayName("Plugin Settings");
  (void)AddProperty<TextEditItem>(constants::kPluginDirListProperty);
  (void)AddProperty<TextEditItem>(constants::kPluginListProperty);
  SetPluginSettingsInfo(CreateDefaultPluginSettingsInfo());
}

std::unique_ptr<mvvm::SessionItem> PluginSettingsItem::Clone() const
{
  return mvvm::MakeClone(*this);
}

PluginSettingsInfo PluginSettingsItem::GetPluginSettingsInfo() const
{
  const auto& dir_list = GetItem<TextEditItem>(constants::kPluginDirListProperty);
  const auto& plugin_list = GetItem<TextEditItem>(constants::kPluginListProperty);
  return {dir_list.IsEditorEnabled(), dir_list.GetText(), plugin_list.IsEditorEnabled(),
          plugin_list.GetText()};
}

void PluginSettingsItem::SetPluginSettingsInfo(const PluginSettingsInfo& info)
{
  auto& dir_list = GetItem<TextEditItem>(constants::kPluginDirListProperty);
  dir_list.SetEditorEnabled(info.use_dir_list);
  dir_list.SetText(info.dir_list);

  auto& plugin_list = GetItem<TextEditItem>(constants::kPluginListProperty);
  plugin_list.SetEditorEnabled(info.use_plugin_list);
  plugin_list.SetText(info.plugin_list);
}

std::vector<std::string> GetPluginFileNames(const PluginSettingsInfo& info)
{
  std::vector<std::string> result;
  if (info.use_dir_list)
  {
    const auto libs = FindSharedLibrariesInDirList(info.dir_list);
    (void)result.insert(result.end(), libs.begin(), libs.end());
  }

  if (info.use_plugin_list)
  {
    const auto libs = GetSharedLibrariesFromPluginNames(info.plugin_list);
    (void)result.insert(result.end(), libs.begin(), libs.end());
  }

  return result;
}

}  // namespace oac_tree_gui
