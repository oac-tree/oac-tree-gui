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

#ifndef OAC_TREE_GUI_VIEWS_PVMONITOR_MONITOR_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_PVMONITOR_MONITOR_WIDGET_H_

#include <QWidget>

class QTabWidget;
class QToolBar;

namespace sup::gui
{
class ItemStackWidget;
struct AppViewContext;
}  // namespace sup::gui

namespace oac_tree_gui
{

class ApplicationModels;
class JobItem;
class JobManager;
class ProcedureItem;
class WorkspaceItem;
class MonitorWidgetActions;
class WorkspaceEditorWidget;

/**
 * @brief The MonitorWidget class shows and monitors the workspace of the first procedure.
 *
 * Monitoring runs the procedure as a local job. While monitoring, the widget shows the workspace of
 * the job's expanded procedure.
 */
class MonitorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MonitorWidget(const sup::gui::AppViewContext& context, QWidget* parent_widget = nullptr);
  ~MonitorWidget() override;

  MonitorWidget(const MonitorWidget&) = delete;
  MonitorWidget& operator=(const MonitorWidget&) = delete;
  MonitorWidget(MonitorWidget&&) = delete;
  MonitorWidget& operator=(MonitorWidget&&) = delete;

  /**
   * @brief Sets the models and shows the workspace of the first procedure.
   *
   * Creates an untitled procedure if there is none. Monitoring must be stopped.
   */
  void SetModels(ApplicationModels* models);

  /**
   * @brief Returns the workspace shown in the widget.
   */
  WorkspaceItem* GetWorkspaceItem() const;

  void OnStartMonitoringRequest();
  void OnStopMonitoringRequest();

signals:
  void RunningStatusChanged(bool is_running);

private:
  void SetupConnections();
  QList<QAction*> GetEditorActions();
  QList<QAction*> GetControlActions();

  //! Shows the given workspace in the tree and the table.
  void SetWorkspaceItem(WorkspaceItem* item);

  /**
   * @brief Sets widget's actions in a state corresponding to the given running status.
   */
  void SetIsRunning(bool is_running);

  ApplicationModels* m_models{nullptr};
  ProcedureItem* m_procedure{nullptr};
  JobItem* m_job_item{nullptr};
  WorkspaceItem* m_workspace_item{nullptr};

  JobManager* m_job_manager{nullptr};
  MonitorWidgetActions* m_monitor_actions{nullptr};

  sup::gui::ItemStackWidget* m_stack_widget{nullptr};
  WorkspaceEditorWidget* m_workspace_tree{nullptr};
  WorkspaceEditorWidget* m_workspace_table{nullptr};
  QToolBar* m_tool_bar{nullptr};
  QTabWidget* m_tab_widget{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_PVMONITOR_MONITOR_WIDGET_H_
