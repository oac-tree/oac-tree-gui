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

#include "sequencer_monitor_view.h"

#include <sequencergui/components/message_handler_factory.h>
#include <sequencergui/jobsystem/job_manager.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/monitor/operation_action_handler.h>
#include <sequencergui/operation/operation_job_panel.h>
#include <sequencergui/operation/operation_realtime_panel.h>
#include <sequencergui/operation/operation_workspace_panel.h>
#include <sequencergui/widgets/item_stack_widget.h>
#include <sequencergui/widgets/style_utils.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/widgets/all_items_tree_view.h>
#include <mvvm/widgets/top_items_tree_view.h>
#include <mvvm/widgets/widget_utils.h>

#include <QDebug>
#include <QSplitter>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidgetAction>

namespace sequencergui
{

SequencerMonitorView::SequencerMonitorView(QWidget *parent)
    : QWidget(parent)
    , m_job_panel(new OperationJobPanel)
    , m_realtime_panel(new OperationRealTimePanel)
    , m_workspace_panel(new OperationWorkspacePanel)
    , m_splitter(new QSplitter)
    , m_job_manager(new JobManager(this))
    , m_actions(new OperationActionHandler(
          m_job_manager, [this] { return m_job_panel->GetSelectedJob(); }, this))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);

  m_splitter->addWidget(CreateLeftPanel());
  m_splitter->addWidget(CreateCentralPanel());
  m_splitter->addWidget(CreateRightPanel());
  m_splitter->setSizes(QList<int>() << mvvm::utils::UnitSize(30) << mvvm::utils::UnitSize(90)
                                    << mvvm::utils::UnitSize(30));

  layout->addWidget(m_splitter);

  SetupConnections();

  m_actions->SetMessageHandler(CreateMessageBoxHandler());
  m_job_manager->SetMessagePanel(m_realtime_panel->GetMessagePanel());

  // FIXME temporary solution to not to show button "Import Procedure"
  m_job_panel->GetToolBar()->actions().at(0)->setVisible(false);
}

SequencerMonitorView::~SequencerMonitorView() = default;

void SequencerMonitorView::SetApplicationModels(ApplicationModels *models)
{
  m_models = models;
  m_job_panel->SetApplicationModels(models);
  m_actions->SetJobModel(models->GetJobModel());
}

void SequencerMonitorView::showEvent(QShowEvent *event)
{
  Q_UNUSED(event);
  if (!m_job_panel->GetSelectedJob())
  {
    if (auto job = ::mvvm::utils::GetTopItem<JobItem>(m_models->GetJobModel()); job)
    {
      m_job_panel->SetSelectedJob(job);
    }
  }
}

void SequencerMonitorView::SetupConnections()
{
  // Process request from MonitorRealTimeWidget to SequencerMonitorActions

  // start request
  connect(m_realtime_panel, &OperationRealTimePanel::runRequest, m_actions,
          &OperationActionHandler::OnStartJobRequest);

  // pause request
  connect(m_realtime_panel, &OperationRealTimePanel::pauseRequest, m_actions,
          &OperationActionHandler::OnPauseJobRequest);

  // stop request
  connect(m_realtime_panel, &OperationRealTimePanel::stopRequest, m_actions,
          &OperationActionHandler::OnStopJobRequest);

  // step request
  connect(m_realtime_panel, &OperationRealTimePanel::stepRequest, m_actions,
          &OperationActionHandler::OnMakeStepRequest);

  // change delay request from MonitorRealTimeWidget to JobManager
  connect(m_realtime_panel, &OperationRealTimePanel::changeDelayRequest, m_job_manager,
          &JobManager::OnChangeDelayRequest);

  // instruction next leave request from JobManager to MonitorRealTimeWidget
  connect(m_job_manager, &JobManager::NextLeavesChanged, m_realtime_panel,
          &OperationRealTimePanel::SetSelectedInstructions);

  // job selection request from MonitorPanel
  connect(m_job_panel, &OperationJobPanel::JobSelected, this, &SequencerMonitorView::OnJobSelected);

  // job submission request
  connect(m_job_panel, &OperationJobPanel::SubmitProcedureRequest, m_actions,
          &OperationActionHandler::OnSubmitJobRequest);

  // job removal request
  connect(m_job_panel, &OperationJobPanel::RemoveJobRequest, m_actions,
          &OperationActionHandler::OnRemoveJobRequest);

  // job regenerate request
  connect(m_job_panel, &OperationJobPanel::RegenerateJobRequest, m_actions,
          &OperationActionHandler::OnRegenerateJobRequest);

  // job selection request from SequencerMonitorActions
  connect(m_actions, &OperationActionHandler::MakeJobSelectedRequest, m_job_panel,
          &OperationJobPanel::SetSelectedJob);
}

//! Setup widgets to show currently selected job.
void SequencerMonitorView::OnJobSelected(JobItem *item)
{
  m_job_manager->SetCurrentJob(item);
  m_realtime_panel->SetProcedure(item ? item->GetExpandedProcedure() : nullptr);
  m_workspace_panel->SetProcedure(item ? item->GetExpandedProcedure() : nullptr);
}

QWidget *SequencerMonitorView::CreateLeftPanel()
{
  // tuning tool bar to place it into tool bar of ItemStackWidget
  auto toolbar = m_job_panel->GetToolBar();
  toolbar->layout()->setContentsMargins(0, 0, 0, 0);
  toolbar->layout()->setSpacing(0);
  auto widget_action = new QWidgetAction(this);
  widget_action->setDefaultWidget(toolbar);

  auto result = new ItemStackWidget;
  result->AddWidget(m_job_panel, {widget_action});
  return result;
}

//! Create central panel with single OperationRealTimePanel.

QWidget *SequencerMonitorView::CreateCentralPanel()
{
  // tuning tool bar to place it into tool bar of ItemStackWidget
  auto toolbar = m_realtime_panel->GetToolBar();
  toolbar->layout()->setContentsMargins(0, 0, 0, 0);
  toolbar->layout()->setSpacing(0);
  auto widget_action = new QWidgetAction(this);
  widget_action->setDefaultWidget(toolbar);

  auto result = new ItemStackWidget;
  result->AddWidget(m_realtime_panel, {widget_action});
  return result;
}

QWidget *SequencerMonitorView::CreateRightPanel()
{
  auto result = new ItemStackWidget;
  result->AddWidget(m_workspace_panel);
  return result;
}

}  // namespace sequencergui
