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

#ifndef SEQUENCERGUI_MAINWINDOW_PVMONITOR_MAIN_WINDOW_H_
#define SEQUENCERGUI_MAINWINDOW_PVMONITOR_MAIN_WINDOW_H_

#include <QMainWindow>
#include <memory>

class QCloseEvent;

namespace oac_tree_gui
{

class MonitorWidget;
class PvMonitorProject;
class MonitorMainWindowActions;

/**
 * @brief The SequencerMainWindow class is the main window of sup-pvmonitor application.
 */
class PvMonitorMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  PvMonitorMainWindow();
  ~PvMonitorMainWindow() override;

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void InitApplication();
  void ReadSettings();
  void WriteSettings();

  /**
   * @brief Perform widgets setup on new project creation or project load from disk.
   */
  void OnProjectLoad();

  void UpdateProjectNames();

  std::unique_ptr<PvMonitorProject> CreateProject();

  std::unique_ptr<PvMonitorProject> m_project;
  MonitorWidget* m_monitor_widget{nullptr};
  MonitorMainWindowActions* m_actions{nullptr};
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_MAINWINDOW_PVMONITOR_MAIN_WINDOW_H_
