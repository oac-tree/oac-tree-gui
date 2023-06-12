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

#ifndef SEQUENCERGUI_OPERATION_OPERATION_JOB_PANEL_H_
#define SEQUENCERGUI_OPERATION_OPERATION_JOB_PANEL_H_

#include <QWidget>

class QToolBar;

namespace mvvm
{
class CollapsibleListView;
}

namespace sequencergui
{
class JobListWidget;
class JobItem;
class ProcedureItem;
class JobPropertyWidget;
class ApplicationModels;
class OperationJobPanelToolBar;

//! Vertical panel with list of running jobs and job properties at the left side of
//! SequencerMonitorView.

class OperationJobPanel : public QWidget
{
  Q_OBJECT

public:
  explicit OperationJobPanel(QWidget* parent = nullptr);
  ~OperationJobPanel() override;

  void SetApplicationModels(ApplicationModels* models);

  JobItem* GetSelectedJob() const;

  void SetSelectedJob(JobItem* job_item);

  QToolBar* GetToolBar() const;

signals:
  void JobSelected(sequencergui::JobItem* item);
  void RemoveJobRequest();
  void RegenerateJobRequest();

private:
  void OnJobSelectedIntern(JobItem* item);

  mvvm::CollapsibleListView* m_collapsible_list{nullptr};
  JobListWidget* m_job_list_widget{nullptr};
  JobPropertyWidget* m_job_property_widget{nullptr};
  OperationJobPanelToolBar* m_tool_bar{nullptr};

  ApplicationModels* m_models{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_OPERATION_JOB_PANEL_H_
