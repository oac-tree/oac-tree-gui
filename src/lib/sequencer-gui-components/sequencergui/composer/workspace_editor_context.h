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

#ifndef SEQUENCERGUI_COMPOSER_WORKSPACE_EDITOR_CONTEXT_H_
#define SEQUENCERGUI_COMPOSER_WORKSPACE_EDITOR_CONTEXT_H_

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

namespace sequencergui
{

class WorkspaceItem;
struct AnyValueDialogResult;

/**
 * @brief The WorkspaceEditorContext class represents a context to setup WorkspaceEditorActions.
 */
struct WorkspaceEditorContext
{
  //!< callback to retrieve current workspace to operate
  std::function<WorkspaceItem*()> selected_workspace_callback;

  //!< callback to retrieve currently selected item
  std::function<mvvm::SessionItem*()> selected_item_callback;

  //!< callback to report an error
  std::function<void(const sup::gui::MessageEvent&)> send_message_callback;

  //!< callback that sends AnyValueItem for editing, and receives cloned (and possibly edited)
  //!< AnyValueItem back
  std::function<AnyValueDialogResult(const sup::gui::AnyValueItem*)> edit_anyvalue_callback;

  //! callback to get mime data from the clipboard
  std::function<const QMimeData*()> get_mime_data;

  //! callback to set mime data to the clipboard
  std::function<void(std::unique_ptr<QMimeData>)> set_mime_data;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_WORKSPACE_EDITOR_CONTEXT_H_
