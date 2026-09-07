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

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/sequencer_settings_helper.h>

#include <mvvm/model/make_clone.h>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Returns the text edit item registered under the given tag.
 *
 * @throws RuntimeException if the property with the given tag doesn't exist.
 */
TextEditItem& GetTextEditItem(const mvvm::CompoundItem& parent, const std::string& tag)
{
  auto* result = parent.GetItem<TextEditItem>(tag);
  if (result == nullptr)
  {
    throw RuntimeException("Property '" + tag + "' is not registered");
  }
  return *result;
}

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

PluginSettingsItem::PluginSettingsItem() : CompoundItem(mvvm::GetTypeName<PluginSettingsItem>())
{
  (void)SetDisplayName("Plugin Settings");

  {  // property to hold plugin directory list
    auto& property = AddProperty<TextEditItem>(constants::kPluginDirListProperty);
    const auto dir_list = GetDefaultPluginDirList();
    property.SetText(dir_list);
    property.SetEditorEnabled(!dir_list.empty());
  }

  {  // property to hold plugin name list
    auto& property = AddProperty<TextEditItem>(constants::kPluginListProperty);
    const auto dir_list = GetDefaultPluginList();
    property.SetText(dir_list);
    property.SetEditorEnabled(!dir_list.empty());
  }
}

std::unique_ptr<mvvm::SessionItem> PluginSettingsItem::Clone() const
{
  return mvvm::MakeClone(*this);
}

bool PluginSettingsItem::UsePluginDirList() const
{
  return GetTextEditItem(*this, constants::kPluginDirListProperty).IsEditorEnabled();
}

void PluginSettingsItem::SetUsePluginDirList(bool value)
{
  GetTextEditItem(*this, constants::kPluginDirListProperty).SetEditorEnabled(value);
}

std::vector<std::string> PluginSettingsItem::GetPluginDirList() const
{
  return GetTextEditItem(*this, constants::kPluginDirListProperty).GetText();
}

void PluginSettingsItem::SetPluginDirList(const std::vector<std::string>& dir_list)
{
  GetTextEditItem(*this, constants::kPluginDirListProperty).SetText(dir_list);
}

bool PluginSettingsItem::UsePluginList() const
{
  return GetTextEditItem(*this, constants::kPluginListProperty).IsEditorEnabled();
}

void PluginSettingsItem::SetUsePluginList(bool value)
{
  GetTextEditItem(*this, constants::kPluginListProperty).SetEditorEnabled(value);
}

std::vector<std::string> PluginSettingsItem::GetPluginList() const
{
  return GetTextEditItem(*this, constants::kPluginListProperty).GetText();
}

void PluginSettingsItem::SetPluginList(const std::vector<std::string>& dir_list)
{
  GetTextEditItem(*this, constants::kPluginListProperty).SetText(dir_list);
}

std::vector<std::string> GetPluginFileNames(const PluginSettingsItem& item)
{
  std::vector<std::string> result;
  if (item.UsePluginDirList())
  {
    const auto libs = FindSharedLibrariesInDirList(item.GetPluginDirList());
    (void)result.insert(result.end(), libs.begin(), libs.end());
  }

  if (item.UsePluginList())
  {
    const auto libs = GetSharedLibrariesFromPluginNames(item.GetPluginList());
    (void)result.insert(result.end(), libs.begin(), libs.end());
  }

  return result;
}

}  // namespace oac_tree_gui
