/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/nodeeditor/connectableview.h"

#include "sequencergui/nodeeditor/connectableinstructionadapter.h"
#include "sequencergui/nodeeditor/nodeconnection.h"
#include "sequencergui/nodeeditor/nodeport.h"
#include "sequencergui/nodeeditor/positionstrategy.h"
#include "sequencergui/nodeeditor/sceneutils.h"
#include "sequencergui/utils/styleutils.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace
{
const int round_par = 5;

//! Returns rectangle to display ConnectableView label. Takes bounding box of a view as input
//! parameter.
QRectF label_rectangle(const QRectF& rect)
{
  return QRectF(rect.x(), rect.y(), rect.width(), rect.height() / 4);
}

}  // namespace

namespace sequencergui
{
ConnectableView::ConnectableView(std::unique_ptr<ConnectableInstructionAdapter> item)
    : m_item(std::move(item))
{
  setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
  m_rect = ConnectableViewRectangle();

  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);

  UpdateViewFromItem();
  update();
}

void ConnectableView::SetPositionStrategy(std::unique_ptr<PositionStrategyInterface> strategy)
{
  m_strategy = std::move(strategy);
  SetupPorts();
}

ConnectableView::~ConnectableView() = default;

QRectF ConnectableView::boundingRect() const
{
  return m_rect;
}

void ConnectableView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                            QWidget* widget)
{
  Q_UNUSED(widget);

  static QPixmap pixmap(QString(":/icons/map-marker-outline.png"));

  painter->setRenderHint(QPainter::SmoothPixmapTransform);
  painter->setRenderHint(QPainter::Antialiasing);

  painter->setPen(Qt::gray);
  if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus))
  {
    painter->setPen(Qt::DashLine);
  }

  painter->setBrush(ConnectableViewGradient(GetColor(), boundingRect()));
  painter->drawRoundedRect(boundingRect(), round_par, round_par);

  painter->setPen(Qt::black);
  QFont serifFont("Monospace", 8, QFont::Normal);
  painter->setFont(serifFont);
  painter->drawText(label_rectangle(boundingRect()), Qt::AlignCenter, GetLabel());

  if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus))
  {
    auto rect = QRectF(boundingRect().width() * 0.4, boundingRect().height() * 0.7,
                       styleutils::UnitSize(1.75), styleutils::UnitSize(1.75));
    painter->drawPixmap(rect, pixmap, QRectF(0.0, 0.0, pixmap.width(), pixmap.height()));
  }
}

//! Connects children's output port to appropriate input port.

void ConnectableView::MakeChildConnected(ConnectableView* child_view)
{
  if (!child_view)
  {
    throw std::runtime_error("Error in ConnectableView: wrong child");
  }

  auto child_port = child_view->GetChildPort();
  if (!child_port)
  {
    return;
  }

  auto parent_port = GetParentPort();
  if (parent_port->isConnectable(child_port))
  {
    auto connection = new NodeConnection(scene());  // ownership to scene
    connection->setPort2(child_port);
    connection->setPort1(parent_port);
    connection->updatePath();
  }
}

//! Returns list of input ports of given

QList<ChildPort*> ConnectableView::GetChildPorts() const
{
  return GetPorts<ChildPort>();
}

ChildPort* ConnectableView::GetChildPort() const
{
  auto child_ports = GetChildPorts();
  return child_ports.empty() ? nullptr : child_ports.front();
}

ParentPort* ConnectableView::GetParentPort() const
{
  auto parent_ports = GetPorts<ParentPort>();
  return parent_ports.empty() ? nullptr : parent_ports.front();
}

ConnectableInstructionAdapter* ConnectableView::GetConnectableItem() const
{
  return m_item.get();
}

QList<NodeConnection*> ConnectableView::GetOutputConnections() const
{
  QList<NodeConnection*> result;
  if (auto parent_port = GetParentPort(); parent_port)
  {
    result.append(parent_port->connections());
  }
  return result;
}

void ConnectableView::UpdateItemFromView()
{
  m_item->SetX(x());
  m_item->SetY(y());
}

void ConnectableView::UpdateViewFromItem()
{
  if (m_block_view_update)
  {
    return;
  }

  setX(m_item->GetX());
  setY(m_item->GetY());
  if (auto child_port = GetChildPort(); child_port)
  {
    child_port->SetPortInfo(m_item->GetInputPorts().at(0));
  }
}

void ConnectableView::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  QGraphicsItem::mouseMoveEvent(event);
}

QVariant ConnectableView::itemChange(GraphicsItemChange change, const QVariant& value)
{
  if (change == ItemScenePositionHasChanged)
  {
    m_block_view_update = true;
    UpdateItemFromView();
    m_block_view_update = false;
  }
  return value;
}

//! Returns base color of this item.

QColor ConnectableView::GetColor() const
{
  return m_item ? m_item->GetColor() : QColor(Qt::red);
}

//! Returns label of this item.

QString ConnectableView::GetLabel() const
{
  return m_item ? m_item->GetDisplayName() : QString("Unnamed");
}

//! Init ports to connect.

void ConnectableView::SetupPorts()
{
  int portIndex{0};
  for (const auto& info : m_item->GetInputPorts())
  {
    auto inputPort = new ChildPort(this, info);
  }

  portIndex = 0;
  for (const auto& info : m_item->GetOutputPorts())
  {
    auto outputPort = new ParentPort(this, info);
  }

  if (m_strategy)
  {
    m_strategy->UpdatePosition(this);
  }
}

}  // namespace sequencergui
