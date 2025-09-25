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

#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/components/drag_and_drop_helper.h>

#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

namespace oac_tree_gui
{

NodeGraphicsSceneV2::NodeGraphicsSceneV2(QObject *parent_object) : QGraphicsScene(parent_object) {}

// std::vector<InstructionItem *> NodeGraphicsSceneV2::GetSelectedInstructions() const
// {
//   std::vector<mvvm::SessionItem *> result;
//   for (auto shape : mvvm::GetSelectedShapes<mvvm::ConnectableShape>(*this))
//   {
//     result.push_back(mvvm::GetUnderlyingItem(shape));
//   }

//   return mvvm::utils::CastItems<InstructionItem>(result);
// }

// void NodeGraphicsSceneV2::SetSelectedInstructions(const std::vector<InstructionItem *> &to_select)
// {
// }

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
  (void) event;

}

}  // namespace oac_tree_gui
