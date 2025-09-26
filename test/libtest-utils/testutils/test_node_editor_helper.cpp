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

#include <oac_tree_gui/model/instruction_item.h>

#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>

namespace oac_tree_gui::test
{

std::vector<InstructionItem *> FindSceneInstructions(const QGraphicsScene& scene)
{
  std::vector<InstructionItem *> result;
  for (auto shape : mvvm::GetShapes<mvvm::ConnectableShape>(scene))
  {
    result.push_back(mvvm::GetUnderlyingItem<InstructionItem>(shape));
  }
  return result;
}

}  // namespace oac_tree_gui::test
