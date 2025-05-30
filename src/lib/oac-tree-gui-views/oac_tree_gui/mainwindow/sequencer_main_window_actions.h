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

#ifndef OAC_TREE_GUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_ACTIONS_H_
#define OAC_TREE_GUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_ACTIONS_H_

#include <sup/gui/app/main_window_types.h>

#include <QObject>
#include <memory>

class QMainWindow;
class QAction;
class QMenuBar;
class QMenu;
class QStatusBar;
class QToolButton;

namespace mvvm
{
class IProject;
class ProjectHandler;
}  // namespace mvvm

namespace sup::gui
{
class AppContextFocusController;
}  // namespace sup::gui

namespace oac_tree_gui
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

  /**
   * @brief Update currently opened project name.
   *
   * Handles modified status of project name, updates recent project list.
   */
  void UpdateProjectNames();

  /**
   * @brief Populates external status bar with actions.
   */
  void SetupStatusBar(QStatusBar* status_bar);

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

  QToolButton* m_toggle_left_sidebar_button{nullptr};
  QToolButton* m_toggle_right_sidebar_button{nullptr};

  std::unique_ptr<mvvm::ProjectHandler> m_project_handler;
  std::unique_ptr<sup::gui::AppContextFocusController> m_focus_controller;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_ACTIONS_H_
