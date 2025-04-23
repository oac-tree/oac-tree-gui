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

#include "operation_job_panel.h"

#include "job_list_widget.h"
#include "job_property_widget.h"
#include "operation_job_actions.h"

#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>

#include <sup/gui/widgets/collapsible_list_view.h>

#include <QVBoxLayout>

namespace oac_tree_gui
{

OperationJobPanel::OperationJobPanel(QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_collapsible_list(new sup::gui::CollapsibleListView)
    , m_job_list_widget(new JobListWidget)
    , m_job_property_widget(new JobPropertyWidget)
    , m_job_actions(new OperationJobActions(this))
{
  setWindowTitle("Procedures");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_collapsible_list);

  m_collapsible_list->AddWidget(m_job_list_widget);
  // m_collapsible_list->AddCollapsibleWidget(m_job_property_widget, {});

  SetupConnections();

  m_job_actions->SetAvailableProcedures([this]()
                                        { return m_models->GetSequencerModel()->GetProcedures(); });
}

OperationJobPanel::~OperationJobPanel() = default;

void OperationJobPanel::SetModels(ApplicationModels *models)
{
  m_models = models;
  m_job_list_widget->SetJobModel(models->GetJobModel());
}

JobItem *OperationJobPanel::GetSelectedJob() const
{
  return m_job_list_widget->GetSelectedJob();
}

void OperationJobPanel::SetSelectedJob(JobItem *job_item)
{
  m_job_list_widget->SetSelectedJob(job_item);
}

QList<QAction *> OperationJobPanel::GetSequencerMonitorViewActions()
{
  using ActionKey = OperationJobActions::ActionKey;
  return m_job_actions->GetActions(
      {ActionKey::kSubmitJob, ActionKey::kRegenerateJob, ActionKey::kRemoveJob});
}

QList<QAction *> OperationJobPanel::GetOperationMonitorViewActions()
{
  using ActionKey = OperationJobActions::ActionKey;
  return m_job_actions->GetActions({ActionKey::kImportProcedure, ActionKey::kConnect,
                                    ActionKey::kRegenerateJob, ActionKey::kRemoveJob});
}

void OperationJobPanel::SetupConnections()
{
  connect(m_job_list_widget, &JobListWidget::JobSelected, this,
          &OperationJobPanel::OnJobSelectedIntern);

  connect(m_job_actions, &OperationJobActions::SubmitProcedureRequest, this,
          &OperationJobPanel::SubmitProcedureRequest);
  connect(m_job_actions, &OperationJobActions::ImportJobRequest, this,
          &OperationJobPanel::ImportJobRequest);
  connect(m_job_actions, &OperationJobActions::RegenerateJobRequest, this,
          &OperationJobPanel::RegenerateJobRequest);
  connect(m_job_actions, &OperationJobActions::RemoveJobRequest, this,
          &OperationJobPanel::RemoveJobRequest);
  connect(m_job_actions, &OperationJobActions::ConnectRequest, this,
          &OperationJobPanel::ConnectRequest);
}

void OperationJobPanel::OnJobSelectedIntern(JobItem *item)
{
  m_job_property_widget->SetJob(item);
  emit JobSelected(item);
}

}  // namespace oac_tree_gui
