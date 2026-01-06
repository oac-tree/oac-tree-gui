/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "sequencer_settings_model.h"

#include "plugin_settings_item.h"

#include <sup/gui/model/settings_item.h>

#include <mvvm/model/model_utils.h>

namespace oac_tree_gui
{

SequencerSettingsModel::SequencerSettingsModel() : ApplicationModel("SequencerSettingsModel")
{
  PopulateModel();
}

std::unique_ptr<mvvm::ISessionModel> SequencerSettingsModel::Clone() const
{
  return std::make_unique<SequencerSettingsModel>(*this);
}

void SequencerSettingsModel::Clear()
{
  mvvm::ApplicationModel::Clear();
  PopulateModel();
}

std::vector<mvvm::CompoundItem*> SequencerSettingsModel::GetSettingsItems() const
{
  return mvvm::utils::GetTopItems<mvvm::CompoundItem>(*this);
}

void SequencerSettingsModel::PopulateModel()
{
  (void)InsertItem<sup::gui::CommonSettingsItem>();
  (void)InsertItem<PluginSettingsItem>();
}

}  // namespace oac_tree_gui
