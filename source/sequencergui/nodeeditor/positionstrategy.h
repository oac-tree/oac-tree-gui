/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_POSITIONSTRATEGY_H
#define SEQUENCERGUI_NODEEDITOR_POSITIONSTRATEGY_H

#include <functional>

class QRectF;

namespace sequi
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

}  // namespace sequi

#endif  // SEQUENCERGUI_NODEEDITOR_PORTINFO_H
