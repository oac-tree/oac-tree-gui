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

#include "monitor_widget.h"

#include "monitor_widget_actions.h"

#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/monitor_model.h>
#include <sequencergui/pvmonitor/workspace_monitor_helper.h>
#include <sequencergui/pvmonitor/workspace_synchronizer.h>
#include <sequencergui/views/composer/workspace_editor_widget.h>
#include <sup/gui/widgets/item_stack_widget.h>
#include <sup/gui/widgets/message_helper.h>

#include <sup/sequencer/workspace.h>

#include <QVBoxLayout>

namespace
{

/**
 * @brief Sets all action in the container to a given is_enabled sttaus
 */
void SetEnabled(const QList<QAction *> &actions, bool is_enabled)
{
  for (auto action : actions)
  {
    action->setEnabled(is_enabled);
  }
}
}  // namespace

namespace sequencergui
{

MonitorWidget::MonitorWidget(MonitorModel *model, QWidget *parent)
    : QWidget(parent)
    , m_model(model)
    , m_actions(new MonitorWidgetActions(this))
    , m_stack_widget(new sup::gui::ItemStackWidget)
    , m_workspace_tree(new WorkspaceEditorWidget(WorkspacePresentationType::kWorkspaceTree))
    , m_workspace_table(new WorkspaceEditorWidget(WorkspacePresentationType::kWorkspaceTable))
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_stack_widget);

  m_workspace_tree->setWindowTitle("Workspace Tree");
  m_workspace_table->setWindowTitle("Workspace Table");

  m_stack_widget->AddWidget(m_workspace_tree, m_workspace_tree->actions() + GetControlActions());
  m_stack_widget->AddWidget(m_workspace_table, m_workspace_table->actions());

  SetupConnections();  // should be after tree view got its model
}

MonitorWidget::~MonitorWidget() = default;

void MonitorWidget::SetWorkspaceItem(WorkspaceItem *item)
{
  m_workspace_tree->SetWorkspaceItem(item);
  m_workspace_table->SetWorkspaceItem(item);
}

void MonitorWidget::SetupConnections()
{
  connect(m_actions, &MonitorWidgetActions::StartMonitoringRequest, this,
          &MonitorWidget::OnStartMonitoringRequest);

  connect(m_actions, &MonitorWidgetActions::StopMonitoringRequest, this,
          &MonitorWidget::OnStopMonitoringRequest);
}

void MonitorWidget::OnStartMonitoringRequest()
{
  try
  {
    m_workspace = std::make_unique<workspace_t>();

    PopulateDomainWorkspace(*m_model->GetWorkspaceItem(), *m_workspace);

    m_workspace_synchronizer =
        std::make_unique<WorkspaceSynchronizer>(m_model->GetWorkspaceItem(), m_workspace.get());

    m_workspace->Setup();
    SetIsRunning(true);
  }
  catch (std::exception &ex)
  {
    sup::gui::SendWarningMessage({"Setup failed", "Can't setup workspace", ex.what()});
    UpdateVariableEditableProperty(false, *m_model->GetWorkspaceItem());
  }
}

void MonitorWidget::OnStopMonitoringRequest()
{
  SetIsRunning(false);
  // first synchronizer to unsubscribe drom domain
  m_workspace_synchronizer.reset();
  m_workspace.reset();
}

QList<QAction *> MonitorWidget::GetEditorActions()
{
  return m_workspace_tree->actions();
}

QList<QAction *> MonitorWidget::GetControlActions()
{
  return m_actions->GetActions({MonitorWidgetActions::ActionKey::kStartWorkspace,
                                MonitorWidgetActions::ActionKey::kStopWorkspace});
}

void MonitorWidget::SetIsRunning(bool is_running)
{
  // when running, disable editor actions, and enable control actions
  SetEnabled(GetEditorActions(), !is_running);
  m_actions->SetIsRunning(is_running);
}
}  // namespace sequencergui
