/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MONITOR_SEQUENCERMONITORVIEW_H
#define SEQUENCERGUI_MONITOR_SEQUENCERMONITORVIEW_H

#include <QWidget>

class QSplitter;
class QShowEvent;

namespace sequencergui
{
class ApplicationModels;
class MonitorPanel;
class MonitorWorkspaceWidget;
class MonitorRealTimeWidget;
class JobManager;
class ProcedureItem;

//! The SequencerMonitorView is a mainwindow component to run sequences and monitor execution.
//! Contains the panel with jobs on the left, main sequence tree to run in the middle, and
//! the panel to monitor variables on the left.

class SequencerMonitorView : public QWidget
{
  Q_OBJECT

public:
  explicit SequencerMonitorView(QWidget* parent = nullptr);
  ~SequencerMonitorView() override;

  void SetApplicationModels(ApplicationModels* models);

protected:
  void showEvent(QShowEvent* event) override;

private:
  void SetupConnections();

  MonitorPanel* m_monitor_panel{nullptr};
  MonitorRealTimeWidget* m_tree_widget{nullptr};
  MonitorWorkspaceWidget* m_workspace_widget{nullptr};

  QSplitter* m_splitter{nullptr};
  ApplicationModels* m_models{nullptr};

  JobManager* m_job_manager{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_SEQUENCERMONITORVIEW_H
