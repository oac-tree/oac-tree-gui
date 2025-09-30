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

#include "connectable_shape_factory.h"

#include "connectable_instruction_adapter.h"

#include <oac_tree_gui/model/instruction_item.h>

#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/top_bottom_port_position_strategy.h>

namespace oac_tree_gui
{

std::unique_ptr<mvvm::ConnectableShape> ConnectableShapeFactory::CreateShape(
    mvvm::SessionItem *item) const
{
  auto adapter =
      std::make_unique<ConnectableInstructionAdapter>(dynamic_cast<InstructionItem *>(item));
  auto position_strategy = std::make_unique<mvvm::TopBottomPortPositionStrategy>();
  return std::make_unique<mvvm::ConnectableShape>(std::move(adapter), std::move(position_strategy));
}

}  // namespace oac_tree_gui
