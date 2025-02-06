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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_COMPOSER_INSTRUCTION_EDITOR_CONTEXT_H_
#define OAC_TREE_GUI_COMPOSER_INSTRUCTION_EDITOR_CONTEXT_H_

#include <sup/gui/core/message_event.h>

#include <functional>
#include <memory>

class QMimeData;

namespace sup::gui
{
class AnyValueItem;
}

namespace oac_tree_gui
{

class ProcedureItem;
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
  //! callback to get currently selected procedure
  std::function<ProcedureItem*()> selected_procedure;

  //! callback to get currently selected instruction
  std::function<InstructionItem*()> selected_instruction;

  //!< callback to report an error
  std::function<void(const sup::gui::MessageEvent&)> send_message;

  //!< callback that sends AnyValueItem for editing, and receives cloned (and possibly edited)
  //!< AnyValueItem back
  std::function<AnyValueDialogResult(const sup::gui::AnyValueItem*)> edit_anyvalue_callback;

  //! callback to get mime data from the clipboard
  std::function<const QMimeData*()> get_mime_data;

  //! callback to set mime data to the clipboard
  std::function<void(std::unique_ptr<QMimeData>)> set_mime_data;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_INSTRUCTION_EDITOR_CONTEXT_H_
