/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "node_port.h"

#include <sequencergui/nodeeditor/connectable_view.h>
#include <sequencergui/nodeeditor/node_connection.h>

#include <mvvm/widgets/widget_utils.h>

#include <QFont>
#include <QPen>

namespace
{
double port_radius()
{
  return mvvm::utils::UnitSize(0.4);
}

}  // namespace

namespace sequencergui
{
NodePort::NodePort(QGraphicsItem* parent, const PortInfo& info)
    : QGraphicsPathItem(parent), m_label(new QGraphicsTextItem(this)), m_port_info(info)
{
  setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
  const double radius = port_radius();
  const QColor color(QColor(QString::fromStdString(info.m_color)));

  QPainterPath p;
  p.addEllipse(-radius, -radius, 2 * radius, 2 * radius);
  setPath(p);
  setPen(QPen(color.darker(180)));
  setBrush(color);

  QFont serifFont("Monospace", 8, QFont::Normal);
  m_label->setFont(serifFont);
}

NodePort::~NodePort()
{
  while (!m_connections.empty())
  {
    auto conn = m_connections.last();
    conn->setSelected(false);
    delete conn;
  }
}

void NodePort::SetPortInfo(const PortInfo& info)
{
  m_port_info = info;
  setBrush(QColor(QString::fromStdString(info.m_color)));
  update();
}

//! Returns port type. Input and output ports of the same type are compatible.

QString NodePort::portType() const
{
  return QString::fromStdString(m_port_info.m_type);
}

//! Returns true if this is a NodeOutputPort.

bool NodePort::isParentPort() const
{
  return !isChildPort();
}

void NodePort::append(NodeConnection* connection)
{
  m_connections.append(connection);
}

void NodePort::remove(NodeConnection* connection)
{
  m_connections.removeAll(connection);
}

//! Returns true if this port can be connected with the other one.

bool NodePort::isConnectable(const NodePort* other) const
{
  bool different_parents = parentItem() != other->parentItem();
  bool output_to_input = isChildPort() != other->isChildPort();
  bool compatible_types = portType() == other->portType();
  bool not_already_connected = !isConnected(other);
  return different_parents && output_to_input && compatible_types && not_already_connected;
}

//! Returns true if ports are connected.

bool NodePort::isConnected(const NodePort* other) const
{
  for (auto conn : m_connections)
  {
    if (conn->hasPort(other))
    {
      return true;
    }
  }
  return false;
}

//! Returns parent to which this port belongs.

ConnectableView* NodePort::connectableView() const
{
  return dynamic_cast<ConnectableView*>(parentItem());
}

//! Returns vector of connections of given port.

QList<NodeConnection*> NodePort::connections() const
{
  return m_connections;
}

QGraphicsTextItem* NodePort::GetLabel()
{
  return m_label;
}

QVariant NodePort::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
  if (change == ItemScenePositionHasChanged)
  {
    for (auto conn : m_connections)
    {
      conn->updatePath();
    }
  }
  return value;
}

// ----------------------------------------------------------------------------

//! Returns true if this is a NodeInputPort.

bool ChildPort::isChildPort() const
{
  return true;
}

// ----------------------------------------------------------------------------

//! Returns true if this is a NodeInputPort.

bool ParentPort::isChildPort() const
{
  return false;
}

}  // namespace sequencergui
