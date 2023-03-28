/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "suppvmonitor/workspace_synchronizer.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/domain_workspace_builder.h>
#include <sequencergui/transform/variable_transform_helper.h>
#include <suppvmonitor/monitor_model.h>
#include <suppvmonitor/sequencer_workspace_listener.h>
#include <suppvmonitor/workspace_event.h>
#include <suppvmonitor/workspace_item_controller.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/model/sessionmodel.h>

#include <sup/gui/core/exceptions.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/sequencer/workspace.h>

#include <algorithm>

namespace
{

//! Validates that workspace
void ValidateWorkspaces(const sequencergui::WorkspaceItem& workspace_item,
                        const sup::sequencer::Workspace& domain_workspace)
{
  if (domain_workspace.VariableNames().empty())
  {
    throw sup::gui::LogicErrorException("Workspace doesn't not contain variables");
  }

  std::vector<std::string> variable_item_names;
  for (const auto variable_item : workspace_item.GetVariables())
  {
    variable_item_names.push_back(variable_item->GetName());
  }
  auto domain_names = domain_workspace.VariableNames();

  // sequencer stores everything in a map, sorting is necessary for comparison
  std::sort(domain_names.begin(), domain_names.end());
  std::sort(variable_item_names.begin(), variable_item_names.end());

  if (domain_names != variable_item_names)
  {
    throw sup::gui::LogicErrorException("Workspace and WorkspaceItem have different variable set");
  }
}

}  // namespace

namespace suppvmonitor
{

WorkspaceSynchronizer::WorkspaceSynchronizer(sequencergui::WorkspaceItem* workspace_item,
                                             sup::sequencer::Workspace* domain_workspace,
                                             QObject* parent)
    : QObject(parent)
    , m_workspace_listener(std::make_unique<SequencerWorkspaceListener>())
    , m_workspace_item_controller(std::make_unique<WorkspaceItemController>(workspace_item))
    , m_workspace(domain_workspace)
    , m_workspace_item(workspace_item)
{
  connect(m_workspace_listener.get(), &SequencerWorkspaceListener::VariabledUpdated, this,
          &WorkspaceSynchronizer::OnDomainVariableUpdated, Qt::QueuedConnection);

  m_workspace_item_controller->SetCallback([this](const auto& event)
                                           { OnWorkspaceEventFromGUI(event); });
}

WorkspaceSynchronizer::~WorkspaceSynchronizer() = default;

//! Creates domain workspace corresponding to WorkspaceItem and start listening.

void WorkspaceSynchronizer::Start()
{
  ValidateWorkspaces(*m_workspace_item, *m_workspace);

  // For the moment we assume that the Workspace has been already setup. It means that we have to
  // manually propagate initial values from the domain to WorkspaceItem, and only then start
  // listening the domain.

  UpdateValuesFromDomain();

  m_workspace_listener->StartListening(GetWorkspace());
}

sup::sequencer::Workspace* WorkspaceSynchronizer::GetWorkspace() const
{
  return m_workspace;
}

sequencergui::WorkspaceItem* WorkspaceSynchronizer::GetWorkspaceItem() const
{
  return m_workspace_item;
}

//! Updates all values in WorkspaceItem from the domain's workspace.

void WorkspaceSynchronizer::UpdateValuesFromDomain()
{
  for (const auto& name : m_workspace->VariableNames())
  {
    auto variable = m_workspace->GetVariable(name);

    sup::dto::AnyValue anyvalue;
    variable->GetValue(anyvalue);
    WorkspaceEvent event{name, anyvalue, variable->IsAvailable()};
    m_workspace_item_controller->ProcessEventFromDomain(event);
  }
}

void WorkspaceSynchronizer::OnDomainVariableUpdated()
{
  auto event = m_workspace_listener->PopEvent();
  m_workspace_item_controller->ProcessEventFromDomain(event);
}

void WorkspaceSynchronizer::OnWorkspaceEventFromGUI(const WorkspaceEvent& event)
{
  GetWorkspace()->SetValue(event.variable_name, event.value);
}

}  // namespace suppvmonitor
