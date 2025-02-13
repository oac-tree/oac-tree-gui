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

#ifndef OAC_TREE_GUI_VIEWS_NODEEDITOR_GRAPHICS_VIEW_H_
#define OAC_TREE_GUI_VIEWS_NODEEDITOR_GRAPHICS_VIEW_H_

#include <oac_tree_gui/nodeeditor/graphics_scene_types.h>

#include <QGraphicsView>

class QKeyEvent;

namespace oac_tree_gui
{

class GraphicsScene;

/**
 * @brief The GraphicsView class shows GraphicsScene and is responsible for zoom/selection/centering
 * activities.
 */
class GraphicsView : public QGraphicsView
{
  Q_OBJECT

public:
  GraphicsView(GraphicsScene* scene, QWidget* parent_widget);

  void CenterView();

  ESelectionModes GetSelectionModes();

  void onSelectionMode(int);
  void onChangeScale(double new_scale);

signals:
  void deleteSelectedRequest();
  void selectionModeChanged(int);

protected:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_GRAPHICS_VIEW_H_
