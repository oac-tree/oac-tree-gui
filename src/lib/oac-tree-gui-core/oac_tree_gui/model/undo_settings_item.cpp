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

#include "undo_settings_item.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/model/make_clone.h>

namespace oac_tree_gui
{

UndoSettingsItem::UndoSettingsItem() : CompoundItem(mvvm::GetTypeName<UndoSettingsItem>())
{
  (void)SetDisplayName("Undo Settings");
  (void)AddProperty(constants::kUseUndoSetting, constants::kUseUndoDefault)
      .SetDisplayName("Enable undo/redo");
  (void)AddProperty(constants::kUndoLimitSetting, constants::kUndoLimitDefault)
      .SetDisplayName("Undo limit");
  ActivateStrategiesImpl();
}

std::unique_ptr<mvvm::SessionItem> UndoSettingsItem::Clone() const
{
  auto result = mvvm::MakeClone(*this);
  result->ActivateStrategies();
  return result;
}

UndoInfo UndoSettingsItem::GetUndoInfo() const
{
  return {Property<bool>(constants::kUseUndoSetting),
          Property<mvvm::int32>(constants::kUndoLimitSetting)};
}

void UndoSettingsItem::SetUndoInfo(const UndoInfo& info)
{
  (void)SetProperty(constants::kUseUndoSetting, info.is_enabled);
  (void)SetProperty(constants::kUndoLimitSetting, info.undo_limit);
}

void UndoSettingsItem::ActivateStrategies()
{
  ActivateStrategiesImpl();
}

void UndoSettingsItem::ActivateStrategiesImpl()
{
  GetItem(constants::kUseUndoSetting).SetStrategy(this, &UndoSettingsItem::OnSetFlag);
}

bool UndoSettingsItem::OnSetFlag(SessionItem* property, const mvvm::variant_t& variant,
                                 mvvm::role_t role)
{
  mvvm::utils::BeginMacro(*this, "SetUndoFlag");
  const bool result = mvvm::utils::SetData(*property, variant, role);
  (void)GetItem(constants::kUndoLimitSetting).SetEnabled(std::get<bool>(variant));
  mvvm::utils::EndMacro(*this);
  return result;
}

}  // namespace oac_tree_gui
