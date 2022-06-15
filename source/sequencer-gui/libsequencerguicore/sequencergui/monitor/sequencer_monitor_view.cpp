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

#include "sequencergui/monitor/sequencer_monitor_view.h"

#include "sequencergui/jobsystem/job_context.h"
#include "sequencergui/jobsystem/job_manager.h"
#include "sequencergui/model/application_models.h"
#include "sequencergui/model/instruction_item.h"
#include "sequencergui/model/job_item.h"
#include "sequencergui/model/job_model.h"
#include "sequencergui/model/procedure_item.h"
#include "sequencergui/model/sequencer_model.h"
#include "sequencergui/monitor/monitor_panel.h"
#include "sequencergui/monitor/monitor_realtime_widget.h"
#include "sequencergui/monitor/monitor_workspace_widget.h"
#include "sequencergui/monitor/sequencer_monitor_actions.h"
#include "sequencergui/utils/style_utils.h"

#include "mvvm/standarditems/container_item.h"
#include "mvvm/widgets/all_items_tree_view.h"
#include "mvvm/widgets/top_items_tree_view.h"

#include <QDebug>
#include <QSplitter>
#include <QVBoxLayout>

namespace sequencergui
{

SequencerMonitorView::SequencerMonitorView(QWidget *parent)
    : QWidget(parent)
    , m_monitor_panel(new MonitorPanel)
    , m_realtime_widget(new MonitorRealTimeWidget)
    , m_workspace_widget(new MonitorWorkspaceWidget)
    , m_splitter(new QSplitter)
    , m_job_manager(new JobManager(this))
    , m_actions(new SequencerMonitorActions(
          m_job_manager, [this] { return m_monitor_panel->GetSelectedJob(); }, this))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);
  //  layout->setSpacing(0);
  //  layout->setMargin(0);

  m_splitter->addWidget(m_monitor_panel);
  m_splitter->addWidget(m_realtime_widget);
  m_splitter->addWidget(m_workspace_widget);
  m_splitter->setSizes(QList<int>() << styleutils::UnitSize(30) << styleutils::UnitSize(90)
                                    << styleutils::UnitSize(30));

  layout->addWidget(m_splitter);

  SetupConnections();

  m_job_manager->SetMessagePanel(m_realtime_widget->GetMessagePanel());
}

SequencerMonitorView::~SequencerMonitorView() = default;

void SequencerMonitorView::SetApplicationModels(ApplicationModels *models)
{
  m_models = models;
  m_monitor_panel->SetApplicationModels(models);
  m_actions->SetJobModel(models->GetJobModel());
}

void SequencerMonitorView::showEvent(QShowEvent *event)
{
  Q_UNUSED(event);
  if (!m_monitor_panel->GetSelectedJob())
  {
    if (auto job = m_models->GetJobModel()->GetTopItem<JobItem>(); job)
    {
      m_monitor_panel->SetSelectedJob(job);
    }
  }
}

void SequencerMonitorView::SetupConnections()
{
  // Process request from MonitorTreeWidget to JobManager
  connect(m_realtime_widget, &MonitorRealTimeWidget::runRequest, this,
          &SequencerMonitorView::OnRunJobRequest);

  // Pause request from MonitorTreeWidget to JobManager
  connect(m_realtime_widget, &MonitorRealTimeWidget::pauseRequest, m_job_manager,
          &JobManager::OnPauseJobRequest);

  // Stop request from MonitorTreeWidget to JobManager
  connect(m_realtime_widget, &MonitorRealTimeWidget::stopRequest, m_job_manager,
          &JobManager::OnStopJobRequest);

  // Process step button click
  auto on_step = [this]() { m_job_manager->OnMakeStepRequest(); };
  connect(m_realtime_widget, &MonitorRealTimeWidget::stepRequest, this, on_step);

  // Selection request from JobManager to this
  auto on_selection = [this](auto instruction)
  { m_realtime_widget->SetSelectedInstruction(instruction); };
  connect(m_job_manager, &JobManager::InstructionStatusChanged, this, on_selection);

  connect(m_realtime_widget, &MonitorRealTimeWidget::changeDelayRequest, m_job_manager,
          &JobManager::onChangeDelayRequest);

  connect(m_monitor_panel, &MonitorPanel::JobSelected, this, &SequencerMonitorView::OnJobSelected);

  connect(m_monitor_panel, &MonitorPanel::SubmitProcedureRequest, this,
          &SequencerMonitorView::OnSubmitProcedureRequest);
}

//! Setup widgets to show currently selected job.
void SequencerMonitorView::OnJobSelected(JobItem *item)
{
  qDebug() << "SequencerMonitorView::OnJobSelected JobItem" << item;
  m_job_manager->SetCurrentJob(item);
  m_realtime_widget->SetProcedure(item ? item->GetExpandedProcedure() : nullptr);
  m_workspace_widget->SetProcedure(item ? item->GetExpandedProcedure() : nullptr);
}

//! Submits given procedure for execution.
void SequencerMonitorView::OnSubmitProcedureRequest(ProcedureItem *item)
{
  auto job = m_models->GetJobModel()->InsertItem<JobItem>();
  qDebug() << "OnSubmitProcedureRequest 1.1 ProcedureItem" << item << " job " << job;
  job->SetProcedure(item);
  m_job_manager->SubmitJob(job);
  m_monitor_panel->SetSelectedJob(job);
}

void SequencerMonitorView::OnRunJobRequest()
{
  auto selected_job = m_monitor_panel->GetSelectedJob();
  if (!selected_job)
  {
    return;
  }

  m_job_manager->OnStartJobRequest();
}

}  // namespace sequencergui
