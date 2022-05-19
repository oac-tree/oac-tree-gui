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

#include "sequencergui/widgets/overlaywidgetpositionstrategy.h"

#include "sequencergui/utils/styleutils.h"

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
  return sequencergui::styleutils::UnitSize() * number_of_characters;
}

//! Returns horizontal distance to the widget corner.
int GetYposOffset()
{
  const int number_of_characters = 1;
  return sequencergui::styleutils::UnitSize() * number_of_characters;
}

}  // namespace

namespace sequencergui
{

OverlayWidgetPositionStrategy::OverlayWidgetPositionStrategy(QWidget* overlay_widget, QWidget* area,
                                                             QObject* parent)
    : QObject(parent), m_overlay_widget(overlay_widget), m_area(area)
{
  m_area->installEventFilter(this);
  m_overlay_widget->show();
  UpdateMessageWidgetGeometry();
}

bool OverlayWidgetPositionStrategy::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::Resize)
  {
    UpdateMessageWidgetGeometry();
  }

  return QObject::eventFilter(obj, event);
}

void OverlayWidgetPositionStrategy::UpdateMessageWidgetGeometry()
{
  auto pos = CalculateWidgetPosition();
  m_overlay_widget->setGeometry(pos.x(), pos.y(), m_overlay_widget->width(),
                                m_overlay_widget->height());
}

//! Caclulates position of message widget to be aproximately at the lower right corner of the area.
//! Takes into account scroll bars if exist.

QPoint OverlayWidgetPositionStrategy::CalculateWidgetPosition() const
{
  int x = m_area->width() - m_overlay_widget->width() - GetXposOffset();
  int y = m_area->height() - m_overlay_widget->height() - GetYposOffset();

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
