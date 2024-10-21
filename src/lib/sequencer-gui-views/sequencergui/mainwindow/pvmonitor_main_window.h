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

#ifndef SEQUENCERGUI_MAINWINDOW_PVMONITOR_MAIN_WINDOW_H_
#define SEQUENCERGUI_MAINWINDOW_PVMONITOR_MAIN_WINDOW_H_

#include <QMainWindow>
#include <memory>

class QCloseEvent;

namespace sequencergui
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

  /**
   * @brief Perform widgets setup on project modification.
   */
  void OnProjectModified();

  /**
   * @brief Creates main application project agent.
   */
  std::unique_ptr<PvMonitorProject> CreateProjectAgent();


  std::unique_ptr<PvMonitorProject> m_project_agent;
  MonitorWidget* m_monitor_widget{nullptr};
  MonitorMainWindowActions* m_actions{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_PVMONITOR_MAIN_WINDOW_H_
