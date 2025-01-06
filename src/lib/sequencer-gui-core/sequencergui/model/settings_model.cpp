/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "settings_model.h"

#include "settings_item.h"

#include <mvvm/model/model_utils.h>

namespace sequencergui
{

SettingsModel::SettingsModel() : ApplicationModel("SettingsModel")
{
  PopulateModel();
}

void SettingsModel::Clear()
{
  mvvm::ApplicationModel::Clear();
  PopulateModel();
}

std::vector<mvvm::CompoundItem*> SettingsModel::GetSettingsItems() const
{
  return mvvm::utils::GetTopItems<mvvm::CompoundItem>(this);
}

void SettingsModel::PopulateModel()
{
  InsertItem<CommonSettingsItem>();
}

}  // namespace sequencergui
