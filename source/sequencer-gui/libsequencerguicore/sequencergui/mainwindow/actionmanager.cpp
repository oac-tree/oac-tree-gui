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

#include "sequencergui/mainwindow/actionmanager.h"

#include "mvvm/widgets/widgetutils.h"

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

namespace sequencergui
{

ActionManager::ActionManager(QMainWindow *mainwindow) : QObject(mainwindow)
{
  CreateActions(mainwindow);
  SetupMenus(mainwindow->menuBar());
}

ActionManager::~ActionManager() = default;

//! Create main actions.

void ActionManager::CreateActions(QMainWindow *mainwindow)
{
  m_create_new_project_action = new QAction("&New Project", this);
  m_create_new_project_action->setShortcuts(QKeySequence::New);
  m_create_new_project_action->setStatusTip("Create a new project");
  //  connect(m_createNewProjectAction, &QAction::triggered, this,
  //          &ActionManager::createNewProjectRequest);

  m_open_existing_project_action = new QAction("&Open Project", this);
  m_open_existing_project_action->setShortcuts(QKeySequence::Open);
  m_open_existing_project_action->setStatusTip("Open an existing project");
  //  connect(m_openExistingProjectAction, &QAction::triggered,
  //          [this]() { openExistingProjectRequest({}); });

  m_save_current_project_action = new QAction("&Save Project", this);
  m_save_current_project_action->setShortcuts(QKeySequence::Save);
  m_save_current_project_action->setStatusTip("Save project");
  m_save_current_project_action->setShortcutContext(Qt::ApplicationShortcut);
  //  connect(m_saveCurrentProjectAction, &QAction::triggered, this,
  //          &ActionManager::saveCurrentProjectRequest);

  m_save_project_as_action = new QAction("Save &As...", this);
  m_save_project_as_action->setShortcuts(QKeySequence::SaveAs);
  m_save_project_as_action->setStatusTip("Save project under different name");
  //  connect(m_saveProjectAsAction, &QAction::triggered, this,
  //  &ActionManager::saveProjectAsRequest);

  m_exit_action = new QAction("E&xit Application", this);
  m_exit_action->setShortcuts(QKeySequence::Quit);
  m_exit_action->setStatusTip("Exit the application");
  connect(m_exit_action, &QAction::triggered, mainwindow, &QMainWindow::close);
}

//! Equips menu with actions.

void ActionManager::SetupMenus(QMenuBar *menubar)
{
  auto file_menu = menubar->addMenu("&File");
  connect(file_menu, &QMenu::aboutToShow, this, &ActionManager::AboutToShowFileMenu);
  file_menu->addAction(m_create_new_project_action);
  file_menu->addAction(m_open_existing_project_action);
  m_recent_project_menu = file_menu->addMenu("Recent Projects");

  file_menu->addSeparator();
  file_menu->addAction(m_save_current_project_action);
  file_menu->addAction(m_save_project_as_action);

  file_menu->addSeparator();
  file_menu->addAction(m_exit_action);
}

void ActionManager::AboutToShowFileMenu()
{
  QStringList m_recentProjects;  //  FIXME replace
  m_recent_project_menu->clear();
  m_recent_project_menu->setEnabled(!m_recentProjects.isEmpty());

  for (auto project_dir : m_recentProjects)
  {
    auto trimmed_project_dir = mvvm::utils::WithTildeHomePath(project_dir);
    auto action = m_recent_project_menu->addAction(trimmed_project_dir);
    action->setData(QVariant::fromValue(project_dir));
    auto on_project_selected = [this, project_dir]()
    {
      // openExistingProjectRequest(project_dir); FIXME uncomment
    };
    connect(action, &QAction::triggered, on_project_selected);
  }

  if (!m_recentProjects.empty())
  {
    m_recent_project_menu->addSeparator();
    auto action = m_recent_project_menu->addAction("Clear Menu");
    //      connect(action, &QAction::triggered, [this]() { clearResentProjectListRequest(); });
  }
}

}  // namespace sequencergui
