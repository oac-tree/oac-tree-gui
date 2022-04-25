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

#include "sequencergui/widgets/overlaymessagecontroller.h"

#include "sequencergui/widgets/overlaymessageframe.h"

#include <QAbstractScrollArea>
#include <QEvent>

namespace sequencergui
{

OverlayMessageController::OverlayMessageController(OverlayMessageFrame* message,
                                                   QAbstractScrollArea* area, QObject* parent)
    : QObject(parent), m_message_frame(message), m_area(area)
{
  m_area->installEventFilter(this);
  UpdateLabelGeometry();
}

bool OverlayMessageController::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::Resize)
  {
    UpdateLabelGeometry();
  }

  return QObject::eventFilter(obj, event);
}

void OverlayMessageController::UpdateLabelGeometry()
{
  if (!m_message_frame || !m_area)
  {
    return;
  }
  m_message_frame->SetRectangle(QRect(0, 0, m_area->width(), m_area->height()));
  m_message_frame->SetPosition(0, 0);
}

}  // namespace sequencergui
