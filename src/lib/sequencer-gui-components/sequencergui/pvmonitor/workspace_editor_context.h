/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

class VariableItem;
class WorkspaceItem;

//! Context to setup WorkspaceEditorActions.

struct WorkspaceEditorContext
{
  //!< callback to retrieve currently selected item
  std::function<mvvm::SessionItem*()> get_selected_item_callback;

  //!< callback to report an error
  std::function<void(const sup::gui::MessageEvent&)> send_message_callback;

  //!< callback that sends AnyValueItem for editing, and receives cloned (and possibly edited)
  //!< AnyValueItem back
  std::function<std::unique_ptr<sup::gui::AnyValueItem>(const sup::gui::AnyValueItem&)>
      get_anyvalue_callback;

  //!< callback to retrieve current workspace to operate
  std::function<WorkspaceItem*()> get_workspace_callback;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_WORKSPACE_EDITOR_CONTEXT_H_
