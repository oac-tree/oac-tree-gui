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

#ifndef SEQUENCERGUI_CORE_WIDGETOVERLAYMESSAGEHANDLER_H
#define SEQUENCERGUI_CORE_WIDGETOVERLAYMESSAGEHANDLER_H

#include <sequencergui/core/message_handler_interface.h>
#include <memory>

class QWidget;

namespace sequencergui
{

class WidgetOverlayMessagePublisher;

//! Display messages in widget corner.

class WidgetOverlayMessageHandler : public MessageHandlerInterface
{
public:
  explicit WidgetOverlayMessageHandler(QWidget* view);
  ~WidgetOverlayMessageHandler();

  void SendMessage(const std::string& text) override;

private:
  std::unique_ptr<WidgetOverlayMessagePublisher> m_message_publisher;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_CORE_WIDGETOVERLAYMESSAGEHANDLER_H
