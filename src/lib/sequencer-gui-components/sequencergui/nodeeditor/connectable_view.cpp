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

#include "connectable_view.h"

#include <sequencergui/nodeeditor/connectable_instruction_adapter.h>
#include <sequencergui/nodeeditor/node_connection.h>
#include <sequencergui/nodeeditor/node_port.h>
#include <sequencergui/nodeeditor/position_strategy.h>
#include <sequencergui/nodeeditor/scene_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace
{
const int round_par = 5;

//! Returns rectangle to display ConnectableView label. Takes bounding box of a view as input
//! parameter.
QRectF GetLabelRectangle(const QRectF& rect)
{
  // take a rectangle which is a half in height of the view's rectangle
  QRectF result(rect.x(), rect.y(), rect.width(), rect.height() / 2);

  // slightly shrink it
  auto side_gap = mvvm::utils::UnitSize(0.25);
  auto top_gap = mvvm::utils::UnitSize(0.75);
  return result.marginsRemoved(
      QMarginsF(side_gap, top_gap, side_gap, 0.0));  // left, top, right, bottom
}

int GetSmallFontSize()
{
  static const int kSmallFontSize(mvvm::utils::AppFontSize() * 0.8);
  return kSmallFontSize;
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
  QFont serifFont("Monospace", GetSmallFontSize(), QFont::Normal);
  serifFont.setWordSpacing(-mvvm::utils::UnitSize(0.5));  // decrease word spacing
  painter->setFont(serifFont);

  QTextOption text_option(Qt::AlignHCenter | Qt::AlignTop);
  text_option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
  painter->drawText(GetLabelRectangle(boundingRect()), GetLabel(), text_option);

  if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus))
  {
    auto rect = QRectF(boundingRect().width() * 0.4, boundingRect().height() * 0.7,
                       mvvm::utils::UnitSize(1.75), mvvm::utils::UnitSize(1.75));
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
