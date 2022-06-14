/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/core/message_handler_factory.h"

#include "sequencergui/core/message_handler_decorator.h"
#include "sequencergui/core/standard_message_handlers.h"
#include "sequencergui/core/widget_overlay_message_handler.h"
#include "sequencergui/widgets/dialog_message_handler.h"

namespace sequencergui
{

std::unique_ptr<MessageHandlerInterface> CreateNullMessageHandler()
{
  return std::make_unique<NullMessageHandler>();
}

std::unique_ptr<MessageHandlerInterface> CreateStdMessageHandler()
{
  return std::make_unique<StdMessageHandler>();
}

std::unique_ptr<MessageHandlerInterface> CreateWidgetOverlayMessageHandler(QWidget *view)
{
  return std::make_unique<WidgetOverlayMessageHandler>(view);
}

std::unique_ptr<MessageHandlerInterface> CreateMessageHandlerDecorator(
    MessageHandlerInterface *component)
{
  return MessageHandlerDecorator::Create(component);
}

std::unique_ptr<MessageHandlerInterface> CreateMessageBoxHandler()
{
  return std::make_unique<DialogMessageHandler>();
}

}  // namespace sequencergui
