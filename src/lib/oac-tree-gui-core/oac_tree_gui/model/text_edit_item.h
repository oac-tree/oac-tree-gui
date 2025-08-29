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

#ifndef OAC_TREE_GUI_MODEL_TEXT_EDIT_ITEM_H
#define OAC_TREE_GUI_MODEL_TEXT_EDIT_ITEM_H

#include <mvvm/model/compound_item.h>

namespace oac_tree_gui
{

/**
 * @brief The TextEditItem class holds the data related to the text editing.
 *
 * It is intended for short texts editing (i.e. a list of plugins to load) using the QTextEdit
 * widget. The item has a boolean property to enable/disable the editor, and a string property to
 * store the serialized content of the QTextEdit.
 */
class TextEditItem : public mvvm::CompoundItem
{
public:
  TextEditItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Checks if the editor is enabled.
   */
  bool IsEditorEnabled() const;

  /**
   * @brief Sets whether the editor is enabled.
   */
  void SetEditorEnabled(bool value);

  /**
   * @brief Returns editor content as a list of text lines.
   */
  std::vector<std::string> GetText() const;

  /**
   * @brief Set editor text content from a list of text lines.
   */
  void SetText(const std::vector<std::string>& lines);
};

/**
 * @brief Converts vector of strings to a single string.
 *
 * This string serves as a serialization format for the vector, so that it can be used as item
 * property.
 */
std::string GetSettingStringFromVector(const std::vector<std::string>& vec);

/**
 * @brief Returns vector of strings from the single string.
 *
 * This is the inverse function to GetSettingStringFromVector.
 */
std::vector<std::string> GetVectorFromSettingString(const std::string& str);

}  // namespace oac_tree_gui

namespace mvvm
{
template <>
struct item_traits<oac_tree_gui::TextEditItem>
{
  static constexpr std::string_view type_name() noexcept { return "TextEdit"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_TEXT_EDIT_ITEM_H
