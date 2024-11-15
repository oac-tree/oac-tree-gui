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

#ifndef SEQUENCERGUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_ACTIONS_H_
#define SEQUENCERGUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_ACTIONS_H_

#include <sup/gui/app/main_window_types.h>

#include <QObject>
#include <memory>

class QMainWindow;
class QAction;
class QMenuBar;
class QMenu;

namespace mvvm
{
class IProject;
class ProjectHandler;
}  // namespace mvvm

namespace sup::gui
{
class AppContextFocusController;
}  // namespace sup::gui

namespace sequencergui
{

/**
 * @brief The SequencerMainWindowActions class handles actions of the main menubar.
 */
class SequencerMainWindowActions : public QObject
{
  Q_OBJECT

public:
  explicit SequencerMainWindowActions(mvvm::IProject* project, QMainWindow* mainwindow);
  ~SequencerMainWindowActions() override;

  /**
   * @brief Closes current project.
   *
   * Internally performs check for unsaved data, and proceeds via save/discard/cancel dialog.
   * Returns true if project was successfully saved or user has agreed to discard unsaved changes,
   * and false otherwise. The later normally means that the user has changed his mind in the course
   * of this operation, canceled dialog, and the project has remained in unsaved state.
   *
   * @return True in the case of success.
   */
  bool CloseCurrentProject() const;

  void UpdateFocusWidget(QWidget* old, QWidget* now);

  void OnProjectModified();

signals:
  void RestartApplicationRequest(sup::gui::AppExitCode);

private:
  /**
   * @brief Create common main actions.
   */
  void CreateActions(QMainWindow* mainwindow);

  /**
   * @brief Populates main menu with actions.
   */
  void SetupMenus();
  void SetupFileMenu();
  void SetupEditMenu();
  void SetupViewMenu();
  void SetupToolsMenu();
  void SetupHelpMenu();

  void OnAbout();
  void OnChangeSystemFont();
  void OnApplicationSettingsDialog();
  void OnResetSettings();

  QAction* m_exit_action{nullptr};
  QAction* m_about_action{nullptr};

  QAction* m_system_font_action{nullptr};
  QAction* m_settings_dialog_action{nullptr};
  QAction* m_reset_settings_action{nullptr};

  QMenu* m_recent_project_menu{nullptr};

  std::unique_ptr<mvvm::ProjectHandler> m_project_handler;
  std::unique_ptr<sup::gui::AppContextFocusController> m_focus_controller;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_ACTIONS_H_
