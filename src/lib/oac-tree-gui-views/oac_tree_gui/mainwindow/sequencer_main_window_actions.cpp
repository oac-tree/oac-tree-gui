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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencer_main_window_actions.h"

#include "about_application_dialog.h"

#include <oac_tree_gui/components/component_helper.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/style/style_helper.h>

#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_command.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/app/app_context_focus_controller.h>
#include <sup/gui/components/proxy_action.h>
#include <sup/gui/mainwindow/main_window_helper.h>
#include <sup/gui/mainwindow/settings_editor_dialog.h>
#include <sup/gui/mainwindow/settings_helper.h>
#include <sup/gui/mainwindow/status_bar_helper.h>

#include <mvvm/project/project_handler.h>
#include <mvvm/project/project_handler_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolButton>

namespace
{
const QString kApplicationType = "oac-tree GUI";
}  // namespace

namespace oac_tree_gui
{

SequencerMainWindowActions::SequencerMainWindowActions(mvvm::IProject* project,
                                                       QMainWindow* mainwindow)
    : QObject(mainwindow)
    , m_project_handler(std::make_unique<mvvm::ProjectHandler>(project))
    , m_focus_controller(sup::gui::CreateGlobalFocusController())
{
  sup::gui::AppRegisterMenuBar(mainwindow->menuBar(),
                               {sup::gui::constants::kFileMenu, sup::gui::constants::kEditMenu,
                                sup::gui::constants::kViewMenu, sup::gui::constants::kToolsMenu,
                                sup::gui::constants::kHelpMenu});

  CreateActions(mainwindow);
  SetupMenus();
}

SequencerMainWindowActions::~SequencerMainWindowActions() = default;

bool SequencerMainWindowActions::CloseCurrentProject() const
{
  return m_project_handler->CloseProject();
}

void SequencerMainWindowActions::UpdateProjectNames()
{
  m_project_handler->UpdateNames();
}

void SequencerMainWindowActions::SetupStatusBar(QStatusBar* status_bar)
{
  status_bar->setVisible(true);

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
  sup::gui::AddPermanentStretch(status_bar);
  status_bar->addPermanentWidget(m_toggle_right_sidebar_button, 0);
}

void SequencerMainWindowActions::CreateActions(QMainWindow* mainwindow)
{
  m_exit_action = new QAction("Exit Application", this);
  m_exit_action->setShortcuts(QKeySequence::Quit);
  m_exit_action->setStatusTip("Exit the application");
  connect(m_exit_action, &QAction::triggered, mainwindow, &QMainWindow::close);

  m_about_action = new QAction("About application", this);
  m_about_action->setStatusTip("About application");
  connect(m_about_action, &QAction::triggered, this, &SequencerMainWindowActions::OnAbout);

  m_system_font_action = new QAction("Set system font (restart is required)", this);
  m_system_font_action->setStatusTip("Summon font settings dialog");
  connect(m_system_font_action, &QAction::triggered, this,
          &SequencerMainWindowActions::OnChangeSystemFont);

  m_settings_dialog_action = new QAction("Application settings", this);
  m_settings_dialog_action->setStatusTip("Summon application settings dialog");
  connect(m_settings_dialog_action, &QAction::triggered, this,
          &SequencerMainWindowActions::OnApplicationSettingsDialog);

  m_reset_settings_action = new QAction("Reset settings to defaults", this);
  m_reset_settings_action->setStatusTip(
      "Reset persistent application settings on disk to their defaults");
  m_reset_settings_action->setToolTip(
      "Reset persistent application settings on disk to their defaults");
  connect(m_reset_settings_action, &QAction::triggered, this,
          &SequencerMainWindowActions::OnResetSettings);
}

void SequencerMainWindowActions::SetupMenus()
{
  SetupFileMenu();
  SetupEditMenu();
  SetupViewMenu();
  SetupToolsMenu();
  SetupHelpMenu();
}

void SequencerMainWindowActions::SetupFileMenu()
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
  auto preferences_menu = file_menu->addMenu("Preferences");
  preferences_menu->setToolTipsVisible(true);
  preferences_menu->addAction(m_system_font_action);
  preferences_menu->addAction(m_settings_dialog_action);
  preferences_menu->addSeparator();
  preferences_menu->addAction(m_reset_settings_action);

