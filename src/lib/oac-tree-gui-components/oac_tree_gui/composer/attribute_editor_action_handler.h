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

#ifndef OAC_TREE_GUI_COMPOSER_ATTRIBUTE_EDITOR_ACTION_HANDLER_H_
#define OAC_TREE_GUI_COMPOSER_ATTRIBUTE_EDITOR_ACTION_HANDLER_H_

#include <oac_tree_gui/composer/attribute_editor_context.h>

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace oac_tree_gui
{

class AttributeItem;

/**
 * @brief The AttributeEditorActionHandler class implements the logic to handle actions related to
 * instruction/variable attribute editing.
 *
 * Actions itself are defined on the board of AttributeEditorActions. The handler uses a callback to
 * deduce which attribute is currently selected.
 */
class AttributeEditorActionHandler
{
public:
  explicit AttributeEditorActionHandler(AttributeEditorContext context);
  ~AttributeEditorActionHandler();

  AttributeEditorActionHandler(const AttributeEditorActionHandler&) = delete;
  AttributeEditorActionHandler& operator=(const AttributeEditorActionHandler&) = delete;
  AttributeEditorActionHandler(AttributeEditorActionHandler&&) = delete;
  AttributeEditorActionHandler& operator=(AttributeEditorActionHandler&&) = delete;

  /**
   * @brief Checks if currently selected attribute can be switched to placeholder type.
   *
   * Attributes that accept text (`$par`, `@var`). The attribute returns to a value by clearing the
   * text.
   */
  bool CanSetPlaceholderType() const;

  /**
   * @brief Sets the placeholder text to the currently selected attribute.
   * @see AttributeEditorActionHandler::CanSetPlaceholderType.
   */
  void OnSetPlaceholderType();

  /**
   * @brief Checks if it is possible to call external AnyValueEditor for the currently selected
   * attribute.
   *
   * We do not implement OnEditAnyValue since it is handled separately by
   * InstructionEditorActionHandler and WorkspaceEditorActionHandler.
   */
  bool CanEditAnyValue() const;

private:
  /**
   * @brief Returns currently selected item.
   */
  mvvm::SessionItem* GetSelectedItem() const;

  /**
   * @brief Returns currently selected attribute, or nullptr.
   */
  AttributeItem* GetSelectedAttribute() const;

  /**
   * @brief Checks if the currently selected item is the AnyValue of an instruction or variable.
   */
  bool IsAnyValue() const;

  AttributeEditorContext m_context;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_ATTRIBUTE_EDITOR_ACTION_HANDLER_H_
