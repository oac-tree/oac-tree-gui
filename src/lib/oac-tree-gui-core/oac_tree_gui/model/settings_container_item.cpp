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

#include "settings_container_item.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/model/make_clone.h>

namespace oac_tree_gui
{

SettingsContainerItem::SettingsContainerItem()
    : CompoundItem(mvvm::GetTypeName<SettingsContainerItem>())
{
  (void)SetDisplayName("Settings");
}

std::unique_ptr<mvvm::SessionItem> SettingsContainerItem::Clone() const
{
  return mvvm::MakeClone(*this);
}

std::optional<mvvm::variant_t> SettingsContainerItem::GetValue(const std::string& key) const
{
  if (!mvvm::utils::HasTag(*this, key))
  {
    return {};
  }
  return GetItem(key).Data();
}

void SettingsContainerItem::SetValue(const std::string& key, const mvvm::variant_t& value)
{
  if (!mvvm::utils::HasTag(*this, key))
  {
    (void)AddProperty(key, value);
    return;
  }

  auto& property = GetItem(key);
  if (property.Data().index() == value.index())
  {
    (void)property.SetData(value);
  }
  else
  {
    (void)mvvm::utils::ReplaceData(property, value, mvvm::DataRole::kData);
  }
}

}  // namespace oac_tree_gui
