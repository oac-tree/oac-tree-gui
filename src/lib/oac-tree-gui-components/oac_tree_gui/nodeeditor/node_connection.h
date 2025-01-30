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

#ifndef OAC_TREE_GUI_NODEEDITOR_NODE_CONNECTION_H_
#define OAC_TREE_GUI_NODEEDITOR_NODE_CONNECTION_H_

//! Node editor: original code is taken from
//! http://algoholic.eu/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/
//! Copyright (c) 2012, STANISLAW ADASZEWSKI

#include <QGraphicsPathItem>

namespace oac_tree_gui
{

class NodePort;
class ConnectableView;

/**
 * @brief Elastic connection to connect ParentPort and ChildPort together.
 */
class NodeConnection : public QGraphicsPathItem
{
public:
  explicit NodeConnection(QGraphicsScene* scene);
  ~NodeConnection() override;

  void setPos1(const QPointF& pos);
  void setPos2(const QPointF& pos);
  void setPort1(NodePort* port);
  void setPort2(NodePort* port);

  void updatePath();

  NodePort* port1() const;

  bool hasPort(const NodePort* port) const;

  void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

  ConnectableView* parentView() const;

  ConnectableView* childView() const;

private:
  QPointF m_pos1;
  QPointF m_pos2;
  NodePort* m_port1{nullptr};  //! The beginning of the connection.
  NodePort* m_port2{nullptr};  //! Connection end.
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_NODE_CONNECTION_H_
