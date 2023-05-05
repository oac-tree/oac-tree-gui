/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_PVMONITOR_MONITOR_MAIN_WINDOW_ACTIONS_H_
#define SEQUENCERGUI_PVMONITOR_MONITOR_MAIN_WINDOW_ACTIONS_H_

#include <QWidget>

class QMainWindow;
class QAction;
class QMenuBar;
class QMenu;

namespace mvvm
{
class SessionModelInterface;
}

namespace sup::gui
{
class ProjectHandler;
}

namespace sequencergui
{

//! Vertical panel located on the left of XMLTreeView

class MonitorMainWindowActions : public QObject
{
  Q_OBJECT

public:
  explicit MonitorMainWindowActions(mvvm::SessionModelInterface* model, QMainWindow* mainwindow);
  ~MonitorMainWindowActions() override;

  bool CanCloseApplication() const;

private:
  void CreateActions(QMainWindow* mainwindow);
  void SetupMenus(QMenuBar* menubar);
  void AboutToShowFileMenu();

  QAction* m_create_new_project_action{nullptr};
  QAction* m_open_existing_project_action{nullptr};
  QAction* m_save_current_project_action{nullptr};
  QAction* m_save_project_as_action{nullptr};
  QAction* m_exit_action{nullptr};

  QMenu* m_recent_project_menu{nullptr};

  sup::gui::ProjectHandler* m_project_handler{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_MONITOR_MAIN_WINDOW_ACTIONS_H_
