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

#include <memory>

class QWidget;

namespace sup::gui
{
class AnyValueItem;
}  // namespace sup::gui

namespace sequencergui
{

class AnyValueEditorDialog;

/**
 * @brief Helper function to create a dialog containing full-fleged AnyValueEditor.
 *
 * @param item Initial value.
 * @param parent Parent widget.
 *
 * @return Created dialog.
 */
std::unique_ptr<AnyValueEditorDialog> CreateAnyValueExtendedEditorDialog(
    const sup::gui::AnyValueItem* item, QWidget* parent = nullptr);

/**
 * @brief Helper function to create a dialog containing simplified tree-like AnyValue editor.
 *
 * @param item Initial value.
 * @param parent Parent widget.
 *
 * @return Created dialog.
 */
std::unique_ptr<AnyValueEditorDialog> CreateAnyValueCompactTreeEditorDialog(
    const sup::gui::AnyValueItem* item, QWidget* parent = nullptr);

/**
 * @brief Helper function to create a dialog containing simplified scalar AnyValue editor.
 *
 * @param item Initial value.
 * @param parent Parent widget.
 *
 * @return Created dialog.
 */
std::unique_ptr<AnyValueEditorDialog> CreateAnyValueCompactScalarEditorDialog(
    const sup::gui::AnyValueItem* item, QWidget* parent = nullptr);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_ANYVALUE_EDITOR_DIALOG_FACTORY_H_
