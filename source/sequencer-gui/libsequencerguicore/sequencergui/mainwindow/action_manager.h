/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_ACTION_MANAGER_H_
#define SEQUENCERGUI_MAINWINDOW_ACTION_MANAGER_H_

#include <QWidget>

class QMainWindow;
class QAction;
class QMenuBar;
class QMenu;

namespace sequencergui
{

class ProjectHandler;
class SequencerModel;

//! Vertical panel located on the left of XMLTreeView

class ActionManager : public QObject
{
  Q_OBJECT

public:
  explicit ActionManager(SequencerModel* model, QMainWindow* mainwindow);
  ~ActionManager() override;

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

  ProjectHandler* m_project_handler{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_ACTION_MANAGER_H_
