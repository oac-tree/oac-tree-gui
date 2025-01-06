/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include <sequencergui/jobsystem/domain_events.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>

#include <sup/sequencer/workspace.h>

namespace sequencergui
{

WorkspaceSynchronizer::WorkspaceSynchronizer(WorkspaceItem* workspace_item,
                                             sup::sequencer::Workspace* domain_workspace)
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
}  // namespace sequencergui
