/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include "sequencer_workspace_listener.h"
#include "workspace_event.h"
#include "workspace_item_controller.h"
#include "workspace_monitor_helper.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>

#include <sup/sequencer/workspace.h>

namespace
{

/**
 * @brief Checks that domain and GUI workspaces are related.
 *
 * Number of variables and their names should coincide.
 */
void ValidateWorkspaces(const sequencergui::WorkspaceItem* workspace_item,
                        const sup::sequencer::Workspace* domain_workspace)
{
  if (!workspace_item)
  {
    throw sequencergui::LogicErrorException("Uninitialized workspace item");
  }

  if (!domain_workspace)
  {
    throw sequencergui::LogicErrorException("Uninitialized domain workspace");
  }

  if (domain_workspace->VariableNames().empty())
  {
    throw sequencergui::LogicErrorException("Workspace doesn't contain variables");
  }

  std::vector<std::string> variable_item_names;
  for (const auto variable_item : workspace_item->GetVariables())
  {
    variable_item_names.push_back(variable_item->GetName());
  }
  auto domain_names = domain_workspace->VariableNames();

  // sequencer stores everything in a map, sorting is necessary for comparison
  std::sort(domain_names.begin(), domain_names.end());
  std::sort(variable_item_names.begin(), variable_item_names.end());

  if (domain_names != variable_item_names)
  {
    throw sequencergui::LogicErrorException(
        "Workspace and WorkspaceItem have different variable set");
  }
}

}  // namespace

namespace sequencergui
{

WorkspaceSynchronizer::WorkspaceSynchronizer(sup::sequencer::Workspace* domain_workspace,
                                             QObject* parent)
    : QObject(parent)
    , m_workspace_listener(std::make_unique<SequencerWorkspaceListener>(domain_workspace))
    , m_workspace(domain_workspace)
{
  if (domain_workspace->IsSuccessfullySetup())
  {
    throw RuntimeException("Domain workspace has already been set up.");
  }

  connect(m_workspace_listener.get(), &SequencerWorkspaceListener::VariabledUpdated, this,
          &WorkspaceSynchronizer::OnDomainVariableUpdated, Qt::QueuedConnection);

  m_workspace_listener->StartListening();
}

WorkspaceSynchronizer::WorkspaceSynchronizer(WorkspaceItem* workspace_item,
                                             sup::sequencer::Workspace* domain_workspace,
                                             QObject* parent)
    : WorkspaceSynchronizer(domain_workspace, parent)
{
  SetWorkspaceItem(workspace_item);
}

void WorkspaceSynchronizer::SetWorkspaceItem(WorkspaceItem* workspace_item)
{
  m_workspace_item_controller = std::make_unique<WorkspaceItemController>(workspace_item);
  m_workspace_item = workspace_item;

  m_workspace_item_controller->SetCallback([this](const auto& event)
                                           { OnWorkspaceEventFromGUI(event); });
}

bool WorkspaceSynchronizer::HasStarted() const
{
  return m_started;
}

WorkspaceSynchronizer::~WorkspaceSynchronizer() = default;

//! Starts synchronization between domain Workspace and GUI's WorkspaceItem.

void WorkspaceSynchronizer::Start()
{
  if (m_started)
  {
    throw LogicErrorException("Synchronizer has been already started");
  }

  ValidateWorkspaces(m_workspace_item, m_workspace);

  if (!m_workspace->IsSuccessfullySetup())
  {
    m_workspace->Setup();
  }

  UpdateVariableEditableProperty(true, *GetWorkspaceItem());

  m_started = true;
}

void WorkspaceSynchronizer::Shutdown()
{
  UpdateVariableEditableProperty(false, *GetWorkspaceItem());

  m_workspace->Teardown();

  for (auto item : GetWorkspaceItem()->GetVariables())
  {
    item->SetIsAvailable(false);
  }
}

sup::sequencer::Workspace* WorkspaceSynchronizer::GetWorkspace() const
{
  return m_workspace;
}

WorkspaceItem* WorkspaceSynchronizer::GetWorkspaceItem() const
{
  return m_workspace_item;
}

void WorkspaceSynchronizer::OnDomainVariableUpdated()
{
  auto event = m_workspace_listener->PopEvent();

  // Handling the case when the domain was set and has started to receive callbacks while
  // WorkspaceItem wasn't set yet. Since this method is connected to the GUI thread via queued
  // connection, no data race is expected. It is safe just to check if synchronizer has been already
  // started.

  if (m_started)
  {
    m_workspace_item_controller->ProcessEventFromDomain(event);
  }
}

void WorkspaceSynchronizer::OnWorkspaceEventFromGUI(const WorkspaceEvent& event)
{
  if (m_started)
  {
    GetWorkspace()->SetValue(event.variable_name, event.value);
  }
}

}  // namespace sequencergui
