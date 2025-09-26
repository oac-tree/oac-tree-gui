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

#include "test_node_editor_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/instruction_item.h>

#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>
#include <mvvm/nodeeditor/i_node_port.h>
#include <mvvm/nodeeditor/node_port_shape.h>

namespace oac_tree_gui::test
{

namespace
{

/**
 * @brief Returns scene position of the port with given direction.
 */
QPointF GetPortScenePosition(const mvvm::ConnectableShape &shape, mvvm::PortDirection direction)
{
  auto port_shapes = mvvm::utils::CastItems<mvvm::NodePortShape>(shape.childItems());
  for (auto port_shape : port_shapes)
  {
    if (port_shape->GetNodePort()->GetPortDirection() == direction)
    {
      return port_shape->scenePos();
    }
  }
  throw RuntimeException("Input port not found");
}

}  // namespace

std::vector<InstructionItem *> FindSceneInstructions(const QGraphicsScene &scene)
{
  std::vector<InstructionItem *> result;
  for (auto shape : mvvm::GetShapes<mvvm::ConnectableShape>(scene))
  {
    result.push_back(mvvm::GetUnderlyingItem<InstructionItem>(shape));
  }
  return result;
}

QPointF GetInputPortScenePosition(const mvvm::ConnectableShape &shape)
{
  return GetPortScenePosition(shape, mvvm::PortDirection::kInput);
}

QPointF GetOutputPortScenePosition(const mvvm::ConnectableShape &shape)
{
  return GetPortScenePosition(shape, mvvm::PortDirection::kOutput);
}

}  // namespace oac_tree_gui::test
