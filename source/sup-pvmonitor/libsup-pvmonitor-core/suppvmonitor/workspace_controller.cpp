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

#include "suppvmonitor/workspace_controller.h"

#include <sup/sequencer/workspace.h>
#include <suppvmonitor/monitor_model.h>
#include <suppvmonitor/sequencer_workspace_listener.h>

#include <QDebug>

namespace suppvmonitor
{

WorkspaceController::WorkspaceController(MonitorModel* model, QObject* parent)
    : QObject(parent)
    , m_workspace_listener(std::make_unique<SequencerWorkspaceListener>())
    , m_model(model)
{
}

WorkspaceController::~WorkspaceController() = default;

void WorkspaceController::OnSetupWorkspaceRequest()
{
  qDebug() << "OnSetupWorkspaceRequest";
  m_workspace = std::make_unique<sup::sequencer::Workspace>();

  m_workspace_listener->StartListening(m_workspace.get());
}

void WorkspaceController::ProcessVariable() {}

}  // namespace suppvmonitor
