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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_TAB_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_TAB_WIDGET_H_

#include <sup/gui/mainwindow/session_item_widget.h>

#include <QWidget>
#include <vector>

class QTabWidget;
class QToolBar;

namespace mvvm
{
class ModelListener;
}

namespace oac_tree_gui
{

class InstructionItem;
class JobItem;
class JobLog;
class MonitorRealTimeActions;
class OperationRealTimeWidget;
class WorkspaceVariableTreeView;
class NodeEditorWidget;
class InstructionTaskMonitor;
class ProcedureItem;

/**
 * @brief The OperationTabWidget class shows running jobs with the possibility to select what to see
 * in a tab.
 *
 * Contains following views:
 * - main panel with real time tree
 * - workspace with variable tree
 * - workspace with variable table
 * - task monitor
 * - node editor
 */
class OperationTabWidget : public sup::gui::SessionItemWidget
{
  Q_OBJECT

public:
  explicit OperationTabWidget(QWidget* parent_widget = nullptr);
  ~OperationTabWidget() override;

  OperationTabWidget(const OperationTabWidget&) = delete;
  OperationTabWidget& operator=(const OperationTabWidget&) = delete;
  OperationTabWidget(OperationTabWidget&&) = delete;
  OperationTabWidget& operator=(OperationTabWidget&&) = delete;

  void SetItem(mvvm::SessionItem* job_item) override;
  const mvvm::SessionItem* GetItem() const override;
  void SetModel(mvvm::ISessionModel* model) override;

  void SetCurrentJob(JobItem* job_item);

  void SetSelectedInstructions(const std::vector<const InstructionItem*>& items);

  void SetJobLog(JobLog* job_log);

  int GetCurrentTickTimeout();

  /**
   * @brief Updates the tick timeout shown in the toolbar without emitting a change request.
   */
  void SetCurrentTickTimeout(int msec);

signals:
  void RunRequest(oac_tree_gui::JobItem* job);
  void PauseRequest(oac_tree_gui::JobItem* job);
  void StepRequest(oac_tree_gui::JobItem* job);
  void StopRequest(oac_tree_gui::JobItem* job);
  void ResetRequest(oac_tree_gui::JobItem* job);
  void ChangeDelayRequest(oac_tree_gui::JobItem* job, int msec);
  void ToggleBreakpointRequest(oac_tree_gui::JobItem* job,
                               const oac_tree_gui::InstructionItem* instruction);

private:
  void SetupConnections();
  void AddTab(QWidget* widget, const QString& label, const QString& tooltip);
  void SetExpandedProcedureItem(ProcedureItem* procedure);

  MonitorRealTimeActions* m_actions{nullptr};
  QToolBar* m_tool_bar{nullptr};
  QTabWidget* m_tab_widget{nullptr};
  OperationRealTimeWidget* m_realtime_widget{nullptr};
  WorkspaceVariableTreeView* m_workspace_tree{nullptr};
  WorkspaceVariableTreeView* m_workspace_table{nullptr};
  NodeEditorWidget* m_node_editor_widget{nullptr};
  InstructionTaskMonitor* m_instruction_task_monitor{nullptr};

  JobItem* m_current_job{nullptr};
  ProcedureItem* m_current_expanded_procedure_item{nullptr};

  std::unique_ptr<mvvm::ModelListener> m_model_listener;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_TAB_WIDGET_H_
