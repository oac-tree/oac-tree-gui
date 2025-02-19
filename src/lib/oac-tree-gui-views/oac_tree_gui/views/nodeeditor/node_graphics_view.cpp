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

#include "node_graphics_view.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/nodeeditor/node_graphics_scene.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QKeyEvent>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Returns Qt drag mode suitable for the given operation mode.
 */
QGraphicsView::DragMode GetQtDragMode(GraphicsViewOperationMode operation_mode)
{
  static const std::map<GraphicsViewOperationMode, QGraphicsView::DragMode> mode_map{
      {kSimpleSelection, QGraphicsView::NoDrag},
      {kRubberSelection, QGraphicsView::RubberBandDrag},
      {kHandDrag, QGraphicsView::ScrollHandDrag}};

  auto iter = mode_map.find(operation_mode);
  if (iter == mode_map.end())
  {
    throw RuntimeException("Can't define Qt drag mode");
  }
  return iter->second;
}

}  // namespace

NodeGraphicsView::NodeGraphicsView(NodeGraphicsScene* scene, QWidget* parent_widget)
    : QGraphicsView(scene, parent_widget)
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setRenderHint(QPainter::Antialiasing);
  setAcceptDrops(true);
  setMouseTracking(true);
  setDragMode(QGraphicsView::RubberBandDrag);
}

void NodeGraphicsView::CenterView()
{
  if (scene()->items().isEmpty())
  {
    centerOn(GetGraphicsViewportCenter());
  }
  else
  {
    centerOn(scene()->itemsBoundingRect().center());
  }
}

GraphicsViewOperationMode NodeGraphicsView::GetOperationMode()
{
  return m_operation_mode;
}

void NodeGraphicsView::SetOperationMode(GraphicsViewOperationMode mode)
{
  m_operation_mode = mode;

  setDragMode(GetQtDragMode(m_operation_mode));
  setInteractive(m_operation_mode != kHandDrag);  // non-interactive for kHandDrag
  emit OperationModeChanged(m_operation_mode);
}

void NodeGraphicsView::onChangeScale(double new_scale)
{
  QTransform oldMatrix = transform();
  resetTransform();
  translate(oldMatrix.dx(), oldMatrix.dy());
  scale(new_scale, new_scale);
}

void NodeGraphicsView::keyPressEvent(QKeyEvent* event)
{
  switch (event->key())
  {
  case Qt::Key_Space:
    if (GetOperationMode() != kHandDrag && !event->isAutoRepeat())
    {
      SetOperationMode(kHandDrag);
    }
    break;
  default:
    QGraphicsView::keyPressEvent(event);
  }
}

void NodeGraphicsView::keyReleaseEvent(QKeyEvent* event)
{
  switch (event->key())
  {
  case Qt::Key_Space:
    if (GetOperationMode() != kRubberSelection && !event->isAutoRepeat())
    {
      SetOperationMode(kRubberSelection);  // space released
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
