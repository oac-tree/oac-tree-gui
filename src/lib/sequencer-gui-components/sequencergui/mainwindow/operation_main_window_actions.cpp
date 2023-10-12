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

#include "operation_main_window_actions.h"
#include "app_actions.h"
#include "app_action_manager.h"

#include <sequencergui/mainwindow/about_application_dialog.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/widgets/style_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

namespace sequencergui
{

OperationMainWindowActions::OperationMainWindowActions(mvvm::SessionModelInterface *model,
                                                       QMainWindow *mainwindow)
    : QObject(mainwindow)
{
  CreateActions(mainwindow);
  SetupMenus(mainwindow->menuBar());
}

OperationMainWindowActions::~OperationMainWindowActions() = default;

//! Create main actions.

void OperationMainWindowActions::CreateActions(QMainWindow *mainwindow)
{
  m_open_action = new QAction("Open XML procedure", this);
  m_open_action->setShortcuts(QKeySequence::Open);
  m_open_action->setStatusTip("Open sequencer XML file");
  connect(m_open_action, &QAction::triggered, mainwindow,
          [this](auto) { emit ImportJobRequest(); });

  m_exit_action = new QAction("Exit Application", this);
  m_exit_action->setShortcuts(QKeySequence::Quit);
  m_exit_action->setStatusTip("Exit the application");
  connect(m_exit_action, &QAction::triggered, mainwindow, &QMainWindow::close);

  m_about_action = new QAction("About application", this);
  m_about_action->setStatusTip("About application");
  connect(m_about_action, &QAction::triggered, this, &OperationMainWindowActions::OnAbout);
}

//! Equips menu with actions.

void OperationMainWindowActions::SetupMenus(QMenuBar *menubar)
{
  AppRegisterMenuBar(menubar);

  auto file_menu = AppAddMenu(constants::kFileMenu)->GetMenu();

  file_menu->addAction(m_open_action);
  file_menu->addSeparator();
  file_menu->addAction(m_exit_action);

  // will be populated from other widgets
  AppAddMenu(constants::kViewMenu);

  auto help_menu = AppAddMenu(constants::kHelpMenu)->GetMenu();
  help_menu->addAction(m_about_action);
}

void OperationMainWindowActions::OnAbout()
{
  AboutApplicationDialog dialog(mvvm::utils::FindMainWindow());
  dialog.exec();
}

}  // namespace sequencergui
