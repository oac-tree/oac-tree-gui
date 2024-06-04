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
#include "app_constants.h"

#include <sequencergui/model/sequencer_model.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_command.h>
#include <sup/gui/app/app_context_focus_controller.h>
#include <sup/gui/app/application_helper.h>
#include <sup/gui/app/main_window_helper.h>
#include <sup/gui/project/project_handler.h>
#include <sup/gui/project/project_handler_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

namespace
{
const QString kApplicationType = "Sequencer GUI";

/**
 * @brief Creates focus controller pointing to global managers.
 */
std::unique_ptr<sup::gui::AppContextFocusController> CreateFocusController()
{
  return std::make_unique<sup::gui::AppContextFocusController>(
      sup::gui::GetGlobalContextManager(), sup::gui::GetGlobalCommandManager(), nullptr);
}

}  // namespace

namespace sequencergui
{

SequencerMainWindowActions::SequencerMainWindowActions(
    const std::vector<mvvm::SessionModelInterface *> &models, QMainWindow *mainwindow)
    : QObject(mainwindow)
    , m_project_handler(new sup::gui::ProjectHandler(mvvm::ProjectType::kFileBased,
                                                     kApplicationType, models, mainwindow))
    , m_focus_controller(CreateFocusController())
{
  sup::gui::AppRegisterMenuBar(mainwindow->menuBar(),
                               {sup::gui::constants::kFileMenu, sup::gui::constants::kEditMenu,
                                sup::gui::constants::kViewMenu, sup::gui::constants::kToolsMenu,
                                sup::gui::constants::kHelpMenu});

  CreateActions(mainwindow);
  SetupMenus();

  connect(m_project_handler, &sup::gui::ProjectHandler::ProjectLoaded, this,
          &SequencerMainWindowActions::ProjectLoaded);

  connect(qApp, &QApplication::focusChanged, m_focus_controller.get(),
          &sup::gui::AppContextFocusController::OnFocusWidgetUpdate);
}

SequencerMainWindowActions::~SequencerMainWindowActions() = default;

bool SequencerMainWindowActions::CloseCurrentProject() const
{
  return m_project_handler->CloseCurrentProject();
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
  preferences_menu->addSeparator();
  preferences_menu->addAction(m_reset_settings_action);

  file_menu->addSeparator();
  file_menu->addAction(m_exit_action);
}

void SequencerMainWindowActions::SetupEditMenu()
{
  auto command =
      sup::gui::AppAddCommandToMenu(sup::gui::constants::kEditMenu, app::constants::kCutCommandId);
  command->SetText("Cut").SetShortcut(QKeySequence::Cut);

  command =
      sup::gui::AppAddCommandToMenu(sup::gui::constants::kEditMenu, app::constants::kCopyCommandId);
  command->SetText("Copy").SetShortcut(QKeySequence::Copy);

  command = sup::gui::AppAddCommandToMenu(sup::gui::constants::kEditMenu,
                                          app::constants::kPasteCommandId);
  command->SetText("Paste").SetShortcut(QKeySequence::Paste);

  command = sup::gui::AppAddCommandToMenu(sup::gui::constants::kEditMenu,
                                          app::constants::kPasteSpecialCommandId);
  command->SetText("Paste Special").SetShortcut(QKeySequence("Ctrl+Shift+V"));
}

void SequencerMainWindowActions::SetupViewMenu() {}

void SequencerMainWindowActions::SetupToolsMenu()
{
  sup::gui::AppAddCommandToMenu(sup::gui::constants::kToolsMenu,
                                app::constants::kValidateProcedureCommandId);

  sup::gui::AppAddCommandToMenu(sup::gui::constants::kToolsMenu,
                                app::constants::kExportXmlCommandId);
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

void SequencerMainWindowActions::OnSummonSettingsDialogSettings() {}

void SequencerMainWindowActions::OnChangeSystemFont()
{
  if (sup::gui::SummonChangeSystemFontDialog())
  {
    emit RestartApplicationRequest(sup::gui::Restart);
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
