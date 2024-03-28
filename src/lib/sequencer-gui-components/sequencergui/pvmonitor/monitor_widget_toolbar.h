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

#ifndef SEQUENCERGUI_PVMONITOR_MONITOR_WIDGET_TOOLBAR_H_
#define SEQUENCERGUI_PVMONITOR_MONITOR_WIDGET_TOOLBAR_H_

#include <QToolBar>

namespace sequencergui
{

class WorkspaceEditorActions;

class MonitorWidgetToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit MonitorWidgetToolBar(QWidget* parent = nullptr);
  ~MonitorWidgetToolBar() override;

  /**
   * @brief Update enable/disable state of all toolbar actions according to the flag.
   *
   * @param is_running Workspace is running, when true.
   */
  void UpdateActionsState(bool is_running);

signals:
  void AddVariableRequest(const QString& variable_type_name);
  void EditAnyvalueRequest();
  void RemoveVariableRequest();
  void StartMonitoringRequest();
  void StopMonitoringRequest();

private:
  std::unique_ptr<QMenu> CreateAddVariableMenu();
  void InsertStrech();

  WorkspaceEditorActions* m_workspace_editor_actions{nullptr};

  QAction* m_start_action{nullptr};
  QAction* m_stop_action{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_MONITOR_WIDGET_TOOLBAR_H_
