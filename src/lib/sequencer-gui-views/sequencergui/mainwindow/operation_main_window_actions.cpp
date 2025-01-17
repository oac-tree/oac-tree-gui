/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "operation_main_window_actions.h"

#include <sequencergui/mainwindow/about_application_dialog.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/style/style_helper.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_action_manager.h>
#include <sup/gui/app/app_command.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/app/app_context_focus_controller.h>
#include <sup/gui/mainwindow/main_window_helper.h>
#include <sup/gui/mainwindow/status_bar_helper.h>

#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolButton>

namespace sequencergui
{

OperationMainWindowActions::OperationMainWindowActions(QMainWindow *mainwindow)
    : QObject(mainwindow), m_focus_controller(sup::gui::CreateGlobalFocusController())
{
  sup::gui::AppRegisterMenuBar(mainwindow->menuBar(),
                               {sup::gui::constants::kFileMenu, sup::gui::constants::kViewMenu,
                                sup::gui::constants::kHelpMenu});

  CreateActions(mainwindow);
  SetupMenus();

  SetupStatusBar(mainwindow->statusBar());
}

void OperationMainWindowActions::SetupStatusBar(QStatusBar *status_bar)
{
  m_toggle_left_sidebar_button = new QToolButton;
  m_toggle_left_sidebar_button->setToolTip("Show/hide left panel");
  m_toggle_left_sidebar_button->setIcon(FindIcon("dock-left"));
  sup::gui::SetupStatusBarButton(m_toggle_left_sidebar_button,
                                 sup::gui::constants::kToggleLeftPanelCommandId);

  m_toggle_right_sidebar_button = new QToolButton;
  m_toggle_right_sidebar_button->setToolTip("Show/hide right panel");
  m_toggle_right_sidebar_button->setIcon(FindIcon("dock-right"));
  sup::gui::SetupStatusBarButton(m_toggle_right_sidebar_button,
                                 sup::gui::constants::kToggleRightPanelCommandId);

  status_bar->addPermanentWidget(m_toggle_left_sidebar_button, 0);

  auto expander = new QWidget;
  expander->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  status_bar->addPermanentWidget(expander, 1);

  status_bar->addPermanentWidget(m_toggle_right_sidebar_button, 0);
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

  m_system_font_action = new QAction("System font (restart is required)", this);
  m_system_font_action->setStatusTip("Summon font settings dialog");
  connect(m_system_font_action, &QAction::triggered, this,
          &OperationMainWindowActions::OnChangeSystemFont);

  m_reset_settings_action = new QAction("Reset settings to defaults", this);
  m_reset_settings_action->setStatusTip(
      "Reset persistent application settings on disk to their defaults");
  m_reset_settings_action->setToolTip(
      "Reset persistent application settings on disk to their defaults");
  connect(m_reset_settings_action, &QAction::triggered, this,
          &OperationMainWindowActions::OnResetSettings);
}

void OperationMainWindowActions::SetupMenus()
{
  SetupFileMenu();
  SetupViewMenu();
  SetupHelpMenu();
}

void OperationMainWindowActions::SetupFileMenu()
{
  auto file_menu = sup::gui::AppGetMenu(sup::gui::constants::kFileMenu);

  file_menu->addAction(m_open_action);

  file_menu->addSeparator();
  auto preferences_menu = file_menu->addMenu("Preferences");
  preferences_menu->setToolTipsVisible(true);
  preferences_menu->addAction(m_system_font_action);
  preferences_menu->addSeparator();
  preferences_menu->addAction(m_reset_settings_action);

  file_menu->addSeparator();
  file_menu->addAction(m_exit_action);
}

void OperationMainWindowActions::SetupViewMenu()
{
  auto command = sup::gui::AppAddCommandToMenu(sup::gui::constants::kViewMenu,
                                               sup::gui::constants::kToggleLeftPanelCommandId);
  command->SetShortcut(QKeySequence("Alt+0"));

  command = sup::gui::AppAddCommandToMenu(sup::gui::constants::kViewMenu,
                                          sup::gui::constants::kToggleRightPanelCommandId);
  command->SetShortcut(QKeySequence("Alt+Shift+0"));
}

void OperationMainWindowActions::SetupHelpMenu()
{
  auto help_menu = sup::gui::AppGetMenu(sup::gui::constants::kHelpMenu);
  help_menu->addAction(m_about_action);
}

void OperationMainWindowActions::OnAbout()
{
  AboutApplicationDialog dialog(mvvm::utils::FindMainWindow());
  dialog.exec();
}

void OperationMainWindowActions::OnChangeSystemFont()
{
  if (sup::gui::SummonChangeSystemFontDialog())
  {
    emit RestartApplicationRequest(sup::gui::Restart);
  }
}

void OperationMainWindowActions::OnResetSettings()
{
  if (sup::gui::ShouldResetSettingsAndRestart())
  {
    emit RestartApplicationRequest(sup::gui::CleanSettingsAndRestart);
  }
}

}  // namespace sequencergui
