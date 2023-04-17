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

#ifndef SEQUENCERGUI_COMPONENTS_MESSAGE_HANDLER_FACTORY_H_
#define SEQUENCERGUI_COMPONENTS_MESSAGE_HANDLER_FACTORY_H_

//! Factory functions to create various message reporters.

#include <sup/gui/components/message_handler_interface.h>

#include <memory>

class QWidget;

namespace sequencergui
{

//! Creates message handler that does nothing.
std::unique_ptr<sup::gui::MessageHandlerInterface> CreateNullMessageHandler();

//! Creates message handler that prints messages to standard output.
std::unique_ptr<sup::gui::MessageHandlerInterface> CreateStdMessageHandler();

//! Creates message handler intended to work with the graphics view.
std::unique_ptr<sup::gui::MessageHandlerInterface> CreateWidgetOverlayMessageHandler(QWidget* view);

//! Creates message handler decorator.
std::unique_ptr<sup::gui::MessageHandlerInterface> CreateMessageHandlerDecorator(
    sup::gui::MessageHandlerInterface* component);

//! Creates message handler that pop's up a message box.
std::unique_ptr<sup::gui::MessageHandlerInterface> CreateMessageBoxHandler();

//! Creates message handler that throws runtime exception.
std::unique_ptr<sup::gui::MessageHandlerInterface> CreateThrowingMessageHandler();


}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_MESSAGE_HANDLER_FACTORY_H_
