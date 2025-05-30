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

#ifndef OAC_TREE_GUI_NODEEDITOR_POSITION_STRATEGY_H_
#define OAC_TREE_GUI_NODEEDITOR_POSITION_STRATEGY_H_

class QRectF;

namespace oac_tree_gui
{

class ConnectableView;
class NodePort;

class PositionStrategyInterface
{
public:
  virtual ~PositionStrategyInterface() = default;
  virtual void UpdatePosition(ConnectableView* view) = 0;
};

//! Rearange port position on ConnectableView according to the left-to-right behavioral tree
//! topology.

class LeftRightPositionStrategy : public PositionStrategyInterface
{
public:
  void UpdatePosition(ConnectableView* view) override;

  void SetPositionToLeft(const QRectF& rect, NodePort* port, int index);

  void SetPositionToRight(const QRectF& rect, NodePort* port, int index);
};

//! Rearange port position on ConnectableView according to the top-to-bottom behavioral tree
//! topology.

class TopBottomPositionStrategy : public PositionStrategyInterface
{
public:
  void UpdatePosition(ConnectableView* view) override;

  void SetPositionToTop(const QRectF& rect, NodePort* port, int index);

  void SetPositionToBottom(const QRectF& rect, NodePort* port, int index);
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_POSITION_STRATEGY_H_
