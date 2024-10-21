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

#ifndef SEQUENCERGUI_MAINWINDOW_OPERATION_MAIN_WINDOW_ACTIONS_H_
#define SEQUENCERGUI_MAINWINDOW_OPERATION_MAIN_WINDOW_ACTIONS_H_

#include <sup/gui/app/main_window_types.h>

#include <QObject>

class QMainWindow;
class QAction;
class QMenuBar;

namespace mvvm
{
class ISessionModel;
}

namespace sequencergui
{

/**
 * @brief The OperationMainWindowActions class  handles actions of the main menubar.
 */
class OperationMainWindowActions : public QObject
{
  Q_OBJECT

public:
  explicit OperationMainWindowActions(mvvm::ISessionModel* model, QMainWindow* mainwindow);
  ~OperationMainWindowActions() override;

signals:
  void ImportJobRequest();
  void RestartApplicationRequest(sup::gui::AppExitCode);

private:
  void CreateActions(QMainWindow* mainwindow);
  void SetupMenus(QMenuBar* menubar);
  void OnAbout();
  void OnChangeSystemFont();
  void OnResetSettings();

  QAction* m_open_action{nullptr};
  QAction* m_exit_action{nullptr};
  QAction* m_about_action{nullptr};

  QAction* m_system_font_action{nullptr};
  QAction* m_reset_settings_action{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_OPERATION_MAIN_WINDOW_ACTIONS_H_
