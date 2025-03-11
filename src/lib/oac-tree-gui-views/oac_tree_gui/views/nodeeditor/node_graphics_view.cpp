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
#include <oac_tree_gui/nodeeditor/scene_constants.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QKeyEvent>
#include <QWheelEvent>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Returns Qt drag mode suitable for the given operation mode.
 */
QGraphicsView::DragMode GetQtDragMode(GraphicsViewOperationMode operation_mode)
{
  static const std::map<GraphicsViewOperationMode, QGraphicsView::DragMode> kModeMap{
      {GraphicsViewOperationMode::kSimpleSelection, QGraphicsView::NoDrag},
      {GraphicsViewOperationMode::kRubberSelection, QGraphicsView::RubberBandDrag},
      {GraphicsViewOperationMode::kHandDrag, QGraphicsView::ScrollHandDrag}};

  auto iter = kModeMap.find(operation_mode);
  if (iter == kModeMap.end())
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
  setInteractive(m_operation_mode
                 != GraphicsViewOperationMode::kHandDrag);  // non-interactive for kHandDrag
  emit OperationModeChanged(m_operation_mode);
}

void NodeGraphicsView::SetZoomFactor(double factor)
{
  QTransform matrix;
  matrix.scale(factor, factor);
  setTransform(matrix);
  emit ZoomFactorChanged(factor);
}

double NodeGraphicsView::GetZoomFactor() const
{
  return transform().m11();
}

void NodeGraphicsView::keyPressEvent(QKeyEvent* event)
{
  switch (event->key())
  {
  case Qt::Key_Space:
    if (GetOperationMode() != GraphicsViewOperationMode::kHandDrag && !event->isAutoRepeat())
    {
      SetOperationMode(GraphicsViewOperationMode::kHandDrag);
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
    if (GetOperationMode() != GraphicsViewOperationMode::kRubberSelection && !event->isAutoRepeat())
    {
      SetOperationMode(GraphicsViewOperationMode::kRubberSelection);  // space released
    }
    break;
  case Qt::Key_Delete:
  case Qt::Key_Backspace:
    emit deleteSelectedRequest();
    break;
  default:
    QGraphicsView::keyPressEvent(event);
  }
}

void NodeGraphicsView::wheelEvent(QWheelEvent* event)
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    const bool zoom_in_wheel = event->angleDelta().y() > 0;
    const bool zoom_out_wheel = !zoom_in_wheel;

    const double factor = zoom_in_wheel ? constants::kWheelDefaultZoomInFactor
                                        : 1. / constants::kWheelDefaultZoomInFactor;

    if (zoom_in_wheel && !CanZoomIn())
    {
      return;
    }

    if (zoom_out_wheel && !CanZoomOut())
    {
      return;
    }

    qDebug() << "NodeGraphicsView::wheelEvent " << factor << GetZoomFactor()
             << event->angleDelta().y();

    // zoom in/out while holding the focus on cursor position
    const auto anchor = transformationAnchor();
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    scale(factor, factor);
    setTransformationAnchor(anchor);
    emit ZoomFactorChanged(GetZoomFactor());
  }
  else
  {
    QGraphicsView::wheelEvent(event);
  }
}

bool NodeGraphicsView::CanZoomIn() const
{
  return GetZoomFactor() < constants::kMaxZoomFactor;
}

bool NodeGraphicsView::CanZoomOut() const
{
  return GetZoomFactor() > constants::kMinZoomFactor;
}

}  // namespace oac_tree_gui
