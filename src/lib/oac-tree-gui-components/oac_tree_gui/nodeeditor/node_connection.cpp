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

#include "node_connection.h"

#include <oac_tree_gui/components/component_helper.h>
#include <oac_tree_gui/nodeeditor/node_port.h>

#include <QGraphicsScene>
#include <QPainter>
#include <QPen>

namespace oac_tree_gui
{
NodeConnection::NodeConnection(QGraphicsScene* scene)
{
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setPen(QPen(Qt::darkGray, 2));
  setBrush(Qt::NoBrush);
  setZValue(-1);
  if (scene)
  {
    scene->addItem(this);
  }
}

NodeConnection::~NodeConnection()
{
  if (m_port1)
  {
    m_port1->remove(this);
  }

  if (m_port2)
  {
    m_port2->remove(this);
  }
}

void NodeConnection::setPos1(const QPointF& pos)
{
  m_pos1 = pos;
}

void NodeConnection::setPos2(const QPointF& pos)
{
  m_pos2 = pos;
}

void NodeConnection::setPort1(NodePort* port)
{
  m_port1 = port;
  m_port1->append(this);
  setPos1(port->scenePos());
}

void NodeConnection::setPort2(NodePort* port)
{
  m_port2 = port;
  m_port2->append(this);
  setPos2(port->scenePos());
}

void NodeConnection::updatePath()
{
  if (m_port1)
  {
    m_pos1 = m_port1->scenePos();
  }

  if (m_port2)
  {
    m_pos2 = m_port2->scenePos();
  }

  QPainterPath p;
  p.moveTo(m_pos1);

  if (GetBehaviorTreeStyle() == BehaviorTreeStyle::kTopToBottom)
  {
    qreal dx = m_pos2.y() - m_pos1.y();
    dx = qMax(dx, 200.);
    double step = m_port1->isParentPort() ? dx * 0.25 : -dx * 0.25;
    QPointF ctr1(m_pos1.x(), m_pos1.y() + step);
    QPointF ctr2(m_pos2.x(), m_pos2.y() - step);
    p.cubicTo(ctr1, ctr2, m_pos2);
  }
  else
  {
    qreal dx = m_pos2.x() - m_pos1.x();
    dx = qMax(dx, 200.);
    QPointF ctr1(m_pos1.x() + dx * 0.25, m_pos1.y());
    QPointF ctr2(m_pos2.x() - dx * 0.25, m_pos2.y());
    p.cubicTo(ctr1, ctr2, m_pos2);
  }

  setPath(p);
}

//! Returns the port from where the connection begins.
//! It corresponds to the NodePort, on which the user has clicked first while connecting two ports.

NodePort* NodeConnection::port1() const
{
  return m_port1;
}

//! Returns true if given port belongs to this connection.

bool NodeConnection::hasPort(const NodePort* port) const
{
  return m_port1 == port || m_port2 == port;
}

void NodeConnection::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
  painter->setPen(QPen(Qt::darkGray, 2));
  painter->setBrush(Qt::NoBrush);

  if (isSelected())
  {
    painter->setPen(Qt::DashLine);
  }

  painter->drawPath(path());
}

//! Returns ConnectableView playing the role of a parent.
//! By our convention, parent is the one who owns ParentPort belonging to this connection.

ConnectableView* NodeConnection::parentView() const
{
  auto parent_port = m_port1->isParentPort() ? m_port1 : m_port2;
  return parent_port->connectableView();
}

//! Returns ConnectableView playing the role of a child.
//! By our convention, the child is the one who owns ChildPort of this connection.

ConnectableView* NodeConnection::childView() const
{
  auto child_port = m_port1->isChildPort() ? m_port1 : m_port2;
  return child_port->connectableView();
}

}  // namespace oac_tree_gui
