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

#include "settings_item.h"

#include "settings_constants.h"

#include <mvvm/model/item_factory.h>
#include <mvvm/signals/item_connect.h>

namespace sequencergui
{

namespace
{

bool RegisterItems()
{
  (void)mvvm::RegisterGlobalItem<CommonSettingsItem>();
  return true;
}

const bool settings_item_registered_flag = RegisterItems();

}  // namespace

CommonSettingsItem::CommonSettingsItem() : CompoundItem(Type)
{
  SetDisplayName("Common Settings");

  AddProperty(kUseUndoSetting, kUseUndoDefault).SetDisplayName("Enable undo/redo");
  AddProperty(kUndoLimitSetting, kUndoLimitDefault).SetDisplayName("Undo limit");
}

std::unique_ptr<mvvm::SessionItem> CommonSettingsItem::Clone() const
{
  return std::make_unique<CommonSettingsItem>(*this);
}

void CommonSettingsItem::Activate()
{
  // Enable/disable property "Undo limit" when property "Enable undo/redo" changes
  auto on_property_changed = [this](const mvvm::DataChangedEvent&)
  { GetItem(kUndoLimitSetting)->SetEnabled(Property<bool>(kUseUndoSetting)); };

  mvvm::connect::Connect<mvvm::DataChangedEvent>(
      /*source*/ GetItem(kUseUndoSetting), on_property_changed, GetSlot());
}

}  // namespace sequencergui
