/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/nodeeditor/graphicsview.h"

#include "sequencergui/nodeeditor/graphicsscene.h"

#include <QKeyEvent>

namespace sequi
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
      deleteSelectedRequest();
      break;
  case Qt::Key_Backspace:
      deleteSelectedRequest();
      break;
  default:
    QGraphicsView::keyPressEvent(event);
  }
}

}  // namespace sequi
