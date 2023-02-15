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
#include <sequencergui/model/domain_workspace_builder.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/variable_item_transform_utils.h>
#include <suppvmonitor/monitor_model.h>
#include <suppvmonitor/sequencer_workspace_listener.h>
#include <suppvmonitor/workspace_event.h>
#include <suppvmonitor/workspace_item_controller.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/model/sessionmodel.h>

#include <sup/gui/dto/conversion_utils.h>
#include <sup/sequencer/workspace.h>

namespace suppvmonitor
{

WorkspaceSynchronizer::WorkspaceSynchronizer(MonitorModel* model, QObject* parent)
    : QObject(parent)
    , m_workspace_listener(std::make_unique<SequencerWorkspaceListener>())
    , m_workspace_item_controller(std::make_unique<WorkspaceItemController>(model))
    , m_model(model)
{
  connect(m_workspace_listener.get(), &SequencerWorkspaceListener::VariabledUpdated, this,
          &WorkspaceSynchronizer::OnDomainVariableUpdated, Qt::QueuedConnection);

  m_workspace_item_controller->SetCallback([this](const auto& event)
                                           { OnWorkspaceEventFromGUI(event); });
}

WorkspaceSynchronizer::WorkspaceSynchronizer(sequencergui::WorkspaceItem* workspace_item,
                                             sup::sequencer::Workspace* domain_workspace,
                                             QObject* parent)
    : QObject(parent), m_domain_workspace(domain_workspace), m_workspace_item(workspace_item)
{
  connect(m_workspace_listener.get(), &SequencerWorkspaceListener::VariabledUpdated, this,
          &WorkspaceSynchronizer::OnDomainVariableUpdated, Qt::QueuedConnection);

  m_workspace_item_controller->SetCallback([this](const auto& event)
                                           { OnWorkspaceEventFromGUI(event); });
}

WorkspaceSynchronizer::~WorkspaceSynchronizer() = default;

//! Creates domain workspace corresponding to WorkspaceItem and start listening.

void WorkspaceSynchronizer::OnSetupWorkspaceRequest()
{
  if (!GetWorkspaceItem())
  {
    throw sequencergui::LogicErrorException("No WorkspaceItem");
  }

  m_workspace = std::make_unique<sup::sequencer::Workspace>();

  m_workspace_builder = std::make_unique<sequencergui::DomainWorkspaceBuilder>();
  m_workspace_builder->PopulateDomainWorkspace(GetWorkspaceItem(), m_workspace.get());

  m_workspace->Setup();

  m_workspace_listener->StartListening(m_workspace.get());

  // FIXME implement setting of domain initial values here, block OnDomainVariableUpdated
  // notifications
}

sup::sequencer::Workspace* WorkspaceSynchronizer::GetWorkspace() const
{
  return m_workspace.get();
}

void WorkspaceSynchronizer::OnDomainVariableUpdated()
{
  auto event = m_workspace_listener->PopEvent();
  m_workspace_item_controller->ProcessEventFromDomain(event);
}

void WorkspaceSynchronizer::OnWorkspaceEventFromGUI(const WorkspaceEvent& event)
{
  m_workspace->SetValue(event.m_variable_name, event.m_value);
}

sequencergui::WorkspaceItem* WorkspaceSynchronizer::GetWorkspaceItem()
{
  return mvvm::utils::GetTopItem<sequencergui::WorkspaceItem>(m_model);
}

}  // namespace suppvmonitor
