/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "settings_helper.h"

#include <sequencergui/model/settings_model.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>

#include <QSettings>

namespace sequencergui
{

const SettingsModel &GetGlobalSettings()
{
  static const SettingsModel model;
  return model;
}

void SaveSettingsInPersistentStorage(const SettingsModel &model)
{
  QSettings settings;

  auto xml_string = mvvm::utils::ToXMLString(*model.GetRootItem());
  settings.setValue(kRootSetingsModelName, QString::fromStdString(xml_string));
}

void LoadSettingsFromPersistentStorage(SettingsModel &model)
{
  const QSettings settings;

  // If the QSettings file contains a record, we will fully rebuild the settings model with the
  // content, stored in the file. This is not a very clean approach, since our C++ model
  // might not match what was stored in a file a while ago. TODO find a way to update the model only
  // with meaningful content.
  if (settings.contains(kRootSetingsModelName))
  {
    auto str = settings.value(kRootSetingsModelName).toString().toStdString();
    auto root_item = mvvm::utils::SessionItemFromXMLString(str);
    model.ReplaceRootItem(std::move(root_item));
  }
}

void ReadGlobalSettings()
{
  LoadSettingsFromPersistentStorage(const_cast<SettingsModel &>(GetGlobalSettings()));
}

}  // namespace sequencergui
