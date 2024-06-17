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

#include "monitor_main_window_actions.h"

#include <sequencergui/model/sequencer_model.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/project/project_handler.h>
#include <sup/gui/project/project_handler_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

namespace
{
const QString kApplicationType = "SUP PV Monitor";
}

namespace sequencergui
{

MonitorMainWindowActions::MonitorMainWindowActions(mvvm::SessionModelInterface *model,
                                                   QMainWindow *mainwindow)
    : QObject(mainwindow)
    , m_project_handler(new sup::gui::ProjectHandler(mvvm::ProjectType::kFileBased,
                                                     kApplicationType, {model}, mainwindow))
{
  sup::gui::AppRegisterMenuBar(mainwindow->menuBar(), {sup::gui::constants::kFileMenu});

  CreateActions(mainwindow);
  SetupMenus(mainwindow->menuBar());

  connect(m_project_handler, &sup::gui::ProjectHandler::ProjectLoaded, this,
          &MonitorMainWindowActions::ProjectLoaded);
}

//! Closes current project. Internally performs check for unsaved data, and proceeds via
//! save/discard/cancel dialog. Returns true if project was successfully saved, and false otherwise.
//! The later normally means that the user has changed his mind in the course of this operation and
//! rthe project has remained in unsaved state.

bool MonitorMainWindowActions::CloseCurrentProject() const
{
  return m_project_handler->CloseCurrentProject();
}

MonitorMainWindowActions::~MonitorMainWindowActions() = default;

//! Create main actions.

void MonitorMainWindowActions::CreateActions(QMainWindow *mainwindow)
{
  m_exit_action = new QAction("E&xit Application", this);
  m_exit_action->setShortcuts(QKeySequence::Quit);
  m_exit_action->setStatusTip("Exit the application");
  connect(m_exit_action, &QAction::triggered, mainwindow, &QMainWindow::close);
}

//! Equips menu with actions.

void MonitorMainWindowActions::SetupMenus(QMenuBar *menubar)
{
  auto file_menu = sup::gui::AppGetMenu(sup::gui::constants::kFileMenu);

  auto about_to_show_menu = [this]()
  { sup::gui::AddRecentProjectActions(m_recent_project_menu, *m_project_handler); };
  connect(file_menu, &QMenu::aboutToShow, this, about_to_show_menu);

  sup::gui::AddNewProjectAction(file_menu, *m_project_handler);
  sup::gui::AddOpenExistingProjectAction(file_menu, *m_project_handler);

  m_recent_project_menu = file_menu->addMenu("Recent Projects");

  file_menu->addSeparator();
  sup::gui::AddSaveCurrentProjectAction(file_menu, *m_project_handler);
  sup::gui::AddSaveProjectAsAction(file_menu, *m_project_handler);

  file_menu->addSeparator();
  file_menu->addAction(m_exit_action);
}

}  // namespace sequencergui
