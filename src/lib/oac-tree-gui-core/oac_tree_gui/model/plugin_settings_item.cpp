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

#include "plugin_settings_item.h"

#include "sequencer_settings_constants.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/sequencer_settings_helper.h>

#include <mvvm/utils/string_utils.h>

namespace oac_tree_gui
{

namespace
{
constexpr auto kSeparator = ";";
}

PluginSettingsItem::PluginSettingsItem() : CompoundItem(GetStaticType())
{
  // properties for plugin folders
  (void)AddProperty(constants::kUsePluginDirList, true)
      .SetDisplayName("Use folder list")
      .SetToolTip("Defines whether to use a list of folders to search for plugins");

  auto default_value = GetSettingStringFromVector(GetDefaultPluginDirList());
  (void)AddProperty(constants::kPluginDirList, default_value)
      .SetDisplayName("Plugin folders")
      .SetToolTip("List of folders where to search for plugins");

  // properties for plugin names
  (void)AddProperty(constants::kUsePluginList, false)
      .SetDisplayName("Use plugin list")
      .SetToolTip("Defines whether to use a list of plugin names");

  (void)AddProperty(constants::kPluginList, GetSettingStringFromVector(GetDefaultPluginList()))
      .SetDisplayName("Plugin names")
      .SetToolTip("List of plugin names to load");
}

std::string PluginSettingsItem::GetStaticType()
{
  return "PluginSettings";
}

std::unique_ptr<mvvm::SessionItem> PluginSettingsItem::Clone() const
{
  return std::make_unique<PluginSettingsItem>(*this);
}

bool PluginSettingsItem::UsePluginDirList() const
{
  return Property<bool>(constants::kUsePluginDirList);
}

void PluginSettingsItem::SetUsePluginDirList(bool value)
{
  SetProperty(constants::kUsePluginDirList, value);
}

std::vector<std::string> PluginSettingsItem::GetPluginDirList() const
{
  const auto setting_str = Property<std::string>(constants::kPluginDirList);
  return GetVectorFromSettingString(setting_str);
}

void PluginSettingsItem::SetPluginDirList(const std::vector<std::string> &dir_list)
{
  const auto setting_str = GetSettingStringFromVector(dir_list);
  SetProperty(constants::kPluginDirList, setting_str);
}

bool PluginSettingsItem::UsePluginList() const
{
  return Property<bool>(constants::kUsePluginList);
}

void PluginSettingsItem::SetUsePluginList(bool value)
{
  SetProperty(constants::kUsePluginList, value);
}

std::vector<std::string> PluginSettingsItem::GetPluginList() const
{
  const auto setting_str = Property<std::string>(constants::kPluginList);
  return GetVectorFromSettingString(setting_str);
}

void PluginSettingsItem::SetPluginList(const std::vector<std::string> &dir_list)
{
  const auto setting_str = GetSettingStringFromVector(dir_list);
  SetProperty(constants::kPluginList, setting_str);
}

std::string GetSettingStringFromVector(const std::vector<std::string> &vec)
{
  return mvvm::utils::VectorToString(vec, kSeparator);
}

std::vector<std::string> GetVectorFromSettingString(const std::string &str)
{
  return mvvm::utils::StringToVector<std::string>(str, kSeparator);
}

std::vector<std::string> GetPluginFileNames(const PluginSettingsItem &item)
{
  std::vector<std::string> result;
  if (item.UsePluginDirList())
  {
    for (const auto &dir : item.GetPluginDirList())
    {
      const auto file_names = FindSharedLibraries(dir);
      result.insert(result.end(), file_names.begin(), file_names.end());
    }
  }

  if (item.UsePluginList())
  {
    const auto plugin_names = item.GetPluginList();
    for (const auto &name : plugin_names)
    {
      result.push_back(GetPluginFileName(name));
    }
  }

  return result;
}

}  // namespace oac_tree_gui
