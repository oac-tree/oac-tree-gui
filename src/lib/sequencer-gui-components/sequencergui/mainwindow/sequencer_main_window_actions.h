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

#ifndef SEQUENCERGUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_ACTIONS_H_
#define SEQUENCERGUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_ACTIONS_H_

#include <QObject>

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

//! Main menu bar actions for the main window.

class SequencerMainWindowActions : public QObject
{
  Q_OBJECT

public:
  explicit SequencerMainWindowActions(mvvm::SessionModelInterface* model, QMainWindow* mainwindow);
  ~SequencerMainWindowActions() override;

  bool CloseCurrentProject() const;

  QMenu* GetToolsMenu();

private:
  void CreateActions(QMainWindow* mainwindow);
  void SetupMenus(QMenuBar* menubar);
  void OnAbout();

  QAction* m_exit_action{nullptr};
  QAction* m_about_action{nullptr};
  QMenu* m_recent_project_menu{nullptr};

  sup::gui::ProjectHandler* m_project_handler{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_ACTIONS_H_
