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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_JOB_PANEL_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_JOB_PANEL_H_

#include <QWidget>

namespace sup::gui
{
class CollapsibleListView;
}

namespace oac_tree_gui
{

class JobListWidget;
class JobItem;
class ProcedureItem;
class JobPropertyWidget;
class ApplicationModels;
class OperationJobActions;

/**
 * @brief The OperationJobPanel class represents a vertical panel with list of running jobs and job
 * properties on the left side of OperationMonitorView
 */
class OperationJobPanel : public QWidget
{
  Q_OBJECT

public:
  explicit OperationJobPanel(QWidget* parent_widget = nullptr);
  ~OperationJobPanel() override;

  void SetModels(ApplicationModels* models);

  JobItem* GetSelectedJob() const;

  void SetSelectedJob(JobItem* job_item);

  QList<QAction*> GetSequencerMonitorViewActions();

  QList<QAction*> GetOperationMonitorViewActions();

signals:
  void JobSelected(oac_tree_gui::JobItem* item);
  void SubmitProcedureRequest(oac_tree_gui::ProcedureItem* item);
  void ImportJobRequest();
  void RegenerateJobRequest();
  void RemoveJobRequest();
  void ConnectRequest();

private:
  void SetupConnections();
  void OnJobSelectedIntern(JobItem* item);

  sup::gui::CollapsibleListView* m_collapsible_list{nullptr};
  JobListWidget* m_job_list_widget{nullptr};
  JobPropertyWidget* m_job_property_widget{nullptr};
  OperationJobActions* m_job_actions{nullptr};

  ApplicationModels* m_models{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_JOB_PANEL_H_
