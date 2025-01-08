/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "operation_monitor_view.h"

#include "operation_job_panel.h"
#include "operation_realtime_panel.h"
#include "operation_workspace_panel.h"

#include <sequencergui/jobsystem/automation_client.h>
#include <sequencergui/jobsystem/job_manager.h>
#include <sequencergui/jobsystem/local_job_handler.h>
#include <sequencergui/jobsystem/remote_connection_info.h>
#include <sequencergui/jobsystem/remote_connection_service.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/operation/operation_action_handler.h>
#include <sequencergui/operation/operation_action_helper.h>
#include <sequencergui/style/style_helper.h>
#include <sequencergui/views/editors/user_input_dialogs.h>
#include <sequencergui/views/operation/procedure_action_handler.h>
#include <sequencergui/views/operation/remote_connection_dialog.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/widgets/custom_splitter.h>
#include <sup/gui/widgets/item_stack_widget.h>
#include <sup/gui/widgets/message_helper.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>

#include <QToolBar>
#include <QVBoxLayout>

namespace sequencergui
{

namespace
{
const QString kGroupName("OperationMonitorView");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";

std::function<std::unique_ptr<IAutomationClient>(const std::string &)> GetClientFactoryFunc()
{
  return [](const std::string &server_name)
  { return std::make_unique<AutomationClient>(server_name); };
}

}  // namespace

OperationMonitorView::OperationMonitorView(OperationPresentationMode mode, QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_presentation_mode(mode)
    , m_job_panel(new OperationJobPanel)
    , m_realtime_panel(new OperationRealTimePanel)
    , m_left_panel(CreateLeftPanel())
    , m_workspace_panel{new OperationWorkspacePanel}
    , m_splitter(new sup::gui::CustomSplitter(kSplitterSettingName))
    , m_connection_service(std::make_unique<RemoteConnectionService>(GetClientFactoryFunc()))
    , m_job_manager(new JobManager(
          GetJobHandlerFactoryFunc(CreateDefaultUserContext(this), *m_connection_service), this))
    , m_action_handler(new OperationActionHandler(m_job_manager, CreateOperationContext(), this))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);

  m_splitter->addWidget(m_left_panel);
  m_splitter->addWidget(CreateCentralPanel());
  m_splitter->addWidget(m_workspace_panel);
  m_splitter->setSizes({300, 900, 300});

  layout->addWidget(m_splitter);

  SetupConnections();
  SetupWidgetActions();

  ReadSettings();
}

OperationMonitorView::~OperationMonitorView()
{
  WriteSettings();
}

void OperationMonitorView::SetModels(ApplicationModels *models)
{
  m_models = models;
  m_job_panel->SetModels(models);
  m_action_handler->SetJobContainer(models->GetJobModel()->GetRootItem());
}

bool OperationMonitorView::OnImportJobRequest(const QString &file_name)
{
  ProcedureActionHandler handler;
  return m_action_handler->SubmitImportedJob(handler.LoadProcedureFromFile(file_name));
}

bool OperationMonitorView::HasRunningJobs() const
{
  return m_job_manager->HasRunningJobs();
}

void OperationMonitorView::StopAllJobs()
{
  m_job_manager->StopAllJobs();
}

void OperationMonitorView::showEvent(QShowEvent *event)
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

void OperationMonitorView::closeEvent(QCloseEvent *event)
{
  Q_UNUSED(event);
  WriteSettings();
}

void OperationMonitorView::ReadSettings()
{
  m_splitter->ReadSettings();
}

void OperationMonitorView::WriteSettings()
{
  m_splitter->WriteSettings();
}

