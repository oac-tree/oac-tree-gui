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

#include "workspace_synchronizer.h"

#include "domain_workspace_listener.h"
#include "workspace_item_listener.h"
#include "workspace_monitor_helper.h"

#include <oac_tree_gui/jobsystem/domain_events.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <sup/oac-tree/workspace.h>

namespace oac_tree_gui
{

WorkspaceSynchronizer::WorkspaceSynchronizer(WorkspaceItem* workspace_item,
                                             sup::oac_tree::Workspace* domain_workspace)
    : m_workspace_item(workspace_item)
    , m_domain_workspace(domain_workspace)
    , m_domain_workspace_listener(
          std::make_unique<DomainWorkspaceListener>(m_domain_workspace, CreateUpdateGUICallback()))
    , m_workspace_item_listener(
          std::make_unique<WorkspaceItemListener>(m_workspace_item, m_domain_workspace))
{
}

WorkspaceSynchronizer::~WorkspaceSynchronizer()
{
  for (auto item : m_workspace_item->GetVariables())
  {
    item->SetIsAvailable(false);
  }
}

bool WorkspaceSynchronizer::IsEmptyQueue() const
{
  return m_domain_workspace_listener->GetEventCount() == 0;
}

std::function<void(const VariableUpdatedEvent&)> WorkspaceSynchronizer::CreateUpdateGUICallback()
    const
{
  auto result = [this](const VariableUpdatedEvent& event)
  { m_workspace_item_listener->ProcessEventFromDomain(event); };

  return result;
}
}  // namespace oac_tree_gui
