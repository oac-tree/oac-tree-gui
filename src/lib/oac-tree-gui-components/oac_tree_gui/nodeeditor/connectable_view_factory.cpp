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

#include "connectable_view_factory.h"

#include <oac_tree_gui/components/component_helper.h>
#include <oac_tree_gui/nodeeditor/connectable_instruction_adapter.h>
#include <oac_tree_gui/nodeeditor/connectable_view.h>
#include <oac_tree_gui/nodeeditor/position_strategy.h>

namespace oac_tree_gui
{

ConnectableViewFactory::ConnectableViewFactory() {}

ConnectableViewFactory::~ConnectableViewFactory() = default;

std::unique_ptr<ConnectableView> ConnectableViewFactory::CreateView(InstructionItem *item)
{
  auto adapter = std::make_unique<ConnectableInstructionAdapter>(item);
  auto result = std::make_unique<ConnectableView>(std::move(adapter));

  if (GetBehaviorTreeStyle() == BehaviorTreeStyle::kTopToBottom)
  {
    result->SetPositionStrategy(std::make_unique<TopBottomPositionStrategy>());
  }
  else
  {
    result->SetPositionStrategy(std::make_unique<LeftRightPositionStrategy>());
  }
  return result;
}

}  // namespace oac_tree_gui
