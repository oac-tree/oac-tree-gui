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

#ifndef OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_GRAPHICS_VIEW_H_
#define OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_GRAPHICS_VIEW_H_

#include <oac_tree_gui/nodeeditor/graphics_scene_types.h>
#include <oac_tree_gui/style/graphics_view_style.h>

#include <QGraphicsView>

class QKeyEvent;
class QGraphicsScene;

namespace oac_tree_gui
{

/**
 * @brief The NodeGraphicsView class shows NodeGraphicsScene and is responsible for
 * zoom/selection/centering activities.
 */
class NodeGraphicsView : public QGraphicsView
{
  Q_OBJECT

public:
  explicit NodeGraphicsView(QGraphicsScene* scene, QWidget* parent_widget = nullptr);

  /**
   * @brief Center view on the center of bounding rectangle surrounding all items.
   */
  void CenterView();

  /**
   * @brief Zoom view and re-center to fit everything in a view.
   */
  void FitView();

  /**
   * @brief Returns current operation mode.
   */
  GraphicsViewOperationMode GetOperationMode();

  /**
   * @brief Sets current operation mode.
   */
  void SetOperationMode(GraphicsViewOperationMode mode);

  /**
   * @brief Zooms into the view by the custom factor.
   *
   * A value over 1.0 zooms into the view and a value between 0.0 and 1.0 zooms out of it.
   */
  void SetZoomFactor(double factor);

  /**
   * @brief Returns current zoom factor.
   */
  double GetZoomFactor() const;

signals:
  void deleteSelectedRequest();
  void OperationModeChanged(oac_tree_gui::GraphicsViewOperationMode);
  void ZoomFactorChanged(double value);

protected:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void drawBackground(QPainter* painter, const QRectF& rect) override;

private:
  /**
   * @brief If zoom factor can be bigger.
   */
  bool CanZoomIn() const;

  /**
   * @brief If zoom factor can be smaller.
   */
  bool CanZoomOut() const;

  /**
   * @brief Draws grid with the given step.
   */
  void DrawGrid(double grid_step, QPainter* painter);

  GraphicsViewOperationMode m_operation_mode{GraphicsViewOperationMode::kSimpleSelection};
  style::GraphicsViewStyle m_view_style;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_GRAPHICS_VIEW_H_
