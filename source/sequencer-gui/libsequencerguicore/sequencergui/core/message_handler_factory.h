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

#ifndef SEQUENCERGUI_CORE_MESSAGEHANDLERFACTORY_H
#define SEQUENCERGUI_CORE_MESSAGEHANDLERFACTORY_H

//! @file messagehandlerfactory.h
//! Factory functions to create various message reporters.

#include "sequencergui/core/message_handler_interface.h"

#include <memory>

class QWidget;

namespace sequencergui
{

//! Creates message handler that prints messages to standard output.
std::unique_ptr<MessageHandlerInterface> CreateStdMessageHandler();

//! Creates message handler intended to work with the graphics view.
std::unique_ptr<MessageHandlerInterface> CreateWidgetOverlayMessageHandler(QWidget* view);

//! Creates message handler decorator.
std::unique_ptr<MessageHandlerInterface> CreateMessageHandlerDecorator(
    MessageHandlerInterface* component);

//! Creates message handler that pop's up a message box.
std::unique_ptr<MessageHandlerInterface> CreateMessageBoxHandler();

}  // namespace sequencergui

#endif  // SEQUENCERGUI_CORE_MESSAGEHANDLERFACTORY_H
