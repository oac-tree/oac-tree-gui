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

#ifndef SEQUENCERGUI_PVMONITOR_WORKSPACE_EDITOR_CONTEXT_H_
#define SEQUENCERGUI_PVMONITOR_WORKSPACE_EDITOR_CONTEXT_H_

#include <sup/gui/components/message_event.h>

#include <functional>
#include <memory>
#include <string>

class QWidget;

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
};

/**
 * @brief Create callback to summon AnyValueItem editor dialog.
 *
 * @param parent Parent widget to use for initial dialog placement.
 */
std::function<AnyValueDialogResult(const sup::gui::AnyValueItem*)> CreateAnyValueDialogCallback(
    QWidget* parent);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_WORKSPACE_EDITOR_CONTEXT_H_
