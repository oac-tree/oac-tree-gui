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

#include "node_graphics_scene_v2.h"

#include <oac_tree_gui/components/drag_and_drop_helper.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>

#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

namespace oac_tree_gui
{

namespace
{

QRectF GetDefaultSceneRect()
{
  return {oac_tree_gui::GetGraphicsViewportOrigin(), oac_tree_gui::GetGraphicsViewportSize()};
}

//! Returns name encoded in the drop event.
std::string GetEncodedName(QGraphicsSceneDragDropEvent *event)
{
  return oac_tree_gui::GetNewInstructionType(event->mimeData());
}

//! Returns domain type from the drop event. If domain_type can't be deduced from the event data,
//! will return an empty string.
std::string GetRequestedDomainType(QGraphicsSceneDragDropEvent *event)
{
  return GetEncodedName(event);
}
}  // namespace

NodeGraphicsSceneV2::NodeGraphicsSceneV2(QObject *parent_object) : QGraphicsScene(parent_object)
{
  setSceneRect(GetDefaultSceneRect());
}

void NodeGraphicsSceneV2::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
  if (event->mimeData()->hasFormat(kNewInstructionMimeType))
  {
    event->accept();
  }
  else
  {
    event->ignore();
  }
}

void NodeGraphicsSceneV2::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  qDebug() << "1.1";
  if (auto domain_type = GetRequestedDomainType(event); !domain_type.empty())
  {
    qDebug() << "1.2";
    dropInstructionRequested(QString::fromStdString(domain_type),
                             GetNodeDropPosition(event->scenePos()));
  }
}

}  // namespace oac_tree_gui
