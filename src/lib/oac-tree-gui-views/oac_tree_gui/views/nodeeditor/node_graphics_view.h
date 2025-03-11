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

#ifndef OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_GRAPHICS_VIEW_H_
#define OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_GRAPHICS_VIEW_H_

#include <oac_tree_gui/nodeeditor/graphics_scene_types.h>

#include <QGraphicsView>

class QKeyEvent;

namespace oac_tree_gui
{

class NodeGraphicsScene;

/**
 * @brief The NodeGraphicsView class shows NodeGraphicsScene and is responsible for
 * zoom/selection/centering activities.
 */
class NodeGraphicsView : public QGraphicsView
{
  Q_OBJECT

public:
  explicit NodeGraphicsView(NodeGraphicsScene* scene, QWidget* parent_widget = nullptr);

  /**
   * @brief Center view on the center of bounding rectangle surrounding all items.
   */
  void CenterView();

  /**
   * @brief Returns current operation mode.
   */
  GraphicsViewOperationMode GetOperationMode();

  /**
   * @brief Sets current operation mode.
   */
  void SetOperationMode(GraphicsViewOperationMode mode);

  void onChangeScale(double new_scale);

  /**
   * @brief Zooms into the view by the custom factor factor.
   *
   * A factor over 1.0 zooms into the view and a factor between 0.0 and 1.0 zooms out of it.
   */
  void Zoom(double factor);

signals:
  void deleteSelectedRequest();
  void OperationModeChanged(oac_tree_gui::GraphicsViewOperationMode);

protected:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

private:
  GraphicsViewOperationMode m_operation_mode{GraphicsViewOperationMode::kSimpleSelection};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_GRAPHICS_VIEW_H_
