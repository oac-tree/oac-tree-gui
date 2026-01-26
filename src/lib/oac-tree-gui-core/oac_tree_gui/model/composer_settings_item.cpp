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

namespace
{
constexpr auto kSeparator = ";";

/**
 * @brief Returns list of ProcedureEditorInfo objects from its string representation.
 */
std::vector<ProcedureEditorInfo> GetProcedureInfoListFromString(const std::string& str)
{
  if (str.empty())
  {
    return {};
  }

  auto parts = mvvm::utils::SplitString(str, kSeparator);
  std::vector<ProcedureEditorInfo> editor_info;
  editor_info.reserve(parts.size());
  for (const auto& item_str : parts)
  {
    editor_info.push_back(GetProcedureInfoFromString(item_str));
  }
  return editor_info;
}

/**
 * @brief Returns string representation of ProcedureEditorInfo list.
 */
std::string GetStringFromProcedureInfoList(const std::vector<ProcedureEditorInfo>& editor_info)
{
  std::vector<std::string> str;
  str.reserve(editor_info.size());
  for (const auto& info : editor_info)
  {
    str.push_back(GetStringFromProcedureInfo(info));
  }
  return mvvm::utils::VectorToString(str, kSeparator);
}

}  // namespace

ComposerSettingsItem::ComposerSettingsItem()
    : CompoundItem(mvvm::GetTypeName<ComposerSettingsItem>())
{
  (void)SetDisplayName("Composer Settings");

  // property carrying a string to represent std::vector<ProcedureEditorInfo>
  (void)AddProperty(kEditorInfoList, std::string());
  // property carrying splitter state
  (void)AddProperty(kSplitterState, std::string());
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
  const auto joined_str = GetStringFromProcedureInfoList(editor_info);
  (void)SetProperty<std::string>(kEditorInfoList, joined_str);
}

ComposerViewInfo ComposerSettingsItem::GetComposerViewInfo() const
{
  ComposerViewInfo info;
  info.splitter_state = Property<std::string>(kSplitterState);
  info.editor_info_list = GetProcedureEditorInfoList();
  return info;
}

void ComposerSettingsItem::SetComposerViewInfo(const ComposerViewInfo& view_info)
{
  SetProcedureEditorInfoList(view_info.editor_info_list);
  (void)SetProperty<std::string>(kSplitterState, view_info.splitter_state);
}

}  // namespace oac_tree_gui
