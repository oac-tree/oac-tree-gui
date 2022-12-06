/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "suppvmonitor/workspace_syncronizer.h"

#include <mvvm/model/model_utils.h>
#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/domain_workspace_builder.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/variable_item_transform_utils.h>
#include <sup/gui/dto/conversion_utils.h>
#include <sup/sequencer/workspace.h>
#include <suppvmonitor/monitor_model.h>
#include <suppvmonitor/sequencer_workspace_listener.h>
#include <suppvmonitor/workspace_event.h>
#include <suppvmonitor/workspace_item_controller.h>

namespace suppvmonitor
{

WorkspaceSyncronizer::WorkspaceSyncronizer(MonitorModel* model, QObject* parent)
    : QObject(parent)
    , m_workspace_listener(std::make_unique<SequencerWorkspaceListener>())
    , m_workspace_item_controller(std::make_unique<WorkspaceItemController>(model))
    , m_model(model)
{
  connect(m_workspace_listener.get(), &SequencerWorkspaceListener::VariabledUpdated, this,
          &WorkspaceSyncronizer::OnVariableUpdated, Qt::QueuedConnection);
}

WorkspaceSyncronizer::~WorkspaceSyncronizer() = default;

//! Creates domain workspace corresponding to WorkspaceItem and start listening.

void WorkspaceSyncronizer::OnSetupWorkspaceRequest()
{
  if (!GetWorkspaceItem())
  {
    throw sequencergui::LogicErrorException("No WorkspaceItem");
  }

  m_workspace = std::make_unique<sup::sequencer::Workspace>();

  m_workspace_builder = std::make_unique<sequencergui::DomainWorkspaceBuilder>();
  m_workspace_builder->PopulateDomainWorkspace(GetWorkspaceItem(), m_workspace.get());

  m_workspace_listener->StartListening(m_workspace.get());

  m_workspace->Setup();
}

void WorkspaceSyncronizer::OnVariableUpdated()
{
  auto event = m_workspace_listener->PopEvent();
  m_workspace_item_controller->ProcessDomainEvent(event);
}

sup::sequencer::Workspace* WorkspaceSyncronizer::GetWorkspace() const
{
  return m_workspace.get();
}

sequencergui::WorkspaceItem* WorkspaceSyncronizer::GetWorkspaceItem()
{
  return mvvm::utils::GetTopItem<sequencergui::WorkspaceItem>(m_model);
}

}  // namespace suppvmonitor
