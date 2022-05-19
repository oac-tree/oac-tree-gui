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

#include "sequencergui/monitor/sequencermonitorview.h"

#include "sequencergui/model/applicationmodels.h"
#include "sequencergui/model/instructionitem.h"
#include "sequencergui/model/jobitem.h"
#include "sequencergui/model/jobmodel.h"
#include "sequencergui/model/procedureitem.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/monitor/jobcontext.h"
#include "sequencergui/monitor/jobmanager.h"
#include "sequencergui/monitor/monitorpanel.h"
#include "sequencergui/monitor/monitorrealtimewidget.h"
#include "sequencergui/monitor/monitorworkspacewidget.h"
#include "sequencergui/utils/styleutils.h"

#include "mvvm/standarditems/containeritem.h"
#include "mvvm/widgets/allitemstreeview.h"
#include "mvvm/widgets/topitemstreeview.h"

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
          &JobManager::onPauseProcedureRequest);

  // Stop request from MonitorTreeWidget to JobManager
  connect(m_realtime_widget, &MonitorRealTimeWidget::stopRequest, m_job_manager,
          &JobManager::onStopProcedureRequest);

  // Process step button click
  auto on_step = [this]() { m_job_manager->onMakeStepRequest(); };
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
  m_job_manager->SetCurrentJob(item);
  if (auto context = m_job_manager->GetCurrentContext(); context)
  {
    m_realtime_widget->SetProcedure(context->GetExpandedProcedure());
    m_workspace_widget->SetProcedure(context->GetExpandedProcedure());
  }
}

//! Submits given procedure for execution.
void SequencerMonitorView::OnSubmitProcedureRequest(ProcedureItem *item)
{
  auto job = m_models->GetJobModel()->InsertItem<JobItem>();
  job->SetProcedure(item);
  m_monitor_panel->SetSelectedJob(job);
}

void SequencerMonitorView::OnRunJobRequest()
{
  // FIXME simplify this

  auto selected_job = m_monitor_panel->GetSelectedJob();
  if (!selected_job)
  {
    return;
  }
  m_job_manager->SetCurrentJob(selected_job);
  auto context = m_job_manager->GetCurrentContext();

  if (!context->IsRunning())
  {
    context->onPrepareJobRequest();
    m_realtime_widget->SetProcedure(context->GetExpandedProcedure());
    m_workspace_widget->SetProcedure(context->GetExpandedProcedure());
  }

  m_job_manager->onStartProcedureRequest(selected_job);
}

}  // namespace sequencergui
