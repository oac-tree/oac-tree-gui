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

#include "monitor_main_window_actions.h"

#include <oac-tree-gui/model/sequencer_model.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_constants.h>

#include <mvvm/project/project_handler.h>
#include <mvvm/project/project_handler_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

namespace sequencergui
{

MonitorMainWindowActions::MonitorMainWindowActions(mvvm::IProject *project, QMainWindow *mainwindow)
    : QObject(mainwindow), m_project_handler(std::make_unique<mvvm::ProjectHandler>(project))
{
  sup::gui::AppRegisterMenuBar(mainwindow->menuBar(), {sup::gui::constants::kFileMenu});

  CreateActions(mainwindow);
  SetupMenus();
}

MonitorMainWindowActions::~MonitorMainWindowActions() = default;

bool MonitorMainWindowActions::CloseCurrentProject() const
{
  return m_project_handler->CloseProject();
}

void MonitorMainWindowActions::UpdateProjectNames()
{
  m_project_handler->UpdateNames();
}

void MonitorMainWindowActions::CreateActions(QMainWindow *mainwindow)
{
  m_exit_action = new QAction("E&xit Application", this);
  m_exit_action->setShortcuts(QKeySequence::Quit);
  m_exit_action->setStatusTip("Exit the application");
  connect(m_exit_action, &QAction::triggered, mainwindow, &QMainWindow::close);
}

void MonitorMainWindowActions::SetupMenus()
{
  auto file_menu = sup::gui::AppGetMenu(sup::gui::constants::kFileMenu);

  auto about_to_show_menu = [this]()
  { mvvm::AddRecentProjectActions(m_recent_project_menu, *m_project_handler); };
  connect(file_menu, &QMenu::aboutToShow, this, about_to_show_menu);

  mvvm::AddNewProjectAction(file_menu, *m_project_handler);
  mvvm::AddOpenExistingProjectAction(file_menu, *m_project_handler);

  m_recent_project_menu = file_menu->addMenu("Recent Projects");

  file_menu->addSeparator();
  mvvm::AddSaveCurrentProjectAction(file_menu, *m_project_handler);
  mvvm::AddSaveProjectAsAction(file_menu, *m_project_handler);

  file_menu->addSeparator();
  file_menu->addAction(m_exit_action);
}

}  // namespace sequencergui
