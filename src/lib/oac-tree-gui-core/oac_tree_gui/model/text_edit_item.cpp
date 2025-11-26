/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "text_edit_item.h"

#include "sequencer_settings_constants.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/sequencer_settings_helper.h>

#include <mvvm/utils/string_utils.h>

namespace oac_tree_gui
{

namespace
{
constexpr auto kSeparator = ";";
}

TextEditItem::TextEditItem() : CompoundItem(mvvm::GetTypeName<TextEditItem>())
{
  // properties for plugin folders
  (void)AddProperty(constants::kUseTextEditor, true)
      .SetDisplayName("Use text editor")
      .SetToolTip("Defines whether to enable or disable the text editor");

  (void)AddProperty(constants::kTextEditorContent, std::string())
      .SetDisplayName("Text editor")
      .SetToolTip("String representing the content of the text editor");
}

std::unique_ptr<mvvm::SessionItem> TextEditItem::Clone() const
{
  return std::make_unique<TextEditItem>(*this);
}

bool TextEditItem::IsEditorEnabled() const
{
  return Property<bool>(constants::kUseTextEditor);
}

void TextEditItem::SetEditorEnabled(bool value)
{
  (void)SetProperty(constants::kUseTextEditor, value);
}

std::vector<std::string> TextEditItem::GetText() const
{
  const auto setting_str = Property<std::string>(constants::kTextEditorContent);
  return GetVectorFromSettingString(setting_str);
}

void TextEditItem::SetText(const std::vector<std::string> &lines)
{
  const auto setting_str = GetSettingStringFromVector(lines);
  (void)SetProperty(constants::kTextEditorContent, setting_str);
}

std::string GetSettingStringFromVector(const std::vector<std::string> &vec)
{
  return mvvm::utils::VectorToString(vec, kSeparator);
}

std::vector<std::string> GetVectorFromSettingString(const std::string &str)
{
  return mvvm::utils::StringToVector<std::string>(str, kSeparator);
}

}  // namespace oac_tree_gui
