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
#include "text_edit_item.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/sequencer_settings_helper.h>

#include <mvvm/utils/string_utils.h>

namespace oac_tree_gui
{

PluginSettingsItem::PluginSettingsItem() : CompoundItem(GetStaticType())
{
  SetDisplayName("Plugin Settings");

  { // property to hold plugin directory list
    auto &property = AddProperty<TextEditItem>(constants::kPluginDirListProperty);
    const auto dir_list = GetDefaultPluginDirList();
    property.SetText(dir_list);
    property.SetEditorEnabled(!dir_list.empty());
  }

  { // property to hold plugin name list
    auto &property = AddProperty<TextEditItem>(constants::kPluginListProperty);
    const auto dir_list = GetDefaultPluginList();
    property.SetText(dir_list);
    property.SetEditorEnabled(!dir_list.empty());
  }
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
  return GetItem<TextEditItem>(constants::kPluginDirListProperty)->IsEditorEnabled();
}

void PluginSettingsItem::SetUsePluginDirList(bool value)
{
  GetItem<TextEditItem>(constants::kPluginDirListProperty)->SetEditorEnabled(value);
}

std::vector<std::string> PluginSettingsItem::GetPluginDirList() const
{
  return GetItem<TextEditItem>(constants::kPluginDirListProperty)->GetText();
}

void PluginSettingsItem::SetPluginDirList(const std::vector<std::string> &dir_list)
{
  GetItem<TextEditItem>(constants::kPluginDirListProperty)->SetText(dir_list);
}

bool PluginSettingsItem::UsePluginList() const
{
  return GetItem<TextEditItem>(constants::kPluginListProperty)->IsEditorEnabled();
}

void PluginSettingsItem::SetUsePluginList(bool value)
{
  GetItem<TextEditItem>(constants::kPluginListProperty)->SetEditorEnabled(value);
}

std::vector<std::string> PluginSettingsItem::GetPluginList() const
{
  return GetItem<TextEditItem>(constants::kPluginListProperty)->GetText();
}

void PluginSettingsItem::SetPluginList(const std::vector<std::string> &dir_list)
{
  GetItem<TextEditItem>(constants::kPluginListProperty)->SetText(dir_list);
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
