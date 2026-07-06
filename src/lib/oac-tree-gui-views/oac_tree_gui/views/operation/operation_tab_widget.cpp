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

#include "operation_tab_widget.h"

#include "instruction_task_monitor.h"
#include "monitor_realtime_actions.h"
#include "operation_realtime_widget.h"

#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/style/style_helper.h>
#include <oac_tree_gui/views/composer/workspace_variable_tree_view.h>
#include <oac_tree_gui/views/nodeeditor/node_editor_widget.h>

#include <mvvm/style/mvvm_style_helper.h>

#include <QTabWidget>
#include <QToolBar>
#include <QVBoxLayout>

namespace
{

QList<QAction*> GetToolBarActions(oac_tree_gui::MonitorRealTimeActions* actions)
{
  using ActionKey = oac_tree_gui::MonitorRealTimeActions::ActionKey;
  return actions->GetActions({ActionKey::kRun, ActionKey::kPause, ActionKey::kStep,
                              ActionKey::kStop, ActionKey::kReset, ActionKey::kDelay,
                              ActionKey::kSettings});
}

}  // namespace

namespace oac_tree_gui
{

OperationTabWidget::OperationTabWidget(QWidget* parent_widget)
    : sup::gui::SessionItemWidget(parent_widget)
    , m_actions(new MonitorRealTimeActions(this))
    , m_tool_bar(new QToolBar)
    , m_tab_widget(new QTabWidget)
    , m_realtime_widget(new OperationRealTimeWidget)
    , m_workspace_tree(new WorkspaceVariableTreeView(WorkspacePresentationType::kWorkspaceTree))
    , m_workspace_table(new WorkspaceVariableTreeView(WorkspacePresentationType::kWorkspaceTable))
    , m_node_editor_widget(new NodeEditorWidget(NodeEditorMode::kNodeViewer))
    , m_instruction_task_monitor(new InstructionTaskMonitor)
{
  m_workspace_tree->SetEditable(false);
  m_workspace_table->SetEditable(false);

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_tool_bar->setIconSize(NarrowToolBarIconSize());
  m_tool_bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  // to have fixed height when no actions are added
  m_tool_bar->setFixedHeight(NarrowToolBarIconSize().height() + mvvm::style::UnitSize(1.9));
  m_tool_bar->addActions(GetToolBarActions(m_actions));

  layout->addWidget(m_tool_bar);
  layout->addWidget(m_tab_widget);

  AddTab(m_realtime_widget, "Instructions", "Real time instruction tree with message panel");
  AddTab(m_workspace_tree, "Variable tree", "Workspace variables as a tree");
  AddTab(m_workspace_table, "Variable table", "Workspace variables as a table");
  AddTab(m_node_editor_widget, "Node view", "Instruction tree as a node graph");
  AddTab(m_instruction_task_monitor, "Task monitor", "Instruction task monitor");

  SetupConnections();
}

void OperationTabWidget::SetItem(mvvm::SessionItem* job_item)
{
  SetCurrentJob(dynamic_cast<JobItem*>(job_item));
}

void OperationTabWidget::SetCurrentJob(JobItem* job_item)
{
  m_current_job = job_item;

  auto procedure = (m_current_job == nullptr) ? nullptr : job_item->GetExpandedProcedure();
  auto workspace_item = (procedure == nullptr) ? nullptr : procedure->GetWorkspace();
  auto instructions = (procedure == nullptr) ? nullptr : procedure->GetInstructionContainer();

  m_realtime_widget->SetProcedure(procedure);
  m_workspace_tree->SetWorkspaceItem(workspace_item);
  m_workspace_table->SetWorkspaceItem(workspace_item);
  m_node_editor_widget->SetProcedure(procedure);
  m_instruction_task_monitor->SetInstructionContainer(instructions);

  if (job_item != nullptr)
  {
    m_actions->SetCurrentTickTimeout(job_item->GetTickTimeout().count());
  }
}

void OperationTabWidget::SetSelectedInstructions(const std::vector<const InstructionItem*>& items)
{
  m_realtime_widget->SetSelectedInstructions(items);
}

void OperationTabWidget::SetJobLog(JobLog* job_log)
{
  m_realtime_widget->SetJobLog(job_log);
}

int OperationTabWidget::GetCurrentTickTimeout()
{
  return m_actions->GetCurrentTickTimeout();
}

void OperationTabWidget::SetupConnections()
{
  // forward signals from a toolbar further up, stamping them with the job currently shown here
  connect(m_actions, &MonitorRealTimeActions::RunRequest, this,
          [this]() { emit RunRequest(m_current_job); });
  connect(m_actions, &MonitorRealTimeActions::PauseRequest, this,
          [this]() { emit PauseRequest(m_current_job); });
  connect(m_actions, &MonitorRealTimeActions::StepRequest, this,
          [this]() { emit StepRequest(m_current_job); });
  connect(m_actions, &MonitorRealTimeActions::StopRequest, this,
          [this]() { emit StopRequest(m_current_job); });
  connect(m_actions, &MonitorRealTimeActions::ResetRequest, this,
          [this]() { emit ResetRequest(m_current_job); });
  connect(m_actions, &MonitorRealTimeActions::ChangeDelayRequest, this,
          &OperationTabWidget::ChangeDelayRequest);

  connect(m_actions, &MonitorRealTimeActions::ScrollToSelectionRequest, m_realtime_widget,
          &OperationRealTimeWidget::SetViewportFollowsSelectionFlag);

  connect(m_realtime_widget, &OperationRealTimeWidget::ToggleBreakpointRequest, this,
          [this](const InstructionItem* instruction)
          { emit ToggleBreakpointRequest(m_current_job, instruction); });
}

void OperationTabWidget::AddTab(QWidget* widget, const QString& label, const QString& tooltip)
{
  m_tab_widget->addTab(widget, label);
  m_tab_widget->setTabToolTip(m_tab_widget->indexOf(widget), tooltip);
}

}  // namespace oac_tree_gui
