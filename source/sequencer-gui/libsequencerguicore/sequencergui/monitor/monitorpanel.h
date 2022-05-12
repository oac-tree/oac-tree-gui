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

#ifndef SEQUENCERGUI_MONITOR_MONITORPANEL_H
#define SEQUENCERGUI_MONITOR_MONITORPANEL_H

#include <QWidget>

class QSplitter;

namespace sequencergui
{
class PanelToolBar;
class JobListWidget;
class ProcedureItem;
class SequencerModel;
class CollapsibleListView;

//! Left panel on SequencerMonitorView.
//! Contains widget with open procedures and running jobs.

class MonitorPanel : public QWidget
{
  Q_OBJECT

public:
  explicit MonitorPanel(QWidget* parent = nullptr);
  ~MonitorPanel() override;

  void SetModel(SequencerModel* model);

  ProcedureItem* GetSelectedProcedure() const;

  void SetSelectedProcedure(ProcedureItem* procedure);

signals:
  void procedureSelected(sequencergui::ProcedureItem* procedureItem);

private:
  PanelToolBar* m_tool_bar{nullptr};
  CollapsibleListView* m_collapsible_list{nullptr};
  JobListWidget* m_job_list_widget{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_MONITORPANEL_H
