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

#include "message_handler_factory.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/widgets/dialog_message_handler.h>
#include <sequencergui/widgets/widget_overlay_message_handler.h>
#include <sup/gui/components/message_handler_decorator.h>
#include <sup/gui/components/standard_message_handlers.h>

namespace sequencergui
{

std::unique_ptr<sup::gui::MessageHandlerInterface> CreateNullMessageHandler()
{
  return std::make_unique<sup::gui::NullMessageHandler>();
}

std::unique_ptr<sup::gui::MessageHandlerInterface> CreateStdMessageHandler()
{
  return std::make_unique<sup::gui::StdMessageHandler>();
}

std::unique_ptr<sup::gui::MessageHandlerInterface> CreateWidgetOverlayMessageHandler(QWidget *view)
{
  return std::make_unique<WidgetOverlayMessageHandler>(view);
}

std::unique_ptr<sup::gui::MessageHandlerInterface> CreateMessageHandlerDecorator(
    sup::gui::MessageHandlerInterface *component)
{
  return sup::gui::MessageHandlerDecorator::Create(component);
}

std::unique_ptr<sup::gui::MessageHandlerInterface> CreateMessageBoxHandler()
{
  return std::make_unique<DialogMessageHandler>();
}

std::unique_ptr<sup::gui::MessageHandlerInterface> CreateThrowingMessageHandler()
{
  return std::make_unique<sup::gui::ThrowingMessageHandler<::sequencergui::RuntimeException>>();
}

}  // namespace sequencergui
