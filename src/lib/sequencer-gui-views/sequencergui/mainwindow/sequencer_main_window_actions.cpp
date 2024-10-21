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

#include "sequencer_main_window_actions.h"

#include "about_application_dialog.h"
#include "settings_editor_dialog.h"
#include "settings_helper.h"

#include <sequencergui/components/component_helper.h>
#include <sequencergui/model/sequencer_model.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_command.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/app/app_context_focus_controller.h>
#include <sup/gui/mainwindow/main_window_helper.h>
#include <sup/gui/project/project_handler_utils.h>
#include <sup/gui/project/project_handler.h>

#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

namespace
{
const QString kApplicationType = "Sequencer GUI";
}

namespace sequencergui
{

SequencerMainWindowActions::SequencerMainWindowActions(mvvm::IProject *project,
                                                       QMainWindow *mainwindow)
    : QObject(mainwindow)
    , m_project_handler(std::make_unique<sup::gui::ProjectHandler>(project))
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
  return m_project_handler->CloseCurrentProject();
}

void SequencerMainWindowActions::OnProjectModified()
{
  m_project_handler->UpdateNames();
}

void SequencerMainWindowActions::CreateActions(QMainWindow *mainwindow)
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
  { sup::gui::AddRecentProjectActions(m_recent_project_menu, *m_project_handler); };
  connect(file_menu, &QMenu::aboutToShow, this, about_to_show_menu);

  sup::gui::AddNewProjectAction(file_menu, *m_project_handler);
  sup::gui::AddOpenExistingProjectAction(file_menu, *m_project_handler);

  m_recent_project_menu = file_menu->addMenu("Recent Projects");

  file_menu->addSeparator();
  sup::gui::AddSaveCurrentProjectAction(file_menu, *m_project_handler);
  sup::gui::AddSaveProjectAsAction(file_menu, *m_project_handler);

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
}

void SequencerMainWindowActions::SetupViewMenu() {}

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
  SettingsEditorDialog dialog;
  dialog.SetInitialValues(GetGlobalSettings());
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

}  // namespace sequencergui
