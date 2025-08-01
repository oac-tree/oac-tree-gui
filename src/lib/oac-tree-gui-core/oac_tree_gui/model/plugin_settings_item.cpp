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

#include "sequencer_settings_model.h"

namespace oac_tree_gui
{

PluginSettingsItem::PluginSettingsItem() : CompoundItem(GetStaticType())
{
  AddProperty(constants::kPluginList, std::string());
}

std::string PluginSettingsItem::GetStaticType()
{
  return "PluginSettings";
}

std::unique_ptr<mvvm::SessionItem> PluginSettingsItem::Clone() const
{
  return std::make_unique<PluginSettingsItem>(*this);
}

}  // namespace oac_tree_gui
