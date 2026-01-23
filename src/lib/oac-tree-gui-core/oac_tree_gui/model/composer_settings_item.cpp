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

#include <mvvm/utils/string_utils.h>

namespace oac_tree_gui
{

ComposerSettingsItem::ComposerSettingsItem()
    : CompoundItem(mvvm::GetTypeName<ComposerSettingsItem>())
{
  (void)SetDisplayName("Composer Settings");

  // property carrying a string to represent std::vector<ProcedureEditorInfo>
  (void)AddProperty(kEditorInfoList, std::string());
}

std::unique_ptr<mvvm::SessionItem> ComposerSettingsItem::Clone() const
{
  return std::make_unique<ComposerSettingsItem>(*this);
}

std::vector<ProcedureEditorInfo> ComposerSettingsItem::GetProcedureEditorInfoList() const
{
  const auto str = Property<std::string>(kEditorInfoList);
  return GetProcedureInfoListFromString(str);
}

void ComposerSettingsItem::SetProcedureEditorInfoList(
    const std::vector<ProcedureEditorInfo>& editor_info)
{
  const auto str = GetStringFromProcedureInfoList(editor_info);
  SetProperty<std::string>(kEditorInfoList, str);
}

}  // namespace oac_tree_gui
