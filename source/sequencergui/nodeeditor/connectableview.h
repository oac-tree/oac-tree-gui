/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_CONNECTABLEVIEW_H
#define SEQUENCERGUI_NODEEDITOR_CONNECTABLEVIEW_H

#include <QGraphicsItem>
#include <memory>

namespace sequi
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
  ConnectableView(std::unique_ptr<ConnectableInstructionAdapter> item);
  ~ConnectableView() override;

  void SetPositionStrategy(std::unique_ptr<PositionStrategyInterface> strategy);

  QRectF boundingRect() const override;

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) override;

  virtual void makeChildConnected(ConnectableView* childView);

  QList<ChildPort*> childPorts() const;

  ChildPort* childPort() const;

  ParentPort* parentPort() const;

  ConnectableInstructionAdapter* connectableItem() const;

  QList<NodeConnection*> outputConnections() const;

  void updateItemFromView();

  void updateViewFromItem();

  template <typename T>
  QList<T*> ports() const;

protected:
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
  QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
  QColor color() const;
  QString label() const;
  void SetupPorts();

  QRectF m_rect;                                          //!< Bounding rectangle.
  std::unique_ptr<ConnectableInstructionAdapter> m_item;  //!< Underlying item of this view.
  bool m_block_view_update{false};
  std::unique_ptr<PositionStrategyInterface> m_strategy;
};

//! Return list of ports of required type.

template <typename T>
QList<T*> ConnectableView::ports() const
{
  QList<T*> result;
  for (auto child : childItems())
    if (auto casted = dynamic_cast<T*>(child); casted)
      result.push_back(casted);
  return result;
}

}  // namespace sequi

#endif  // SEQUENCERGUI_NODEEDITOR_CONNECTABLEVIEW_H
