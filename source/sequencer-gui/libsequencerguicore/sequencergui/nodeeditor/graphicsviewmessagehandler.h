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

#ifndef SEQUENCERGUI_NODEEDITOR_GRAPHICSVIEWMESSAGEHANDLER_H
#define SEQUENCERGUI_NODEEDITOR_GRAPHICSVIEWMESSAGEHANDLER_H

#include "sequencergui/core/messagehandlerinterface.h"
#include <memory>

class QGraphicsView;

namespace sequencergui
{

class WidgetMessageAppearanceController;

//!

class GraphicsViewMessageHandler : public MessageHandlerInterface
{
public:
  explicit GraphicsViewMessageHandler(QGraphicsView* view);
  ~GraphicsViewMessageHandler();

  void SendMessage(const std::string& text) override;

private:
  std::unique_ptr<WidgetMessageAppearanceController> m_message_controller;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_GRAPHICSVIEWMESSAGEHANDLER_H
