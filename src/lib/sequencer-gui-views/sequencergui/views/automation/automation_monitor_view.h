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

#ifndef SEQUENCERGUI_VIEWS_AUTOMATION_AUTOMATION_MONITOR_VIEW_H_
#define SEQUENCERGUI_VIEWS_AUTOMATION_AUTOMATION_MONITOR_VIEW_H_

#include <QWidget>
#include <memory>

class QSplitter;

namespace sequencergui
{

class AutomationClient;
class AutomationMonitorToolBar;
class JobListWidget;
class OperationRealTimePanel;
class ApplicationModels;
class SequencerModel;
class JobModel;
class AutomationJobHandler;
class OperationWorkspacePanel;
class JobItem;

/**
 * @brief The AutomationMonitorView class is a central view of RemoteMainWindow.
 */
class AutomationMonitorView : public QWidget
{
  Q_OBJECT

public:
  explicit AutomationMonitorView(QWidget* parent = nullptr);
  ~AutomationMonitorView() override;

  void SetApplicationModels(ApplicationModels* models);

private:
  void OnRunRequest();
  void OnPauseRequest();
  void OnStepRequest();
  void OnStopRequest();
  void OnJobSelected(sequencergui::JobItem* item);

  QWidget* CreateLeftPanel();
  QWidget* CreateCentralPanel();

  AutomationMonitorToolBar* m_tool_bar{nullptr};
  QSplitter* m_splitter{nullptr};
  JobListWidget* m_job_list{nullptr};
  OperationRealTimePanel* m_realtime_panel{nullptr};
  OperationWorkspacePanel* m_workspace_panel{nullptr};

  void OnConnect(const QString& server_name);

  SequencerModel* m_sequencer_model{nullptr};
  JobModel* m_job_model{nullptr};
  std::unique_ptr<AutomationClient> m_automation_client;
  std::vector<std::unique_ptr<AutomationJobHandler>> m_job_handlers;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_AUTOMATION_AUTOMATION_MONITOR_VIEW_H_
