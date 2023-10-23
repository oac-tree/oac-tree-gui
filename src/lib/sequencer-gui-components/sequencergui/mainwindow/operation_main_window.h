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

#ifndef SEQUENCERGUI_MAINWINDOW_OPERATION_MAIN_WINDOW_H_
#define SEQUENCERGUI_MAINWINDOW_OPERATION_MAIN_WINDOW_H_

#include <QMainWindow>
#include <memory>
#include <sequencergui/mainwindow/app_settings.h>

class QCloseEvent;

namespace sequencergui
{

class OperationMonitorView;
class OperationMainWindowActions;
class ApplicationModels;

//! The main window of sequencer-operation applcation

class OperationMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  OperationMainWindow();
  ~OperationMainWindow() override;

  bool ImportProcedure(const QString& file_name);

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void PopulateModel();
  void InitApplication();
  void ReadSettings();
  void WriteSettings();
  bool CanCloseApplication();
  void OnRestartRequest(AppExitCode  exit_code);

  std::unique_ptr<ApplicationModels> m_models;

  OperationMainWindowActions* m_action_manager{nullptr};

  OperationMonitorView* m_operation_view{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_OPERATION_MAIN_WINDOW_H_
