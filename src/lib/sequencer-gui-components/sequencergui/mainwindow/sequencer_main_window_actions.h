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

class QMainWindow;
class QAction;
class QMenuBar;
class QMenu;

namespace mvvm
{
class SessionModelInterface;
}

namespace sup::gui
{
class ProjectHandler;
}

namespace sequencergui
{

/**
 * @brief The SequencerMainWindowActions class handles actions of the main menubar.
 */
class SequencerMainWindowActions : public QObject
{
  Q_OBJECT

public:
  explicit SequencerMainWindowActions(mvvm::SessionModelInterface* model, QMainWindow* mainwindow);
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

signals:
  void RestartApplicationRequest(sup::gui::AppExitCode);
  void ProjectLoaded();

private:
  /**
   * @brief Create main actions.
   */
  void CreateActions(QMainWindow* mainwindow);

  /**
   * @brief Populates main menu with actions.
   */
  void SetupMenus();

  void OnAbout();
  void OnSummonSettingsDialogSettings();
  void OnChangeSystemFont();
  void OnResetSettings();

  QAction* m_exit_action{nullptr};
  QAction* m_about_action{nullptr};

  QAction* m_system_font_action{nullptr};
  QAction* m_reset_settings_action{nullptr};

  QMenu* m_recent_project_menu{nullptr};

  sup::gui::ProjectHandler* m_project_handler{nullptr};
  bool m_reset_settings_request{false};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_ACTIONS_H_
