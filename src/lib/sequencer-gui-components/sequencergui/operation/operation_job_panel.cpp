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

#include "operation_job_panel.h"

#include "operation_job_panel_actions.h"

#include <sequencergui/model/application_models.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/monitor/job_list_widget.h>
#include <sequencergui/monitor/job_property_widget.h>

#include <mvvm/widgets/collapsible_list_view.h>

#include <QVBoxLayout>

namespace sequencergui
{

OperationJobPanel::OperationJobPanel(QWidget *parent)
    : QWidget(parent)
    , m_collapsible_list(new mvvm::CollapsibleListView)
    , m_job_list_widget(new JobListWidget)
    , m_job_property_widget(new JobPropertyWidget)
    , m_job_actions(new OperationJobPanelActions(this))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_collapsible_list);

  m_collapsible_list->AddWidget(m_job_list_widget);
  //  m_collapsible_list->AddCollapsibleWidget(m_job_property_widget, {});

  SetupConnections();

  m_job_actions->SetAvailableProcedures([this]()
                                        { return m_models->GetSequencerModel()->GetProcedures(); });
}

OperationJobPanel::~OperationJobPanel() = default;

void OperationJobPanel::SetApplicationModels(ApplicationModels *models)
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
  return m_job_actions->GetSequencerMonitorViewActions();
}

QList<QAction *> OperationJobPanel::GetOperationMonitorViewActions()
{
  return m_job_actions->GetOperationMonitorViewActions();
}

void OperationJobPanel::SetupConnections()
{
  connect(m_job_list_widget, &JobListWidget::JobSelected, this,
          &OperationJobPanel::OnJobSelectedIntern);

  connect(m_job_actions, &OperationJobPanelActions::SubmitProcedureRequest, this,
          &OperationJobPanel::SubmitProcedureRequest);
  connect(m_job_actions, &OperationJobPanelActions::ImportJobRequest, this,
          &OperationJobPanel::ImportJobRequest);
  connect(m_job_actions, &OperationJobPanelActions::RegenerateJobRequest, this,
          &OperationJobPanel::RegenerateJobRequest);
  connect(m_job_actions, &OperationJobPanelActions::RemoveJobRequest, this,
          &OperationJobPanel::RemoveJobRequest);
  connect(m_job_actions, &OperationJobPanelActions::RemoveAndCleanupJobRequest, this,
          &OperationJobPanel::RemoveAndCleanupJobRequest);
}

void OperationJobPanel::OnJobSelectedIntern(JobItem *item)
{
  m_job_property_widget->SetJob(item);
  emit JobSelected(item);
}

}  // namespace sequencergui
