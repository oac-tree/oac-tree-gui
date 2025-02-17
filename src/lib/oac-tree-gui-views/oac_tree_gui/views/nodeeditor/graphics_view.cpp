/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "graphics_view.h"

#include <oac_tree_gui/nodeeditor/node_graphics_scene.h>

#include <mvvm/widgets/widget_utils.h>

#include <QKeyEvent>

namespace oac_tree_gui
{
GraphicsView::GraphicsView(NodeGraphicsScene* scene, QWidget* parent_widget)
    : QGraphicsView(scene, parent_widget)
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setRenderHint(QPainter::Antialiasing);
  setAcceptDrops(true);
  setMouseTracking(true);
  setDragMode(QGraphicsView::RubberBandDrag);
}

void GraphicsView::CenterView()
{
  centerOn(scene()->itemsBoundingRect().center());
}

ESelectionModes GraphicsView::GetSelectionModes()
{
  static const std::map<QGraphicsView::DragMode, ESelectionModes> mode_map{
      {QGraphicsView::NoDrag, kSimpleSelection},
      {QGraphicsView::RubberBandDrag, kRubberSelection},
      {QGraphicsView::ScrollHandDrag, kHandDrag}};

  auto it = mode_map.find(dragMode());
  return it == mode_map.end() ? kUnknownSelection : it->second;
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

}  // namespace oac_tree_gui
