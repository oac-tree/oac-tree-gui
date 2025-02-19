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

#include "node_connection_controller.h"

#include "graphics_scene_types.h"

#include <oac_tree_gui/nodeeditor/node_connection.h>
#include <oac_tree_gui/nodeeditor/node_port.h>

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

namespace
{
const bool kEventWasHandled = true;
const bool kEventWasIgnored = false;
}  // namespace

namespace oac_tree_gui
{
NodeConnectionController::NodeConnectionController(QGraphicsScene* scene) : m_scene(scene)
{
  setParent(m_scene);
  m_scene->installEventFilter(this);
}

bool NodeConnectionController::eventFilter(QObject* object, QEvent* event)
{
  bool isProcessedEvent(false);

  if (auto mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent*>(event); mouseEvent)
  {
    if (event->type() == QEvent::GraphicsSceneMousePress)
    {
      isProcessedEvent = ProcessMousePress(mouseEvent);
    }
    else if (event->type() == QEvent::GraphicsSceneMouseMove)
    {
      isProcessedEvent = ProcessMouseMove(mouseEvent);
    }
    else if (event->type() == QEvent::GraphicsSceneMouseRelease)
    {
      isProcessedEvent = ProcessMouseRelease(mouseEvent);
    }
  }

  return isProcessedEvent ? kEventWasHandled : QObject::eventFilter(object, event);
}

NodePort* NodeConnectionController::FindPort(const QPointF& pos)
{
  QRectF area(0, 0, 4, 4);
  area.moveCenter(pos);

  for (auto item : m_scene->items(area))
  {
    if (auto port = dynamic_cast<NodePort*>(item); port)
    {
      return port;
    }
  }
  return nullptr;
}

bool NodeConnectionController::ProcessMousePress(QGraphicsSceneMouseEvent* event)
{
  if (!m_current_connection && event->button() == Qt::LeftButton)
  {
    if (auto port = FindPort(event->scenePos()); port)
    {
      emit OperationModeChangeRequest(kSimpleSelection);
      m_current_connection = new NodeConnection(m_scene);
      m_current_connection->setPort1(port);
      m_current_connection->setPos1(port->scenePos());
      m_current_connection->setPos2(event->scenePos());
      m_current_connection->updatePath();
      return kEventWasHandled;
    }
  }
  return kEventWasIgnored;
}

bool NodeConnectionController::ProcessMouseMove(QGraphicsSceneMouseEvent* event)
{
  if (m_current_connection)
  {
    m_current_connection->setPos2(event->scenePos());
    m_current_connection->updatePath();
    return kEventWasHandled;
  }
  return kEventWasIgnored;
}

bool NodeConnectionController::ProcessMouseRelease(QGraphicsSceneMouseEvent* event)
{
  if (m_current_connection && event->button() == Qt::LeftButton)
  {
    emit OperationModeChangeRequest(kRubberSelection);
    if (auto port2 = FindPort(event->scenePos()); port2)
    {
      auto port1 = m_current_connection->port1();
      if (port1->isConnectable(port2))
      {
        m_current_connection->setPort2(port2);
        m_current_connection->updatePath();

        // At this point we do not need NodeConnection object anymore.
        // It will be redrawn automatically, when the model process our request.

        auto parent = m_current_connection->parentView();
        auto child = m_current_connection->childView();
        ResetConnection();

        // Sending request for connection.
        emit connectionRequest(child, parent);
      }
    }
    ResetConnection();
    return kEventWasHandled;
  }
  return kEventWasIgnored;
}

void NodeConnectionController::ResetConnection()
{
  delete m_current_connection;
  m_current_connection = nullptr;
}

}  // namespace oac_tree_gui
