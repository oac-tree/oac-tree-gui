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

#ifndef LIBANYVALUE_ANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_EDITOR_CONTEXT_H_
#define LIBANYVALUE_ANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_EDITOR_CONTEXT_H_

#include <sup/gui/components/message_event.h>

#include <functional>
#include <string>

namespace sup::gui
{
class AnyValueItem;
}

namespace sup::gui
{

//! Context to setup AnyvalueEditorActions.
struct AnyValueEditorContext
{
  //!< callback to retrieve currently selected AnyValueItem
  std::function<sup::gui::AnyValueItem*()> get_selected_callback;

  //!< callback to report an error
  std::function<void(const sup::gui::MessageEvent&)> send_message_callback;
};

}  // namespace sup::gui

#endif
