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

#include "composer_settings_item.h"

namespace oac_tree_gui
{

ComposerSettingsItem::ComposerSettingsItem()
    : CompoundItem(mvvm::GetTypeName<ComposerSettingsItem>())
{
  (void)SetDisplayName("Composer Settings");
}

std::unique_ptr<mvvm::SessionItem> ComposerSettingsItem::Clone() const
{
  return std::make_unique<ComposerSettingsItem>(*this);
}

}  // namespace oac_tree_gui
