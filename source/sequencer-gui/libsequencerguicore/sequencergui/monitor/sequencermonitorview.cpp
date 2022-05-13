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

#include "mvvm/standarditems/containeritem.h"
#include "mvvm/widgets/allitemstreeview.h"
#include "mvvm/widgets/topitemstreeview.h"

#include <QSplitter>
#include <QVBoxLayout>

namespace sequencergui
{

SequencerMonitorView::SequencerMonitorView(QWidget *parent)
    : QWidget(parent)
    , m_monitor_panel(new MonitorPanel)
    , m_tree_widget(new MonitorRealTimeWidget)
    , m_workspace_widget(new MonitorWorkspaceWidget)
    , m_splitter(new QSplitter)
    , m_job_manager(new JobManager(this))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);
  //  layout->setSpacing(0);
  //  layout->setMargin(0);

  m_splitter->addWidget(m_monitor_panel);
  m_splitter->addWidget(m_tree_widget);
  m_splitter->addWidget(m_workspace_widget);
  m_splitter->setSizes(QList<int>() << 50 << 300 << 120);

  layout->addWidget(m_splitter);

  SetupConnections();

  m_job_manager->SetMessagePanel(m_tree_widget->GetMessagePanel());
}

SequencerMonitorView::~SequencerMonitorView() = default;

void SequencerMonitorView::SetApplicationModels(ApplicationModels *models)
{
  m_models = models;
  m_job_manager->SetModel(models->GetSequencerModel());
  m_monitor_panel->SetJobModel(models->GetJobModel());
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
  // FIXME restore
  //  auto on_start = [this]()
  //  { m_job_manager->onStartProcedureRequest(m_monitor_panel->GetSelectedProcedure()); };
  //  connect(m_tree_widget, &MonitorRealTimeWidget::runRequest, this, on_start);

  // Pause request from MonitorTreeWidget to JobManager
  connect(m_tree_widget, &MonitorRealTimeWidget::pauseRequest, m_job_manager,
          &JobManager::onPauseProcedureRequest);

  // Stop request from MonitorTreeWidget to JobManager
  connect(m_tree_widget, &MonitorRealTimeWidget::stopRequest, m_job_manager,
          &JobManager::onStopProcedureRequest);

  // Process step button click
  auto on_step = [this]() { m_job_manager->onMakeStepRequest(); };
  connect(m_tree_widget, &MonitorRealTimeWidget::stepRequest, this, on_step);

  // Selection request from JobManager to this
  auto on_selection = [this](auto instruction)
  { m_tree_widget->SetSelectedInstruction(instruction); };
  connect(m_job_manager, &JobManager::InstructionStatusChanged, this, on_selection);

  // FIXME restore
  //  auto on_procedure_selected = [this](auto procedure_item)
  //  {
  //    m_job_manager->SetCurrentProcedure(procedure_item);
  //    if (auto context = m_job_manager->GetCurrentContext(); context)
  //    {
  //      m_tree_widget->SetProcedure(context->GetExpandedProcedure());
  //      m_workspace_widget->SetProcedure(context->GetExpandedProcedure());
  //    }
  //  };
  //  connect(m_monitor_panel, &MonitorPanel::procedureSelected, this, on_procedure_selected);

  connect(m_tree_widget, &MonitorRealTimeWidget::changeDelayRequest, m_job_manager,
          &JobManager::onChangeDelayRequest);
}

ProcedureItem *SequencerMonitorView::GetFirstProcedure()
{
  auto procedure_container = m_models->GetSequencerModel()->GetProcedureContainer();
  return procedure_container->IsEmpty() ? nullptr : procedure_container->GetItem<ProcedureItem>("");
}

}  // namespace sequencergui
