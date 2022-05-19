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

#include "sequencergui/nodeeditor/positionstrategy.h"

#include "sequencergui/nodeeditor/connectableview.h"
#include "sequencergui/nodeeditor/nodeport.h"
#include "sequencergui/utils/styleutils.h"

namespace
{
double port_radius()
{
  return sequencergui::styleutils::UnitSize() * 0.4;
}
}  // namespace

namespace sequencergui
{
void LeftRightPositionStrategy::UpdatePosition(ConnectableView *view)
{
  int index{0};
  for (auto port : view->GetPorts<ChildPort>())
  {
    SetPositionToLeft(view->boundingRect(), port, index++);
  }

  index = 0;
  for (auto port : view->GetPorts<ParentPort>())
  {
    SetPositionToRight(view->boundingRect(), port, index);
  }
}

//! Initializes port position and labels to be on the left side of bounding rectangle of
//! ConnectableView. This port represents an input of a child (i.e. the port is located on an
//! instruction playing the role of a child).

void LeftRightPositionStrategy::SetPositionToLeft(const QRectF &rect, NodePort *port, int index)
{
  auto label = port->GetLabel();

  // The label will be located on the right side of the port.
  label->setPlainText(port->portType().toLower());
  label->setPos(port_radius() * 1.5, -label->boundingRect().height() / 2);

  // Port itself will be located on the left side of the connectable view.
  port->setPos(0.0, rect.height() * 0.6 + index * 4 * port_radius());
}

//! Initializes port position and labels to be on the right side of bounding rectangle of
//! ConnectableView. This port represents an output of a parennt (i.e. the port is located on an
//! instruction playing the role of a parent).

void LeftRightPositionStrategy::SetPositionToRight(const QRectF &rect, NodePort *port, int index)
{
  auto label = port->GetLabel();

  port->GetLabel()->setPlainText("out");
  label->setPos(-port_radius() * 1.5 - label->boundingRect().width(),
                -label->boundingRect().height() / 2);

  // Port itself will be located on the right side of the connectable view.
  port->setPos(rect.width(), rect.height() * 0.4 + index * 4 * port_radius());
}

// ----------------------------------------------------------------------------

void TopBottomPositionStrategy::UpdatePosition(ConnectableView *view)
{
  int index{0};
  for (auto port : view->GetPorts<ChildPort>())
  {
    SetPositionToTop(view->boundingRect(), port, index++);
  }

  index = 0;
  for (auto port : view->GetPorts<ParentPort>())
  {
    SetPositionToBottom(view->boundingRect(), port, index);
  }
}

//! Initializes port position and labels to be on the left side of bounding rectangle of
//! ConnectableView. This port represents an input of a child (i.e. the port is located on an
//! instruction playing the role of a child).

void TopBottomPositionStrategy::SetPositionToTop(const QRectF &rect, NodePort *port, int /*index*/)
{
  port->GetLabel()->setVisible(false);
  port->setPos(rect.width() / 2., 0.0);
}

//! Initializes port position and labels to be on the right side of bounding rectangle of
//! ConnectableView. This port represents an output of a parennt (i.e. the port is located on an
//! instruction playing the role of a parent).

void TopBottomPositionStrategy::SetPositionToBottom(const QRectF &rect, NodePort *port, int index)
{
  port->GetLabel()->setVisible(false);
  port->setPos(rect.width() / 2, rect.height());
}

}  // namespace sequencergui
