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

#ifndef SUPPVMONITOR_MAIN_WINDOW_ACTIONS_H
#define SUPPVMONITOR_MAIN_WINDOW_ACTIONS_H

#include <QWidget>

class QMainWindow;
class QAction;
class QMenuBar;
class QMenu;

namespace mvvm
{
class SessionModelInterface;
}

namespace sequencergui
{
class ProjectHandler;
}

namespace suppvmonitor
{

//! Vertical panel located on the left of XMLTreeView

class MainWindowActions : public QObject
{
  Q_OBJECT

public:
  explicit MainWindowActions(mvvm::SessionModelInterface* model, QMainWindow* mainwindow);
  ~MainWindowActions() override;

  bool CanCloseApplication() const;

private:
  void CreateActions(QMainWindow* mainwindow);
  void SetupMenus(QMenuBar* menubar);
  void AboutToShowFileMenu();
  void OnAbout();

  QAction* m_create_new_project_action{nullptr};
  QAction* m_open_existing_project_action{nullptr};
  QAction* m_save_current_project_action{nullptr};
  QAction* m_save_project_as_action{nullptr};
  QAction* m_exit_action{nullptr};
  QAction* m_about_action{nullptr};

  QMenu* m_recent_project_menu{nullptr};

  sequencergui::ProjectHandler* m_project_handler{nullptr};
};

}  // namespace sequencergui

#endif  // SUPPVMONITOR_MAIN_WINDOW_ACTIONS_H
