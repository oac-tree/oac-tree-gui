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

#ifndef OAC_TREE_GUI_VIEWS_PVMONITOR_MONITOR_MAIN_WINDOW_ACTIONS_H_
#define OAC_TREE_GUI_VIEWS_PVMONITOR_MONITOR_MAIN_WINDOW_ACTIONS_H_

#include <mvvm/project/i_project.h>

#include <QWidget>

class QMainWindow;
class QAction;
class QMenuBar;
class QMenu;

namespace mvvm
{
class ISessionModel;
class ProjectHandler;
}  // namespace mvvm

namespace oac_tree_gui
{

/**
 * @brief The MonitorMainWindowActions class handles actions of the main menubar of
 * PvMonitorMainWindow.
 */
class MonitorMainWindowActions : public QObject
{
  Q_OBJECT

public:
  explicit MonitorMainWindowActions(mvvm::IProject* project, QMainWindow* main_window);
  ~MonitorMainWindowActions() override;

  MonitorMainWindowActions(const MonitorMainWindowActions&) = delete;
  MonitorMainWindowActions& operator=(const MonitorMainWindowActions&) = delete;
  MonitorMainWindowActions(MonitorMainWindowActions&&) = delete;
  MonitorMainWindowActions& operator=(MonitorMainWindowActions&&) = delete;

  /**
   * @brief Closes current project.
   *
   * Internally performs check for unsaved data, and proceeds via save/discard/cancel dialog.
   * Returns true if project was successfully saved or user has agreed to discard unsaved changes,
   * and false otherwise. The later normally means that the user has changed his mind in the course
   * of this operation, canceled dialog, and the project has remained in unsaved state.
   *
   * @return True in the case of success.
   */
  bool CloseCurrentProject() const;

  void UpdateProjectNames();

private:
  void CreateActions(QMainWindow* mainwindow);
  void SetupMenus(QMenuBar* menu_bar);

  QAction* m_exit_action{nullptr};
  QMenu* m_recent_project_menu{nullptr};

  std::unique_ptr<mvvm::ProjectHandler> m_project_handler;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_PVMONITOR_MONITOR_MAIN_WINDOW_ACTIONS_H_
