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

#ifndef SEQUENCERGUI_NODEEDITOR_NODE_PORT_H_
#define SEQUENCERGUI_NODEEDITOR_NODE_PORT_H_

//! Node editor: original code is taken from
//! http://algoholic.eu/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/
//! Copyright (c) 2012, STANISLAW ADASZEWSKI

#include <oac-tree-gui/nodeeditor/port_info.h>

#include <QGraphicsObject>
#include <QList>

namespace sequencergui
{
class NodeConnection;
class ConnectableView;

//! Base class representing parent/child ports of connectable view.
//! Looks like round element attached to parent's bounding box. Parent and child ports of the
//! same type can be connected together.

class NodePort : public QGraphicsPathItem
{
public:
  NodePort(QGraphicsItem* parent, const PortInfo& info);
  ~NodePort() override;

  void SetPortInfo(const PortInfo& info);

  QString portType() const;

  virtual bool isChildPort() const = 0;

  bool isParentPort() const;

  void append(NodeConnection* connection);

  void remove(NodeConnection* connection);

  bool isConnectable(const NodePort* other) const;

  bool isConnected(const NodePort* other) const;

  ConnectableView* connectableView() const;

  QList<NodeConnection*> connections() const;

  QGraphicsTextItem* GetLabel();

protected:
  QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

  QGraphicsTextItem* m_label{nullptr};
  QList<NodeConnection*> m_connections;
  PortInfo m_port_info;
};

//! Represents child port of connectable view.
//! The port is located on an instruction playing the role of a child. The connection from this port
//! should go toward parent view.

class ChildPort : public NodePort
{
public:
  using NodePort::NodePort;
  bool isChildPort() const override;
};

//! Represents parent port of connectable view.
//! The port is located on an instruction playing the role of a parent. The connection from this
//! port should go toward child view.

class ParentPort : public NodePort
{
public:
  using NodePort::NodePort;
  bool isChildPort() const override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_NODE_PORT_H_
