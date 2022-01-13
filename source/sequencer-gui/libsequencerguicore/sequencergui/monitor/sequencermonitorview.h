/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
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

namespace sequi
{
class SequencerModel;
class MonitorPanel;
class MonitorWorkspaceWidget;
class MonitorTreeWidget;
class JobManager;
class ProcedureItem;

//! The SequencerMonitorView is a mainwindow component to run sequences and monitor execution.
//! Contains the panel with jobs on the left, main sequence tree to run in the middle, and
//! the panel to monitor variables on the left.

class SequencerMonitorView : public QWidget
{
  Q_OBJECT

public:
  SequencerMonitorView(QWidget* parent = nullptr);
  ~SequencerMonitorView();

  void SetModel(SequencerModel* model);

private:
  void SetupConnections();
  ProcedureItem* GetFirstProcedure();

  MonitorPanel* m_monitor_panel{nullptr};
  MonitorTreeWidget* m_tree_widget{nullptr};
  MonitorWorkspaceWidget* m_workspace_widget{nullptr};

  QSplitter* m_splitter{nullptr};
  SequencerModel* m_model{nullptr};

  JobManager* m_job_manager{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MONITOR_SEQUENCERMONITORVIEW_H
