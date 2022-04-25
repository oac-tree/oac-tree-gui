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

GraphicsViewMessageController::GraphicsViewMessageController(QGraphicsView *view)
    : m_graphics_view(view)
{
}

GraphicsViewMessageController::~GraphicsViewMessageController() = default;

void GraphicsViewMessageController::AddMessage(const QString &text)
{
  // in the current implementation new message replaces the old one
  m_message = std::make_unique<OverlayMessage>(text, m_graphics_view);

  QTimer::singleShot(4000, this, [=]() { m_message.reset(); });


}

}  // namespace sequencergui
