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

#ifndef OAC_TREE_GUI_VIEWS_EDITORS_ANYVALUE_EDITOR_DIALOG_FACTORY_H_
#define OAC_TREE_GUI_VIEWS_EDITORS_ANYVALUE_EDITOR_DIALOG_FACTORY_H_

#include <oac_tree_gui/components/anyvalue_dialog_result.h>

#include <functional>
#include <memory>

class QWidget;

namespace sup::gui
{
class AnyValueItem;
class AnyValueEditorDialog;
}  // namespace sup::gui

namespace oac_tree_gui
{

/**
 * @brief Helper function to create a dialog containing full-fleged AnyValueEditor.
 *
 * @param item Initial value.
 * @param parent Parent widget.
 *
 * @return Created dialog.
 */
std::unique_ptr<sup::gui::AnyValueEditorDialog> CreateAnyValueExtendedEditorDialog(
    const sup::gui::AnyValueItem* item, QWidget* parent = nullptr);

/**
 * @brief Helper function to create a dialog containing simplified tree-like AnyValue editor.
 *
 * @param item Initial value.
 * @param parent Parent widget.
 *
 * @return Created dialog.
 */
std::unique_ptr<sup::gui::AnyValueEditorDialog> CreateAnyValueCompactTreeEditorDialog(
    const sup::gui::AnyValueItem* item, QWidget* parent = nullptr);

/**
 * @brief Helper function to create a dialog containing simplified scalar AnyValue editor.
 *
 * @param item Initial value.
 * @param parent Parent widget.
 *
 * @return Created dialog.
 */
std::unique_ptr<sup::gui::AnyValueEditorDialog> CreateAnyValueCompactScalarEditorDialog(
    const sup::gui::AnyValueItem* item, QWidget* parent = nullptr);

/**
 * @brief Create callback to summon AnyValueItem editor dialog.
 *
 * @param parent Parent widget to use for initial dialog placement.
 */
std::function<AnyValueDialogResult(const sup::gui::AnyValueItem*)> CreateAnyValueDialogCallback(
    QWidget* parent);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_EDITORS_ANYVALUE_EDITOR_DIALOG_FACTORY_H_
