/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "automation_monitor_view.h"

#include "automation_monitor_tool_bar.h"

#include <sequencergui/automation/automation_client.h>
#include <sequencergui/automation/automation_job_handler.h>
#include <sequencergui/automation/remote_job_observer.h>
#include <sequencergui/jobsystem/abstract_job_handler.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/transform/procedure_item_job_info_builder.h>
#include <sequencergui/views/operation/job_list_widget.h>
#include <sequencergui/views/operation/operation_realtime_panel.h>
#include <sequencergui/views/operation/operation_workspace_panel.h>
#include <sup/gui/widgets/item_stack_widget.h>
#include <sup/gui/widgets/style_utils.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/job_info.h>

#include <QDebug>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>

namespace sequencergui
{

AutomationMonitorView::AutomationMonitorView(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new AutomationMonitorToolBar)
    , m_splitter(new QSplitter)
    , m_job_list(new JobListWidget)
    , m_realtime_panel(new OperationRealTimePanel)
    , m_workspace_panel{new OperationWorkspacePanel}
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_splitter);

  m_splitter->addWidget(CreateLeftPanel());
  m_splitter->addWidget(CreateCentralPanel());
  m_splitter->addWidget(m_workspace_panel);

  m_tool_bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_tool_bar->setIconSize(sup::gui::utils::ToolBarIconSize());

  connect(m_tool_bar, &AutomationMonitorToolBar::ConnectRequest, this,
          &AutomationMonitorView::OnConnect);

  connect(m_job_list, &JobListWidget::JobSelected, this, &AutomationMonitorView::OnJobSelected);

  connect(m_realtime_panel, &OperationRealTimePanel::runRequest, this,
          &AutomationMonitorView::OnRunRequest);
  connect(m_realtime_panel, &OperationRealTimePanel::pauseRequest, this,
          &AutomationMonitorView::OnPauseRequest);
  connect(m_realtime_panel, &OperationRealTimePanel::stepRequest, this,
          &AutomationMonitorView::OnStepRequest);
  connect(m_realtime_panel, &OperationRealTimePanel::stopRequest, this,
          &AutomationMonitorView::OnStopRequest);
}

AutomationMonitorView::~AutomationMonitorView() = default;

void AutomationMonitorView::SetModels(SequencerModel *sequencer_model, JobModel *job_model)
{
  m_sequencer_model = sequencer_model;
  m_job_model = job_model;
  m_job_list->SetJobModel(job_model);
}

void AutomationMonitorView::OnRunRequest()
{
  m_job_handler->OnStartRequest();
}

void AutomationMonitorView::OnPauseRequest()
{
  m_job_handler->OnPauseRequest();
}

void AutomationMonitorView::OnStepRequest()
{
  m_job_handler->OnMakeStepRequest();
}

void AutomationMonitorView::OnStopRequest()
{
  m_job_handler->OnStopRequest();
}

void AutomationMonitorView::OnJobSelected(JobItem *item)
{
  m_realtime_panel->SetCurrentJob(item);
  m_workspace_panel->SetProcedure(item ? item->GetExpandedProcedure() : nullptr);
}

QWidget *AutomationMonitorView::CreateLeftPanel()
{
  auto result = new sup::gui::ItemStackWidget;
  result->AddWidget(m_job_list);
  return result;
}

QWidget *AutomationMonitorView::CreateCentralPanel()
{
  auto result = new sup::gui::ItemStackWidget;
  result->AddWidget(m_realtime_panel, m_realtime_panel->actions());
  return result;
}

void AutomationMonitorView::OnConnect(const QString &server_name)
{
  qDebug() << "RemoteMonitorView::OnConnect()" << server_name;

  m_automation_client.reset();
  m_job_model->Clear();

  try
  {
    m_automation_client = std::make_unique<AutomationClient>(server_name.toStdString());
  }
  catch (const sup::sequencer::InvalidOperationException &ex)
  {
    qDebug() << "Can't connect" << ex.what();
    return;
  }

  if (m_automation_client->GetJobCount() > 0)
  {
    auto job_item = m_job_model->InsertItem<JobItem>();
    m_job_handler = m_automation_client->CreateJobHandler(job_item, 0);
  }
}

}  // namespace sequencergui
