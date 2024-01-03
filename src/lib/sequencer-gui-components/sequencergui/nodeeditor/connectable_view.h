/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_CONNECTABLE_VIEW_H_
#define SEQUENCERGUI_NODEEDITOR_CONNECTABLE_VIEW_H_

#include <QGraphicsItem>
#include <memory>

namespace sequencergui
{
class ConnectableInstructionAdapter;
class ChildPort;
class ParentPort;
class NodeConnection;
class PositionStrategyInterface;

//! Represents ConnectableItem on QGraphicsScene. Shown as a reactangle with rounded corners,
//! gradient, label, and set of input/output ports to connect.
//! In current design, ConnectableView can have only single output port and multiple input ports
//! of different type.

class ConnectableView : public QGraphicsItem
{
public:
  explicit ConnectableView(std::unique_ptr<ConnectableInstructionAdapter> item);
  ~ConnectableView() override;

  void SetPositionStrategy(std::unique_ptr<PositionStrategyInterface> strategy);

  QRectF boundingRect() const override;

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

  void MakeChildConnected(ConnectableView* child_view);

  QList<ChildPort*> GetChildPorts() const;

  ChildPort* GetChildPort() const;

  ParentPort* GetParentPort() const;

  ConnectableInstructionAdapter* GetConnectableItem() const;

  QList<NodeConnection*> GetOutputConnections() const;

  void UpdateItemFromView();

  void UpdateViewFromItem();

  template <typename T>
  QList<T*> GetPorts() const;

protected:
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
  QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
  QColor GetColor() const;
  QString GetLabel() const;
  void SetupPorts();

  QRectF m_rect;                                          //!< Bounding rectangle.
  std::unique_ptr<ConnectableInstructionAdapter> m_item;  //!< Underlying item of this view.
  bool m_block_view_update{false};
  std::unique_ptr<PositionStrategyInterface> m_strategy;
};

//! Return list of ports of required type.

template <typename T>
QList<T*> ConnectableView::GetPorts() const
{
  QList<T*> result;
  for (auto child : childItems())
  {
    if (auto casted = dynamic_cast<T*>(child); casted)
    {
      result.push_back(casted);
    }
  }
  return result;
}

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_CONNECTABLE_VIEW_H_
