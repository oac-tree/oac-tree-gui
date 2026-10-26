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

#include "settings_model_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/plugin_settings_item.h>
#include <oac_tree_gui/model/sequencer_settings_model.h>

#include <sup/gui/model/undo_settings_item.h>
#include <sup/gui/settings/i_settings_manager.h>

namespace oac_tree_gui
{

namespace
{

//! Returns the settings item of the given type, throws if the model has none.
template <typename T>
T& GetSettingsItem(const SequencerSettingsModel& model)
{
  auto* result = model.Get<T>();
  if (result == nullptr)
  {
    throw RuntimeException("Settings model has no item of the requested type");
  }
  return *result;
}

}  // namespace

std::unique_ptr<SequencerSettingsModel> CreateSettingsModel(
    const sup::gui::ISettingsManager& manager)
{
  auto result = std::make_unique<SequencerSettingsModel>();

  GetSettingsItem<sup::gui::UndoSettingsItem>(*result).SetUndoInfo(manager.Get<sup::gui::UndoInfo>(
      sup::gui::kUndoSettingsGroup, sup::gui::SettingsScope::kApplication));

  GetSettingsItem<PluginSettingsItem>(*result).SetPluginSettingsInfo(
      manager.Get<PluginSettingsInfo>(kPluginSettingsGroup, sup::gui::SettingsScope::kApplication,
                                      CreateDefaultPluginSettingsInfo()));

  return result;
}

void WriteSettingsModel(const SequencerSettingsModel& model, sup::gui::ISettingsManager& manager)
{
  manager.Set(sup::gui::kUndoSettingsGroup,
              GetSettingsItem<sup::gui::UndoSettingsItem>(model).GetUndoInfo(),
              sup::gui::SettingsScope::kApplication);

  manager.Set(kPluginSettingsGroup,
              GetSettingsItem<PluginSettingsItem>(model).GetPluginSettingsInfo(),
              sup::gui::SettingsScope::kApplication);
}

}  // namespace oac_tree_gui
