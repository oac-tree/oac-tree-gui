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

#ifndef SEQUENCERGUI_VIEWS_PVMONITOR_MONITOR_MAIN_WINDOW_ACTIONS_H_
#define SEQUENCERGUI_VIEWS_PVMONITOR_MONITOR_MAIN_WINDOW_ACTIONS_H_

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

namespace sequencergui
{

/**
 * @brief The MonitorMainWindowActions class handles actions of the main menubar of
 * PvMonitorMainWindow.
 */
class MonitorMainWindowActions : public QObject
{
  Q_OBJECT

public:
  explicit MonitorMainWindowActions(mvvm::IProject* project, QMainWindow* mainwindow);
  ~MonitorMainWindowActions() override;

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
  void SetupMenus();

  QAction* m_exit_action{nullptr};
  QMenu* m_recent_project_menu{nullptr};

  std::unique_ptr<mvvm::ProjectHandler> m_project_handler;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_PVMONITOR_MONITOR_MAIN_WINDOW_ACTIONS_H_
