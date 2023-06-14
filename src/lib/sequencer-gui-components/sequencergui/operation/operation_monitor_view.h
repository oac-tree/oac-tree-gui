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

#ifndef SEQUENCERGUI_OPERATION_OPERATION_MONITOR_VIEW_H_
#define SEQUENCERGUI_OPERATION_OPERATION_MONITOR_VIEW_H_

#include <QWidget>

class QMainWindow;
class QSplitter;

namespace sequencergui
{

class OperationJobPanel;
class OperationRealTimePanel;
class OperationWorkspacePanel;
class JobItem;
class ApplicationModels;
class JobManager;
class SequencerMonitorActions;

//! Main view of operation application.
//! FIXME Almost a full duplicate of SequencerMonitorView. Merge after prototyping.

class OperationMonitorView : public QWidget
{
  Q_OBJECT

public:
  explicit OperationMonitorView(QMainWindow* parent = nullptr);
  ~OperationMonitorView() override;

  void SetApplicationModels(ApplicationModels* models);

private:
  void ReadSettings();
  void WriteSettings();
  void SetupConnections();
  void OnJobSelected(sequencergui::JobItem* item);
  void OnImportJobRequest();

  QSplitter* m_splitter{nullptr};
  OperationJobPanel* m_job_panel{nullptr};
  OperationRealTimePanel* m_realtime_panel{nullptr};
  OperationWorkspacePanel* m_workspace_panel{nullptr};

  ApplicationModels* m_models{nullptr};

  JobManager* m_job_manager{nullptr};
  SequencerMonitorActions* m_actions{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_OPERATION_MONITOR_VIEW_H_
