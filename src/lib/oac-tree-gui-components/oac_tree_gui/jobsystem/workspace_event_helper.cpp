/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "workspace_event_helper.h"

#include <oac_tree_gui/jobsystem/domain_events.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/oac-tree/workspace.h>

namespace oac_tree_gui
{

bool AreMatchingWorkspaces(const WorkspaceItem& workspace_item,
                           const sup::oac_tree::Workspace& workspace)
{
  std::vector<std::string> variable_item_names;
  for (const auto variable_item : workspace_item.GetVariables())
  {
    variable_item_names.push_back(variable_item->GetName());
  }

  return variable_item_names == workspace.VariableNames();
}

void UpdateVariableFromEvent(const VariableUpdatedEvent& event, VariableItem& item)
{
  UpdateVariableFromEvent(event.value, event.connected, item);
}

void UpdateVariableFromEvent(const sup::dto::AnyValue& value, bool connected, VariableItem& item)
{
  if (connected && sup::dto::IsEmptyValue(value) && !item.IsAvailable())
  {
    item.SetIsAvailable(connected);
    return;
  }

  item.SetIsAvailable(connected);
  UpdateAnyValue(value, item);
}

}  // namespace oac_tree_gui
