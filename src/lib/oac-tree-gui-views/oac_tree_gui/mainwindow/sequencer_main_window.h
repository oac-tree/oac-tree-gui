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

#ifndef OAC_TREE_GUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_H_
#define OAC_TREE_GUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_H_

#include <sup/gui/app/main_window_types.h>

#include <QMainWindow>
#include <memory>

class QCloseEvent;

namespace mvvm
{
class MainVerticalBarWidget;
}

namespace oac_tree_gui
{

class ApplicationModels;
class OperationMonitorView;
class SequencerComposerView;
class SequencerExplorerView;
class SequencerMainWindowActions;
class SequencerMainWindowContext;
class SequencerSettingsModel;

/**
 * @brief The SequencerMainWindow class is the main window of sequencer-gui application.
 */
class SequencerMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit SequencerMainWindow(SequencerMainWindowContext& context);
  ~SequencerMainWindow() override;

  SequencerMainWindow(const SequencerMainWindow&) = delete;
  SequencerMainWindow& operator=(const SequencerMainWindow&) = delete;
  SequencerMainWindow(SequencerMainWindow&&) = delete;
  SequencerMainWindow& operator=(SequencerMainWindow&&) = delete;

  bool ImportProcedure(const QString& file_name);

  /**
   * @brief Returns main application context.
   */
  SequencerMainWindowContext& GetContext() const;

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void InitApplication();
  void InitComponents();
  void ReadSettings();
  void WriteSettings();

  /**
   * @brief Check if the application can be closed.
   *
   * This saves unsaved projects and writes persistent application settings.
   *
   * @return True if the application is ready to be closed.
   */
  bool CanCloseApplication();

  /**
   * @brief Check if the application can be restarted and exit the application with the given code.
   *
   * The code will be used to start the application again from the main.
   */
  void OnRestartRequest(sup::gui::AppExitCode exit_code);

  /**
   * @brief Perform widgets setup on new project creation or project load from disk.
   */
  void OnProjectLoad();

  /**
   * @brief Update currently opened project name.
   */
  void UpdateProjectNames();

  std::unique_ptr<ApplicationModels> CreateProject();

  SequencerMainWindowContext& m_context;
  std::unique_ptr<ApplicationModels> m_models;

  SequencerMainWindowActions* m_action_manager{nullptr};
  mvvm::MainVerticalBarWidget* m_tab_widget{nullptr};

  SequencerExplorerView* m_explorer_view{nullptr};
  SequencerComposerView* m_composer_view{nullptr};
  OperationMonitorView* m_operation_view{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_H_
