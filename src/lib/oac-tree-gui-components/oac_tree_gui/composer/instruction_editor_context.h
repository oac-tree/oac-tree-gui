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

#ifndef OAC_TREE_GUI_COMPOSER_INSTRUCTION_EDITOR_CONTEXT_H_
#define OAC_TREE_GUI_COMPOSER_INSTRUCTION_EDITOR_CONTEXT_H_

#include <sup/gui/core/message_event.h>

#include <functional>
#include <memory>

class QMimeData;

namespace mvvm
{
class SessionItem;
}

namespace sup::gui
{
class AnyValueItem;
}

namespace oac_tree_gui
{

class InstructionContainerItem;
class InstructionItem;
struct AnyValueDialogResult;

/**
 * @brief The InstructionEditorContext struct holds collection of callbacks to retrieve current
 * state of InstructionEditor.
 *
 * It is used by InstructionEditorActionHandler to handle actions related to instruction tree
 * composition.
 */
struct InstructionEditorContext
{
  //! callback to get current instruction container to operate
  std::function<mvvm::SessionItem*()> instruction_container;

  //! callback to get currently selected instructions
  std::function<std::vector<InstructionItem*>()> selected_instructions;

  //!< A callback to notify the user that the item should be selected in a widget. This can
  //!< happen when a new item was just inserted, or some other item should be selected instead of
  //!< just removed.
  std::function<void(mvvm::SessionItem*)> notify_request;

  //! optional factory function that allows to create single instructions and complex aggregates
  std::function<std::unique_ptr<InstructionItem>(const std::string&)> create_instruction;

  //!< callback to report an error
  std::function<void(const sup::gui::MessageEvent&)> send_message;

  //!< callback that sends AnyValueItem for editing, and receives cloned (and possibly edited)
  //!< AnyValueItem back
  std::function<AnyValueDialogResult(const sup::gui::AnyValueItem*)> edit_anyvalue;

  //! callback to get mime data from the clipboard
  std::function<const QMimeData*()> get_mime_data;

  //! callback to set mime data to the clipboard
  std::function<void(std::unique_ptr<QMimeData>)> set_mime_data;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_INSTRUCTION_EDITOR_CONTEXT_H_
