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

#ifndef OAC_TREE_GUI_NODEEDITOR_NODE_CONNECTION_CONTROLLER_H_
#define OAC_TREE_GUI_NODEEDITOR_NODE_CONNECTION_CONTROLLER_H_

//! Node editor: original code is taken from
//! http://algoholic.eu/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/
//! Copyright (c) 2012, STANISLAW ADASZEWSKI

#include <QObject>

class QGraphicsScene;
class QGraphicsSceneMouseEvent;

namespace oac_tree_gui
{

class NodeConnection;
class NodePort;
class ConnectableView;

/**
 * @brief The NodeConnectionController class assists GraphicsScene in establishing a connection
 * between two ports.
 */
class NodeConnectionController : public QObject
{
  Q_OBJECT

public:
  explicit NodeConnectionController(QGraphicsScene* scene);

  bool eventFilter(QObject* object, QEvent* event) override;

signals:
  void connectionRequest(oac_tree_gui::ConnectableView* childView,
                         oac_tree_gui::ConnectableView* parentView);
  void selectionModeChangeRequest(int);

private:
  /**
   * @brief Finds NodePort around given coordinate.
   */
  NodePort* FindPort(const QPointF& pos);

  bool ProcessMousePress(QGraphicsSceneMouseEvent* event);
  bool ProcessMouseMove(QGraphicsSceneMouseEvent* event);
  bool ProcessMouseRelease(QGraphicsSceneMouseEvent* event);
  void ResetConnection();

  QGraphicsScene* m_scene{nullptr};
  NodeConnection* m_current_connection{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_NODE_CONNECTION_CONTROLLER_H_
