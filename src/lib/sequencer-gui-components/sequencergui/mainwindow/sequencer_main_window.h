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

#ifndef SEQUENCERGUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_H_
#define SEQUENCERGUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_H_

#include <QMainWindow>
#include <memory>

#include <sequencergui/mainwindow/app_settings.h>

class QCloseEvent;

namespace mvvm
{
class MainVerticalBarWidget;
}

namespace sequencergui
{

class SequencerExplorerView;
class OperationMonitorView;
class SequencerComposerView;
class SettingsView;
class SequencerMainWindowActions;
class ApplicationModels;

//! The main window of this application.

class SequencerMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  SequencerMainWindow();
  ~SequencerMainWindow() override;

  bool ImportProcedure(const QString& file_name);

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void InitApplication();
  void InitComponents();
  void ReadSettings();
  void WriteSettings();
  void PopulateModel();
  bool CanCloseApplication();
  void OnRestartRequest(AppExitCode  exit_code);

  std::unique_ptr<ApplicationModels> m_models;

  SequencerMainWindowActions* m_action_manager{nullptr};
  mvvm::MainVerticalBarWidget* m_tab_widget{nullptr};

  SequencerExplorerView* m_explorer_view{nullptr};
  SequencerComposerView* m_composer_view{nullptr};
  OperationMonitorView* m_operation_view{nullptr};
  SettingsView* m_settings_view{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_H_
