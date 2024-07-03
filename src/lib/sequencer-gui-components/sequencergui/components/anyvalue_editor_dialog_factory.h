/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPONENTS_ANYVALUE_EDITOR_DIALOG_FACTORY_H_
#define SEQUENCERGUI_COMPONENTS_ANYVALUE_EDITOR_DIALOG_FACTORY_H_

#include <functional>
#include <memory>

class QWidget;

namespace sup::gui
{
class AnyValueItem;
class AnyValueEditorDialog;
}  // namespace sup::gui

namespace sequencergui
{

/**
 * @brief The AnyValueDialogResult struct is intended to return AnyValueItem from the dialog.
 */
struct AnyValueDialogResult
{
  //!< Flag showing if operation was finished (OK button), or dialog was canceled. Canceled dialog
  //!< means that the user whants to leave previous AnyValueItem intact.
  bool is_accepted;
  //!< result of editing; empty object means that the user wants to remove previous item
  std::unique_ptr<sup::gui::AnyValueItem> result;
};

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

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_ANYVALUE_EDITOR_DIALOG_FACTORY_H_
