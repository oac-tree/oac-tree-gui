/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "monitor_widget.h"

#include "monitor_widget_actions.h"

#include <oac_tree_gui/composer/workspace_editor_helper.h>
#include <oac_tree_gui/jobsystem/objects/job_manager.h>
#include <oac_tree_gui/jobsystem/objects/local_job_handler.h>
#include <oac_tree_gui/jobsystem/user_context.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_job_items.h>
#include <oac_tree_gui/style/style_helper.h>
#include <oac_tree_gui/views/composer/workspace_editor_widget.h>

#include <sup/gui/widgets/item_stack_widget.h>
#include <sup/gui/widgets/message_helper.h>

#include <QTabWidget>
#include <QToolBar>
#include <QVBoxLayout>

namespace oac_tree_gui
{

MonitorWidget::MonitorWidget(const sup::gui::AppViewContext& context, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_job_manager(new JobManager(
          [](JobItem& item) { return std::make_unique<LocalJobHandler>(&item, UserContext{}); },
          this))
    , m_monitor_actions(new MonitorWidgetActions(this))
    , m_stack_widget(new sup::gui::ItemStackWidget)
    , m_workspace_tree(
          new WorkspaceEditorWidget(context, WorkspacePresentationType::kWorkspaceTree))
    , m_workspace_table(
          new WorkspaceEditorWidget(context, WorkspacePresentationType::kWorkspaceTable))
    , m_tool_bar(new QToolBar)
    , m_tab_widget(new QTabWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_tab_widget);
  m_tab_widget->addTab(m_stack_widget, "Workspace");

  m_tool_bar->addActions(GetControlActions());
  m_tool_bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_tool_bar->setIconSize(ToolBarIconSize());

  m_workspace_tree->setWindowTitle("Workspace Tree");
  m_workspace_table->setWindowTitle("Workspace Table");

  m_stack_widget->AddWidget(m_workspace_tree, m_workspace_tree->actions());
  m_stack_widget->AddWidget(m_workspace_table, m_workspace_table->actions());

  SetupConnections();  // should be after tree view got its model
}

MonitorWidget::~MonitorWidget() = default;

void MonitorWidget::SetModels(ApplicationModels* models)
{
  m_models = models;

  auto sequencer_model = m_models->GetSequencerModel();
  if (sequencer_model->GetProcedures().empty())
  {
    sequencer_model->CreateUntitledProcedure();
  }
  m_procedure = sequencer_model->GetProcedures().at(0);

  SetWorkspaceItem(m_procedure->GetWorkspace());
}

WorkspaceItem* MonitorWidget::GetWorkspaceItem() const
{
  return m_workspace_item;
}

void MonitorWidget::SetWorkspaceItem(WorkspaceItem* item)
{
  m_workspace_item = item;
  m_workspace_tree->SetWorkspaceItem(item);
  m_workspace_table->SetWorkspaceItem(item);
}

void MonitorWidget::SetupConnections()
{
  connect(m_monitor_actions, &MonitorWidgetActions::StartMonitoringRequest, this,
          &MonitorWidget::OnStartMonitoringRequest);

  connect(m_monitor_actions, &MonitorWidgetActions::StopMonitoringRequest, this,
          &MonitorWidget::OnStopMonitoringRequest);
}

void MonitorWidget::OnStartMonitoringRequest()
{
  auto job_item = m_models->GetJobModel()->InsertItem<LocalJobItem>();
  job_item->SetProcedure(m_procedure);

  try
  {
    m_job_manager->SubmitJob(job_item);
  }
  catch (std::exception& ex)
  {
    m_models->GetJobModel()->RemoveItem(job_item);
    sup::gui::SendWarningMessage({"Setup failed", "Can't setup workspace", ex.what(), ""});
    return;
  }

  m_job_item = job_item;
  SetIsRunning(true);
}

void MonitorWidget::OnStopMonitoringRequest()
{
  if (m_job_item == nullptr)
  {
    return;
  }

  SetIsRunning(false);
  m_job_manager->RemoveJobHandler(m_job_item, RemovalPolicy::kForce);
  m_models->GetJobModel()->RemoveItem(m_job_item);
  m_job_item = nullptr;
}

QList<QAction*> MonitorWidget::GetEditorActions()
{
  return m_workspace_tree->actions();
}

QList<QAction*> MonitorWidget::GetControlActions()
{
  return m_monitor_actions->GetActions({MonitorWidgetActions::ActionKey::kStartWorkspace,
                                        MonitorWidgetActions::ActionKey::kStopWorkspace});
}

void MonitorWidget::SetIsRunning(bool is_running)
{
  // when running, show the job's workspace, disable editor actions, and enable control actions
  SetWorkspaceItem(is_running ? m_job_item->GetExpandedProcedure()->GetWorkspace()
                              : m_procedure->GetWorkspace());
  auto edit_type = is_running ? WorkspaceEditType::kValuesOnly : WorkspaceEditType::kEditorEnabled;
  m_workspace_tree->SetWorkspaceEditType(edit_type);
  m_workspace_table->SetWorkspaceEditType(edit_type);
  m_monitor_actions->SetIsRunning(is_running);
  UpdateVariableEditableProperty(is_running, *m_workspace_item);
  emit RunningStatusChanged(is_running);
}
}  // namespace oac_tree_gui
