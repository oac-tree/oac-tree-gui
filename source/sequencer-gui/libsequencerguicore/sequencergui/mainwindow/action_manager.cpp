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

#include "sequencergui/mainwindow/action_manager.h"

#include "sequencergui/mainwindow/project_handler.h"
#include "sequencergui/model/sequencer_model.h"

#include "mvvm/widgets/widget_utils.h"

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

namespace sequencergui
{

ActionManager::ActionManager(SequencerModel *model, QMainWindow *mainwindow)
    : QObject(mainwindow), m_project_handler(new ProjectHandler(model, mainwindow))

{
  CreateActions(mainwindow);
  SetupMenus(mainwindow->menuBar());
}

//! Returns 'true' if application can be closed.
//! Internally will perform check for unsaved data, and proceed via save/discard/cancel dialog.

bool ActionManager::CanCloseApplication() const
{
  return m_project_handler->CanCloseProject();
}

ActionManager::~ActionManager() = default;

//! Create main actions.

void ActionManager::CreateActions(QMainWindow *mainwindow)
{
  m_create_new_project_action = new QAction("&New Project", this);
  m_create_new_project_action->setShortcuts(QKeySequence::New);
  m_create_new_project_action->setStatusTip("Create a new project");
  connect(m_create_new_project_action, &QAction::triggered, m_project_handler,
          &ProjectHandler::OnCreateNewProject);

  m_open_existing_project_action = new QAction("&Open Project", this);
  m_open_existing_project_action->setShortcuts(QKeySequence::Open);
  m_open_existing_project_action->setStatusTip("Open an existing project");
  connect(m_open_existing_project_action, &QAction::triggered, this,
          [this]() { m_project_handler->OnOpenExistingProject({}); });

  m_save_current_project_action = new QAction("&Save Project", this);
  m_save_current_project_action->setShortcuts(QKeySequence::Save);
  m_save_current_project_action->setStatusTip("Save project");
  m_save_current_project_action->setShortcutContext(Qt::ApplicationShortcut);
  connect(m_save_current_project_action, &QAction::triggered, m_project_handler,
          &ProjectHandler::OnSaveCurrentProject);

  m_save_project_as_action = new QAction("Save &As...", this);
  m_save_project_as_action->setShortcuts(QKeySequence::SaveAs);
  m_save_project_as_action->setStatusTip("Save project under different name");
  connect(m_save_project_as_action, &QAction::triggered, m_project_handler,
          &ProjectHandler::OnSaveProjectAs);

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
  auto recent_projects = m_project_handler->GetRecentProjectList();
  m_recent_project_menu->clear();
  m_recent_project_menu->setEnabled(!recent_projects.isEmpty());

  for (const auto &project_dir : recent_projects)
  {
    auto trimmed_project_dir = mvvm::utils::WithTildeHomePath(project_dir);
    auto action = m_recent_project_menu->addAction(trimmed_project_dir);
    action->setData(QVariant::fromValue(project_dir));
    auto on_project_selected = [this, project_dir]()
    { m_project_handler->OnOpenExistingProject(project_dir); };
    connect(action, &QAction::triggered, on_project_selected);
  }

  if (!recent_projects.empty())
  {
    m_recent_project_menu->addSeparator();
    auto action = m_recent_project_menu->addAction("Clear Menu");
    connect(action, &QAction::triggered, m_project_handler,
            &ProjectHandler::ClearRecentProjectsList);
  }
}

}  // namespace sequencergui
