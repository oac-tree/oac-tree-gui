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

#ifndef OAC_TREE_GUI_MAINWINDOW_OPERATION_MAIN_WINDOW_H_
#define OAC_TREE_GUI_MAINWINDOW_OPERATION_MAIN_WINDOW_H_

#include <sup/gui/app/main_window_types.h>

#include <QMainWindow>
#include <memory>

class QCloseEvent;

namespace oac_tree_gui
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
  void InitApplication();
  void ReadSettings();
  void WriteSettings();
  bool CanCloseApplication();
  void OnRestartRequest(sup::gui::AppExitCode exit_code);

  std::unique_ptr<ApplicationModels> m_models;

  OperationMainWindowActions* m_action_manager{nullptr};

  OperationMonitorView* m_operation_view{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_OPERATION_MAIN_WINDOW_H_
