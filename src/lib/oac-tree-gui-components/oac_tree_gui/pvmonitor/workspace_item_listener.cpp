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

#include "workspace_item_listener.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/pvmonitor/workspace_monitor_helper.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/gui/model/anyvalue_conversion_utils.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/signals/model_listener.h>

#include <sup/oac-tree/workspace.h>

namespace oac_tree_gui
{

WorkspaceItemListener::WorkspaceItemListener(WorkspaceItem* workspace_item,
                                             sup::oac_tree::Workspace* domain_workspace)
    : m_workspace_item(workspace_item), m_domain_workspace(domain_workspace)
{
  ValidateWorkspaces();

  m_block_update_to_domain.resize(m_workspace_item->GetVariableCount(), false);

  m_index_to_item = m_workspace_item->GetVariables();

  std::size_t index{0};
  for (auto item : m_workspace_item->GetVariables())
  {
    m_item_to_index[item] = index;
    ++index;
  }

  m_listener = std::make_unique<mvvm::ModelListener>(workspace_item->GetModel());
  m_listener->Connect<mvvm::DataChangedEvent>(this, &WorkspaceItemListener::OnDataChangedEvent);
  m_listener->Connect<mvvm::ItemInsertedEvent>(this, &WorkspaceItemListener::OnItemInsertedEvent);
}

WorkspaceItemListener::~WorkspaceItemListener() = default;

void WorkspaceItemListener::ProcessEventFromDomain(const VariableUpdatedEvent& event)
{
  // This method is called from the GUI thread. So we can expect, that we will eventually reach the
  // end of this method and unblock update to the domain. Thanks to the blocking flag, the
  // ProcessEventToDomain method will not trigger circular updates back to the domain.

  m_block_update_to_domain[event.index] = true;

  auto item = m_index_to_item[event.index];
  UpdateVariableFromEvent(event.value, event.connected, *item);

  m_block_update_to_domain[event.index] = false;
}

void WorkspaceItemListener::ValidateWorkspaces()
{
  if (!m_domain_workspace)
  {
    throw RuntimeException("Not initialised workspace");
  }

  if (!m_workspace_item)
  {
    throw RuntimeException("Not initialised workspace item");
  }

  if (!AreMatchingWorkspaces(*m_workspace_item, *m_domain_workspace))
  {
    throw RuntimeException("Domain and GUI workspace do not match");
  }
}

void WorkspaceItemListener::OnDataChangedEvent(const mvvm::DataChangedEvent& event)
{
  // finding VariableItem which is a parent of the item with changed data
  for (auto variable_item : m_workspace_item->GetVariables())
  {
    if (mvvm::utils::IsItemAncestor(event.item, variable_item))
    {
      ProcessEventToDomain(variable_item);
      break;
    }
  }
}

void WorkspaceItemListener::OnItemInsertedEvent(const mvvm::ItemInsertedEvent& event)
{
  // If parent is VariableItem, then insert event denotes that AnyValueItem has been regenerated.
  if (auto variable_item = dynamic_cast<VariableItem*>(event.item); variable_item)
  {
    ProcessEventToDomain(variable_item);
  }
}

void WorkspaceItemListener::ProcessEventToDomain(VariableItem* variable_item)
{
  // do not send an event if it was initially triggered from the domain
  if (m_block_update_to_domain[m_item_to_index[variable_item]])
  {
    return;
  }

  if (!m_domain_workspace->IsSuccessfullySetup())
  {
    throw RuntimeException("Attempt to propagate changes to the domain workspace without setup");
  }

  m_domain_workspace->SetValue(variable_item->GetName(), GetAnyValue(*variable_item));
}

}  // namespace oac_tree_gui
