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

#include "sequencergui/nodeeditor/graphicsview.h"

#include "sequencergui/nodeeditor/graphicsscene.h"

#include "sequencergui/utils/styleutils.h"

#include <QKeyEvent>

namespace sequencergui
{
GraphicsView::GraphicsView(GraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent)
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setRenderHint(QPainter::Antialiasing);
  setAcceptDrops(true);
  setMouseTracking(true);
  setDragMode(QGraphicsView::RubberBandDrag);
}

void GraphicsView::onCenterView()
{
  centerOn(scene()->itemsBoundingRect().center());
}

GraphicsView::ESelectionModes GraphicsView::GetSelectionModes()
{
  static const std::map<QGraphicsView::DragMode, ESelectionModes> mode_map{
      {QGraphicsView::NoDrag, kSimpleSelection},
      {QGraphicsView::RubberBandDrag, kRubberSelection},
      {QGraphicsView::ScrollHandDrag, kHandDrag}};

  auto it = mode_map.find(dragMode());
  return it == mode_map.end() ? kUnknownSelection : it->second;
}

QSize GraphicsView::sizeHint() const
{
  return {styleutils::UnitSize()*80, styleutils::UnitSize()*60};
}

QSize GraphicsView::minimumSizeHint() const
{
  return {styleutils::UnitSize()*40, styleutils::UnitSize()*30};
}

void GraphicsView::onSelectionMode(int mode)
{
  switch (mode)
  {
  case kSimpleSelection:
    setDragMode(QGraphicsView::NoDrag);
    setInteractive(true);
    emit selectionModeChanged(kSimpleSelection);
    break;
  case kRubberSelection:
    setDragMode(QGraphicsView::RubberBandDrag);
    setInteractive(true);
    emit selectionModeChanged(kRubberSelection);
    break;
  case kHandDrag:
    setDragMode(QGraphicsView::ScrollHandDrag);
    setInteractive(false);
    emit selectionModeChanged(kHandDrag);
    break;
  default:
    break;
  }
}

void GraphicsView::onChangeScale(double new_scale)
{
  QTransform oldMatrix = transform();
  resetTransform();
  translate(oldMatrix.dx(), oldMatrix.dy());
  scale(new_scale, new_scale);
}

//! Handles mouse press events.

void GraphicsView::keyPressEvent(QKeyEvent* event)
{
  switch (event->key())
  {
  case Qt::Key_Space:
    if (GetSelectionModes() != kHandDrag && !event->isAutoRepeat())
    {
      onSelectionMode(kHandDrag);
    }
    break;
  default:
    QGraphicsView::keyPressEvent(event);
  }
}

//! Handles mouse release events.

void GraphicsView::keyReleaseEvent(QKeyEvent* event)
{
  switch (event->key())
  {
  case Qt::Key_Space:
    if (GetSelectionModes() != kRubberSelection && !event->isAutoRepeat())
    {
      onSelectionMode(kRubberSelection);  // space released
    }
    break;
  case Qt::Key_Delete:
    emit deleteSelectedRequest();
    break;
  case Qt::Key_Backspace:
    emit deleteSelectedRequest();
    break;
  default:
    QGraphicsView::keyPressEvent(event);
  }
}

}  // namespace sequencergui
