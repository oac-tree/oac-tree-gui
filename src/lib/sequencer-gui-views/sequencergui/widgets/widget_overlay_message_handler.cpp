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

#include "widget_overlay_message_handler.h"

#include <sequencergui/widgets/widget_overlay_message_publisher.h>

namespace sequencergui
{

WidgetOverlayMessageHandler::WidgetOverlayMessageHandler(QWidget *view)
    : m_message_publisher(std::make_unique<WidgetOverlayMessagePublisher>(view))
{
}

WidgetOverlayMessageHandler::~WidgetOverlayMessageHandler() = default;

void WidgetOverlayMessageHandler::SendMessage(const std::string &text)
{
  m_message_publisher->AddMessage(QString::fromStdString(text));
}

}  // namespace sequencergui
