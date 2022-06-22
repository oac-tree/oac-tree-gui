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

#ifndef SEQUENCERGUI_NODEEDITOR_POSITION_STRATEGY_H_
#define SEQUENCERGUI_NODEEDITOR_POSITION_STRATEGY_H_

#include <functional>

class QRectF;

namespace sequencergui
{

class ConnectableView;
class NodePort;

class PositionStrategyInterface
{
public:
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

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_POSITION_STRATEGY_H_
