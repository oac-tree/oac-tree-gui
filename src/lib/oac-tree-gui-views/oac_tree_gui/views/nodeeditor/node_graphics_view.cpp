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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "node_graphics_view.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/nodeeditor/scene_constants.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>
#include <oac_tree_gui/style/style_helper.h>
#include <mvvm/style/color_flavor.h>

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

NodeGraphicsView::NodeGraphicsView(QGraphicsScene* scene, QWidget* parent_widget)
    : QGraphicsView(scene, parent_widget)
    , m_view_style(CreateStyleFromResource<style::GraphicsViewStyle>(
          mvvm::ColorFlavor::kUnspecified))
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setRenderHint(QPainter::Antialiasing);
  setAcceptDrops(true);
  setMouseTracking(true);
  setDragMode(QGraphicsView::RubberBandDrag);

  if (m_view_style.render_background)
  {
    setBackgroundBrush(m_view_style.background_color);
  }
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

void NodeGraphicsView::FitView()
{
  if (scene()->items().isEmpty())
  {
    centerOn(GetGraphicsViewportCenter());
    SetZoomFactor(1.0);
  }
  else
  {
    auto rect = scene()->itemsBoundingRect();
    QGraphicsView::fitInView(rect, Qt::KeepAspectRatio);
    emit ZoomFactorChanged(GetZoomFactor());
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

void NodeGraphicsView::drawBackground(QPainter* painter, const QRectF& rect)
{
  QGraphicsView::drawBackground(painter, rect);

  if (!m_view_style.render_background)
  {
    return;
  }

  painter->setPen(QPen(m_view_style.fine_grid_color, 1.0));
  DrawGrid(m_view_style.fine_grid_size, painter);

  painter->setPen(QPen(m_view_style.corase_grid_color, 1.0));
  DrawGrid(m_view_style.coarse_grid_size, painter);
}

bool NodeGraphicsView::CanZoomIn() const
{
  return GetZoomFactor() < constants::kMaxZoomFactor;
}

bool NodeGraphicsView::CanZoomOut() const
{
  return GetZoomFactor() > constants::kMinZoomFactor;
}

void NodeGraphicsView::DrawGrid(double grid_step, QPainter* painter)
{
  const QRect window_rect = this->rect();
  const QPointF top_left = mapToScene(window_rect.topLeft());
  const QPointF bottom_right = mapToScene(window_rect.bottomRight());

  const double left = std::floor((top_left.x() / grid_step) - 0.5);
  const double right = std::floor((bottom_right.x() / grid_step) + 1.0);
  const double bottom = std::floor((top_left.y() / grid_step) - 0.5);
  const double top = std::floor((bottom_right.y() / grid_step) + 1.0);

  // vertical lines
  for (auto xi = static_cast<std::int32_t>(left); xi <= static_cast<std::int32_t>(right); ++xi)
  {
    const QLineF line(xi * grid_step, bottom * grid_step, xi * grid_step, top * grid_step);
    painter->drawLine(line);
  }

  // horizontal lines
  for (auto yi = static_cast<std::int32_t>(bottom); yi <= static_cast<std::int32_t>(top); ++yi)
  {
    const QLineF line(left * grid_step, yi * grid_step, right * grid_step, yi * grid_step);
    painter->drawLine(line);
  }
}

}  // namespace oac_tree_gui
