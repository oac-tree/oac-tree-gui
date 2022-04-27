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

#include "sequencergui/widgets/graphicsviewmessagecontroller.h"

#include "sequencergui/widgets/overlaymessage.h"

#include <QGraphicsView>
#include <QTimer>

namespace sequencergui
{

GraphicsViewMessageController::GraphicsViewMessageController(QWidget *widget)
    : m_widget(widget), m_timer(new QTimer)
{
  m_timer->setSingleShot(true);
  m_timer->setInterval(5000);
  QObject::connect(m_timer, &QTimer::timeout, this,
                   &GraphicsViewMessageController::RemoveMessageOnTimeout);
}

GraphicsViewMessageController::~GraphicsViewMessageController() = default;

void GraphicsViewMessageController::AddMessage(const QString &text)
{
  // in the current implementation new message replaces the old one
  m_message = std::make_unique<OverlayMessage>(text, m_widget);

  m_timer->start();  // starting timer to delete a message after a while
}

void GraphicsViewMessageController::RemoveMessageOnTimeout()
{
  if (m_message->CanBeDeleted())
  {
    m_message.reset();
  }
  else
  {
    // if message can't be deleted now, repeat attempt
    m_timer->start();
  }
}

}  // namespace sequencergui
