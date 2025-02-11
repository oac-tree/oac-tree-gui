/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_VIEWS_PVMONITOR_MONITOR_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_PVMONITOR_MONITOR_WIDGET_H_

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <QWidget>
#include <memory>

class QTabWidget;
class QToolBar;

namespace sup::gui
{
class ItemStackWidget;
}

namespace oac_tree_gui
{

class MonitorModel;
class WorkspaceSynchronizer;
class WorkspaceItem;
class MonitorWidgetActions;
class WorkspaceEditorWidget;

class MonitorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MonitorWidget(QWidget* parent_widget = nullptr);
  ~MonitorWidget() override;

  void SetModel(MonitorModel* model);

  void SetWorkspaceItem(WorkspaceItem* item);

  void OnStartMonitoringRequest();
  void OnStopMonitoringRequest();

private:
  void SetupConnections();
  QList<QAction*> GetEditorActions();
  QList<QAction*> GetControlActions();

  /**
   * @brief Sets widget's actions in a state corresponding to the given running status.
   */
  void SetIsRunning(bool is_running);

  WorkspaceItem* m_workspace_item{nullptr};
  std::unique_ptr<workspace_t> m_workspace;
  std::unique_ptr<WorkspaceSynchronizer> m_workspace_synchronizer;

  MonitorWidgetActions* m_monitor_actions{nullptr};

  sup::gui::ItemStackWidget* m_stack_widget{nullptr};
  WorkspaceEditorWidget* m_workspace_tree{nullptr};
  WorkspaceEditorWidget* m_workspace_table{nullptr};
  QToolBar* m_tool_bar{nullptr};
  QTabWidget* m_tab_widget{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_PVMONITOR_MONITOR_WIDGET_H_
