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

#ifndef OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_VIEW_H_
#define OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_VIEW_H_

#include <QGraphicsItem>
#include <memory>

namespace oac_tree_gui
{

class ConnectableInstructionAdapter;
class ChildPort;
class ParentPort;
class NodeConnection;
class PositionStrategyInterface;

/**
 * @brief The ConnectableView class represents ConnectableItem on the graphics scene.
 *
 * Shown as a rectangle with rounded corners, gradient, label, and set of input/output ports to
 * connect. In the current design, ConnectableView can have only a single output port and multiple input
 * ports of different types.
 *
 * Coordinates (0, 0) corresponds to the upper left corner of the bounding box.
 */
class ConnectableView : public QGraphicsItem
{
public:
  explicit ConnectableView(std::unique_ptr<ConnectableInstructionAdapter> item);
  ~ConnectableView() override;

  void SetPositionStrategy(std::unique_ptr<PositionStrategyInterface> strategy);

  QRectF boundingRect() const override;

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

  /**
   * @brief  Connects children's output port to appropriate input port.
   */
  void MakeChildConnected(ConnectableView* child_view);

  /**
   * @brief Returns list of input ports of given
   */
  QList<ChildPort*> GetChildPorts() const;

  ChildPort* GetChildPort() const;

  ParentPort* GetParentPort() const;

  ConnectableInstructionAdapter* GetConnectableItem() const;

  QList<NodeConnection*> GetOutputConnections() const;

  void UpdateItemFromView();

  void UpdateViewFromItem();

  /**
   * @brief Returns list of ports of required type.
   */
  template <typename T>
  QList<T*> GetPorts() const;

protected:
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
  QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
  /**
   * @brief Return base color of this item.
   */
  QColor GetColor() const;

  /**
   * @brief Returns node main text label.
   */
  QString GetLabel() const;

  /**
   * @brief Initial setup of all ports.
   */
  void SetupPorts();

  QRectF m_rect;                                          //!< Bounding rectangle.
  std::unique_ptr<ConnectableInstructionAdapter> m_item;  //!< Underlying item of this view.
  bool m_block_view_update{false};
  bool m_block_item_update{false};
  std::unique_ptr<PositionStrategyInterface> m_strategy;
};

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

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_VIEW_H_
