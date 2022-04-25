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

#include "mvvm/widgets/widgetutils.h"

#include <QAbstractScrollArea>
#include <QApplication>
#include <QEvent>
#include <QScrollBar>

namespace
{

//! Returns horizontal distance to the widget corner.
int GetXposOffset()
{
  const int number_of_characters = 1;
  return mvvm::utils::WidthOfLetterM() * number_of_characters;
}

//! Returns horizontal distance to the widget corner.
int GetYposOffset()
{
  const int number_of_characters = 1;
  return mvvm::utils::HeightOfLetterM() * number_of_characters;
}

}  // namespace

namespace sequencergui
{

OverlayMessageController::OverlayMessageController(OverlayMessageFrame* message, QWidget* area,
                                                   QObject* parent)
    : QObject(parent), m_message_frame(message), m_area(area)
{
  m_area->installEventFilter(this);
  UpdateLabelGeometry();
  m_message_frame->show();
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
//  m_message_frame->SetRectangle(QRect(0, 0, GetMaximumBoxWidth(), GetMaximumBoxHeight()));

  auto pos = GetBoxPosition();
  m_message_frame->SetPosition(pos.x(), pos.y());
}

//! Caclulates position of message box.
//! Takes into account scroll bars if exist.

QPoint OverlayMessageController::GetBoxPosition() const
{
  int x = m_area->width() - m_message_frame->width() - GetXposOffset();
  int y = m_area->height() - m_message_frame->height() - GetYposOffset();

  // shift position a bit, if scroll bars are present and visible
  if (auto scroll_area = dynamic_cast<QAbstractScrollArea*>(m_area); scroll_area)
  {
    if (QScrollBar* horizontal = scroll_area->horizontalScrollBar();
        horizontal && horizontal->isVisible())
    {
      y -= horizontal->height();
    }

    if (QScrollBar* vertical = scroll_area->verticalScrollBar(); vertical && vertical->isVisible())
    {
      x -= vertical->width();
    }
  }

  return {x, y};
}

}  // namespace sequencergui
