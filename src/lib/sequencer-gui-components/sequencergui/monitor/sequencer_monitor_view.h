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

#ifndef SEQUENCERGUI_MONITOR_SEQUENCER_MONITOR_VIEW_H_
#define SEQUENCERGUI_MONITOR_SEQUENCER_MONITOR_VIEW_H_

#include <QWidget>

class QSplitter;
class QShowEvent;

namespace sequencergui
{

class ApplicationModels;
class JobItem;
class JobManager;
class OperationWorkspacePanel;
class OperationRealTimePanel;
class OperationJobPanel;
class ProcedureItem;
class OperationActionHandler;

//! The SequencerMonitorView is the main window component to run sequences and monitor execution.
//! Contains the panel with jobs on the left, main sequence tree in the middle, and
//! the panel to monitor variables on the right.

class SequencerMonitorView : public QWidget
{
  Q_OBJECT

public:
  enum Mode
  {
    kOperartionMode,  //!< widget will be a part of sequencer-operation application
    kIdeMode          //!< widget will be a part of sequencer-gui
  };

  explicit SequencerMonitorView(Mode mode, QWidget* parent = nullptr);
  ~SequencerMonitorView() override;

  void SetApplicationModels(ApplicationModels* models);

  void OnImportJobRequest(const QString& file_name = {});

protected:
  void showEvent(QShowEvent* event) override;

private:
  void SetupConnections();
  void OnJobSelected(sequencergui::JobItem* item);

  QWidget* CreateLeftPanel(Mode mode);
  QWidget* CreateCentralPanel();
  QWidget* CreateRightPanel();

  OperationJobPanel* m_job_panel{nullptr};
  OperationRealTimePanel* m_realtime_panel{nullptr};
  OperationWorkspacePanel* m_workspace_panel{nullptr};

  QSplitter* m_splitter{nullptr};
  ApplicationModels* m_models{nullptr};

  JobManager* m_job_manager{nullptr};
  OperationActionHandler* m_actions{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_SEQUENCER_MONITOR_VIEW_H_