  file_menu->addSeparator();
  file_menu->addAction(m_exit_action);
}

void SequencerMainWindowActions::SetupEditMenu()
{
  auto command = sup::gui::AppAddCommandToMenu(sup::gui::constants::kEditMenu,
                                               sup::gui::constants::kUndoCommandId);
  command->SetText("Undo").SetShortcut(QKeySequence::Undo);

  command = sup::gui::AppAddCommandToMenu(sup::gui::constants::kEditMenu,
                                          sup::gui::constants::kRedoCommandId);
  command->SetText("Redo").SetShortcut(QKeySequence::Redo);

  sup::gui::AppGetMenu(sup::gui::constants::kEditMenu)->addSeparator();

  command = sup::gui::AppAddCommandToMenu(sup::gui::constants::kEditMenu,
                                          sup::gui::constants::kCutCommandId);
  command->SetText("Cut").SetShortcut(QKeySequence::Cut);

  command = sup::gui::AppAddCommandToMenu(sup::gui::constants::kEditMenu,
                                          sup::gui::constants::kCopyCommandId);
  command->SetText("Copy").SetShortcut(QKeySequence::Copy);

  command = sup::gui::AppAddCommandToMenu(sup::gui::constants::kEditMenu,
                                          sup::gui::constants::kPasteCommandId);
  command->SetText("Paste").SetShortcut(QKeySequence::Paste);

  command = sup::gui::AppAddCommandToMenu(sup::gui::constants::kEditMenu,
                                          sup::gui::constants::kPasteSpecialCommandId);
  command->SetText("Paste Special").SetShortcut(QKeySequence("Ctrl+Shift+V"));

  command = sup::gui::AppAddCommandToMenu(sup::gui::constants::kEditMenu,
                                          sup::gui::constants::kRemoveSelectedCommandId);
  command->SetText("Remove selected");
  command->GetProxyAction()->setShortcuts({Qt::Key_Delete, Qt::Key_Backspace});
}

void SequencerMainWindowActions::SetupViewMenu()
{
  auto command = sup::gui::AppAddCommandToMenu(sup::gui::constants::kViewMenu,
                                               sup::gui::constants::kToggleLeftPanelCommandId);
  command->SetShortcut(QKeySequence("Alt+0"));

  command = sup::gui::AppAddCommandToMenu(sup::gui::constants::kViewMenu,
                                          sup::gui::constants::kToggleRightPanelCommandId);
  command->SetShortcut(QKeySequence("Alt+Shift+0"));
}

void SequencerMainWindowActions::SetupToolsMenu()
{
  sup::gui::AppAddCommandToMenu(sup::gui::constants::kToolsMenu,
                                constants::kValidateProcedureCommandId);

  sup::gui::AppAddCommandToMenu(sup::gui::constants::kToolsMenu, constants::kExportXmlCommandId);
}

void SequencerMainWindowActions::SetupHelpMenu()
{
  auto help_menu = sup::gui::AppGetMenu(sup::gui::constants::kHelpMenu);
  help_menu->addAction(m_about_action);
}

void SequencerMainWindowActions::OnAbout()
{
  AboutApplicationDialog dialog(mvvm::utils::FindMainWindow());
  dialog.exec();
}

void SequencerMainWindowActions::OnChangeSystemFont()
{
  if (sup::gui::SummonChangeSystemFontDialog())
  {
    emit RestartApplicationRequest(sup::gui::Restart);
  }
}

void SequencerMainWindowActions::OnApplicationSettingsDialog()
{
  sup::gui::SettingsEditorDialog dialog;
  dialog.SetInitialValues(sup::gui::GetGlobalSettings());
  if (dialog.exec() == QDialog::Accepted)
  {
    SaveSettingsInPersistentStorage(*dialog.GetResult());
  }
}

void SequencerMainWindowActions::OnResetSettings()
{
  if (sup::gui::ShouldResetSettingsAndRestart())
  {
    emit RestartApplicationRequest(sup::gui::CleanSettingsAndRestart);
  }
}

}  // namespace oac_tree_gui
