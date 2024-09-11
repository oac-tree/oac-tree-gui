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

#include "settings_item.h"

#include "settings_constants.h"

#include <mvvm/model/item_factory.h>

namespace sequencergui
{

namespace
{

bool RegisterItems()
{
  mvvm::RegisterGlobalItem<CommonSettingsItem>();
  return true;
}

static const bool settings_item_registered_flag = RegisterItems();

}  // namespace

CommonSettingsItem::CommonSettingsItem() : CompoundItem(Type)
{
  SetDisplayName("Common Settings");

  AddProperty(kUseUndoSetting, kUseUndoDefault).SetDisplayName("Enable undo/redo");
  AddProperty(kUndoLimitSetting, kUndoLimitDefault).SetDisplayName("Undo limit");
}

std::unique_ptr<mvvm::SessionItem> CommonSettingsItem::Clone(bool make_unique_id) const
{
  return std::make_unique<CommonSettingsItem>(*this, make_unique_id);
}

}  // namespace sequencergui
