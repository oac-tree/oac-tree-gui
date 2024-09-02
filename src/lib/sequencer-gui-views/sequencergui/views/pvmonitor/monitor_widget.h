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

#ifndef SEQUENCERGUI_VIEWS_PVMONITOR_MONITOR_WIDGET_H_
#define SEQUENCERGUI_VIEWS_PVMONITOR_MONITOR_WIDGET_H_

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <QWidget>
#include <memory>

class QTabWidget;
class QToolBar;

namespace sup::gui
{
class ItemStackWidget;
}

namespace sequencergui
{

class MonitorModel;
class WorkspaceSynchronizer;
class WorkspaceEditorActionHandler;
class WorkspaceEditorContext;
class WorkspaceItem;
class MonitorWidgetActions;
class WorkspaceEditorWidget;

class MonitorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MonitorWidget(MonitorModel* model, QWidget* parent = nullptr);
  ~MonitorWidget() override;

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

  MonitorModel* m_model{nullptr};
  std::unique_ptr<workspace_t> m_workspace;
  std::unique_ptr<WorkspaceSynchronizer> m_workspace_synchronizer;

  MonitorWidgetActions* m_monitor_actions{nullptr};

  WorkspaceEditorWidget* m_workspace_tree{nullptr};
  WorkspaceEditorWidget* m_workspace_table{nullptr};
  sup::gui::ItemStackWidget* m_stack_widget{nullptr};
  QToolBar* m_tool_bar{nullptr};
  QTabWidget* m_tab_widget{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_PVMONITOR_MONITOR_WIDGET_H_
