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

#include <oac_tree_gui/model/plugin_settings_item.h>
#include <oac_tree_gui/model/sequencer_settings_model.h>
#include <oac_tree_gui/model/undo_settings_item.h>
#include <oac_tree_gui/settings/i_settings_manager.h>

namespace oac_tree_gui
{

std::unique_ptr<SequencerSettingsModel> CreateSettingsModel(const ISettingsManager& manager)
{
  auto result = std::make_unique<SequencerSettingsModel>();

  result->Get<UndoSettingsItem>()->SetUndoInfo(
      manager.Get<UndoInfo>(kUndoSettingsGroup, SettingsScope::kApplication));

  result->Get<PluginSettingsItem>()->SetPluginSettingsInfo(manager.Get<PluginSettingsInfo>(
      kPluginSettingsGroup, SettingsScope::kApplication, CreateDefaultPluginSettingsInfo()));

  return result;
}

void WriteSettingsModel(const SequencerSettingsModel& model, ISettingsManager& manager)
{
  manager.Set(kUndoSettingsGroup, model.Get<UndoSettingsItem>()->GetUndoInfo(),
              SettingsScope::kApplication);

  manager.Set(kPluginSettingsGroup, model.Get<PluginSettingsItem>()->GetPluginSettingsInfo(),
              SettingsScope::kApplication);
}

}  // namespace oac_tree_gui