void OperationMonitorView::SetupConnections()
{
  // Process request from MonitorRealTimeWidget to SequencerMonitorActions

  // start request
  connect(m_realtime_panel, &OperationRealTimePanel::RunRequest, m_action_handler,
          &OperationActionHandler::OnStartJobRequest);

  // pause request
  connect(m_realtime_panel, &OperationRealTimePanel::PauseRequest, m_action_handler,
          &OperationActionHandler::OnPauseJobRequest);

  // step request
  connect(m_realtime_panel, &OperationRealTimePanel::StepRequest, m_action_handler,
          &OperationActionHandler::OnMakeStepRequest);

  // stop request
  connect(m_realtime_panel, &OperationRealTimePanel::StopRequest, m_action_handler,
          &OperationActionHandler::OnStopJobRequest);

  // reset request
  connect(m_realtime_panel, &OperationRealTimePanel::ResetRequest, m_action_handler,
          &OperationActionHandler::OnResetJobRequest);

  // change delay request
  connect(m_realtime_panel, &OperationRealTimePanel::ChangeDelayRequest, m_action_handler,
          &OperationActionHandler::OnSetTickTimeoutRequest);
  m_action_handler->OnSetTickTimeoutRequest(m_realtime_panel->GetCurrentTickTimeout());

  // instruction next leave request from JobManager to OperationRealTimePanel
  connect(m_job_manager, &JobManager::NextLeavesChanged, m_realtime_panel,
          &OperationRealTimePanel::SetSelectedInstructions);

  // job selection request from MonitorPanel
  connect(m_job_panel, &OperationJobPanel::JobSelected, this, &OperationMonitorView::OnJobSelected);

  // job submission request
  connect(m_job_panel, &OperationJobPanel::SubmitProcedureRequest, m_action_handler,
          &OperationActionHandler::SubmitLocalJob);

  // import request
  connect(m_job_panel, &OperationJobPanel::ImportJobRequest, this,
          [this]() { OnImportJobRequest(); });

  // remote server connect request
  connect(m_job_panel, &OperationJobPanel::ConnectRequest, m_action_handler,
          &OperationActionHandler::OnImportRemoteJobRequest);

  // remove job request
  connect(m_job_panel, &OperationJobPanel::RemoveJobRequest, m_action_handler,
          &OperationActionHandler::OnRemoveJobRequest);

  // job regenerate request
  connect(m_job_panel, &OperationJobPanel::RegenerateJobRequest, m_action_handler,
          &OperationActionHandler::OnRegenerateJobRequest);

  // job selection request from SequencerMonitorActions
  connect(m_action_handler, &OperationActionHandler::MakeJobSelectedRequest, m_job_panel,
          &OperationJobPanel::SetSelectedJob);

  connect(m_realtime_panel, &OperationRealTimePanel::ToggleBreakpointRequest, m_action_handler,
          &OperationActionHandler::OnToggleBreakpoint);
}

void OperationMonitorView::SetupWidgetActions()
{
  m_show_left_sidebar = new QAction("Show/hide Left Sidebar", this);
  m_show_left_sidebar->setShortcut(QKeySequence(QString("Ctrl+0")));
  m_show_left_sidebar->setStatusTip("Show/hide Left Sidebar");
  m_show_left_sidebar->setIcon(FindIcon("dock-left"));
  connect(m_show_left_sidebar, &QAction::triggered, this,
          [this](auto) { m_left_panel->setVisible(!m_left_panel->isVisible()); });

  m_show_right_sidebar = new QAction("Show/hide Right Sidebar", this);
  m_show_right_sidebar->setShortcut(QKeySequence(QString("Ctrl+Shift+0")));
  m_show_right_sidebar->setStatusTip("Show/hide Right Sidebar");
  m_show_right_sidebar->setIcon(FindIcon("dock-right"));
  connect(m_show_right_sidebar, &QAction::triggered, this,
          [this](auto) { m_workspace_panel->setVisible(!m_workspace_panel->isVisible()); });

  sup::gui::AppRegisterAction(sup::gui::constants::kViewMenu, m_show_left_sidebar);
  sup::gui::AppRegisterAction(sup::gui::constants::kViewMenu, m_show_right_sidebar);
}

//! Setup widgets to show currently selected job.
void OperationMonitorView::OnJobSelected(JobItem *item)
{
  m_job_manager->SetActiveJob(item);
  m_realtime_panel->SetCurrentJob(item);

  if (auto handler = m_job_manager->GetJobHandler(item); handler)
  {
    m_realtime_panel->SetJobLog(handler->GetJobLog());
  }

  m_workspace_panel->SetProcedure(item ? item->GetExpandedProcedure() : nullptr);
}

OperationActionContext OperationMonitorView::CreateOperationContext()
{
  OperationActionContext result;
  result.selected_job = [this] { return m_job_panel->GetSelectedJob(); };
  result.send_message = [](const auto &event) { sup::gui::SendWarningMessage(event); };
  result.get_remote_connection_info = [this]()
  { return GetDialogRemoteConnectionInfo(*m_connection_service, this); };
  return result;
}

QWidget *OperationMonitorView::CreateLeftPanel()
{
  auto result = new sup::gui::ItemStackWidget;
  auto actions = m_presentation_mode == OperationPresentationMode::kIdeMode
                     ? m_job_panel->GetSequencerMonitorViewActions()
                     : m_job_panel->GetOperationMonitorViewActions();
  result->AddWidget(m_job_panel, actions);
  return result;
}

//! Create central panel with single OperationRealTimePanel.

QWidget *OperationMonitorView::CreateCentralPanel()
{
  auto result = new sup::gui::ItemStackWidget;
  result->AddWidget(m_realtime_panel, m_realtime_panel->actions());
  return result;
}

}  // namespace sequencergui
