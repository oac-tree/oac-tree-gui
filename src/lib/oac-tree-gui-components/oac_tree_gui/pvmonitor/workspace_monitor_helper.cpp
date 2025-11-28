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

#include "workspace_monitor_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/jobsystem/domain_events.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/domain_workspace_builder.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/workspace.h>

namespace
{
//! Returns vector of attributes which should be marked as readonly when workspace is running.
std::vector<std::string> GetVariableReadonlyAttributesWhenRunning()
{
  return {oac_tree_gui::domainconstants::kChannelAttribute,
          oac_tree_gui::domainconstants::kNameAttribute};
}

}  // namespace

namespace oac_tree_gui
{

void PopulateDomainWorkspace(const WorkspaceItem& item, workspace_t& workspace)
{
  DomainWorkspaceBuilder builder;
  builder.PopulateDomainWorkspace(&item, &workspace);
}

void UpdateVariableEditableProperty(bool is_running, WorkspaceItem& item)
{
  static const auto attributes = GetVariableReadonlyAttributesWhenRunning();

  for (auto variable : item.GetVariables())
  {
    for (const auto& attr : attributes)
    {
      if (mvvm::utils::HasTag(*variable, attr))
      {
        (void)variable->GetItem(attr)->SetEditable(!is_running);
      }
    }
  }
}

void SetupNewVariable(VariableItem* item, int total_variable_count)
{
  if (item == nullptr)
  {
    return;
  }

  item->SetName("var" + std::to_string(total_variable_count));

  // Normally, we set scalar AnyValue to any VariableItem added to the WorkspaceItem. If user wants
  // something else, he has to start AnyValueEditor.

  if (item->GetType() == domainconstants::kPvAccessClientVariableType)
  {
    // PvAccessClient gets it's value from the network. Nethertheless, we
    // set empty AnyValue to make it look the same in the editor, as others.
    // Editor call will be disallowed.
    SetAnyValue(sup::dto::AnyValue(), *item);
  }
  else
  {
    SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *item);
  }
  if (auto available_item = GetIsAvailableItem(*item); available_item)
  {
    (void)available_item->SetVisible(false);
  }
  (void)item->GetAnyValueItem()->SetDisplayName(itemconstants::kAnyValueDefaultDisplayName);
}

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
