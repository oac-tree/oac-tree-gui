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
class JobItem;
class CollapsibleListView;
class JobPropertyWidget;
class ApplicationModels;

//! Vertical panel with list of running jobs and job properties at the left side of
//! SequencerMonitorView.

class MonitorPanel : public QWidget
{
  Q_OBJECT

public:
  explicit MonitorPanel(QWidget* parent = nullptr);
  ~MonitorPanel() override;

  void SetApplicationModels(ApplicationModels* models);

  JobItem* GetSelectedJob() const;

  void SetSelectedJob(JobItem* job_item);

signals:
  void JobSelected(sequencergui::JobItem* item);

private:
  void OnJobSelectedIntern(JobItem* item);

  PanelToolBar* m_tool_bar{nullptr};
  CollapsibleListView* m_collapsible_list{nullptr};
  JobListWidget* m_job_list_widget{nullptr};
  JobPropertyWidget* m_job_property_widget{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_MONITORPANEL_H
