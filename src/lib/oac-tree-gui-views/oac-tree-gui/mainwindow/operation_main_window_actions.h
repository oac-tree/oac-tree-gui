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

#ifndef SEQUENCERGUI_MAINWINDOW_OPERATION_MAIN_WINDOW_ACTIONS_H_
#define SEQUENCERGUI_MAINWINDOW_OPERATION_MAIN_WINDOW_ACTIONS_H_

#include <sup/gui/app/main_window_types.h>

#include <QObject>

#include <memory>

class QMainWindow;
class QAction;
class QMenuBar;
class QStatusBar;
class QToolButton;

namespace sup::gui
{
class AppContextFocusController;
}  // namespace sup::gui

namespace sequencergui
{

/**
 * @brief The OperationMainWindowActions class  handles actions of the main menubar.
 */
class OperationMainWindowActions : public QObject
{
  Q_OBJECT

public:
  explicit OperationMainWindowActions(QMainWindow* mainwindow);
  ~OperationMainWindowActions() override;

  /**
   * @brief Populates external status bar with actions.
   */
  void SetupStatusBar(QStatusBar* status_bar);

signals:
  void ImportJobRequest();
  void RestartApplicationRequest(sup::gui::AppExitCode);

private:
  void CreateActions(QMainWindow* mainwindow);
  void SetupMenus();
  void SetupFileMenu();
  void SetupViewMenu();
  void SetupHelpMenu();

  void OnAbout();
  void OnChangeSystemFont();
  void OnResetSettings();

  QAction* m_open_action{nullptr};
  QAction* m_exit_action{nullptr};
  QAction* m_about_action{nullptr};

  QAction* m_system_font_action{nullptr};
  QAction* m_reset_settings_action{nullptr};

  QToolButton* m_toggle_left_sidebar_button{nullptr};
  QToolButton* m_toggle_right_sidebar_button{nullptr};

  std::unique_ptr<sup::gui::AppContextFocusController> m_focus_controller;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_OPERATION_MAIN_WINDOW_ACTIONS_H_
