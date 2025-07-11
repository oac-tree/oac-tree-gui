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

#include "operation_monitor_view.h"

#include "operation_job_panel.h"
#include "operation_realtime_panel.h"
#include "operation_workspace_panel.h"

#include <oac_tree_gui/jobsystem/automation_client.h>
#include <oac_tree_gui/jobsystem/job_manager.h>
#include <oac_tree_gui/jobsystem/local_job_handler.h>
#include <oac_tree_gui/jobsystem/remote_connection_info.h>
#include <oac_tree_gui/jobsystem/remote_connection_service.h>
#include <oac_tree_gui/mainwindow/main_window_helper.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/operation/operation_action_handler.h>
#include <oac_tree_gui/operation/operation_action_helper.h>
#include <oac_tree_gui/style/style_helper.h>
#include <oac_tree_gui/views/editors/user_input_dialogs.h>
#include <oac_tree_gui/views/operation/remote_connection_dialog.h>

#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/widgets/custom_splitter.h>
#include <sup/gui/widgets/item_stack_widget.h>
#include <sup/gui/widgets/message_helper.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>

#include <QToolBar>
#include <QVBoxLayout>

namespace oac_tree_gui
{

namespace
{

const QString kGroupName("OperationMonitorView");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";
const QString kWorkdirSettingName = kGroupName + "/" + "workdir";

/**
 * @brief Creates factory function to create clients to talk with remote server.
 */
std::function<std::unique_ptr<IAutomationClient>(const std::string &)> GetClientFactoryFunc()
{
  return [](const std::string &server_name)
  { return std::make_unique<AutomationClient>(server_name); };
}

/**
 * @brief Creates remote connection service to talk with sequencer remote server.
 */
std::unique_ptr<RemoteConnectionService> CreateRemoteConnectionService()
{
  return std::make_unique<RemoteConnectionService>(GetClientFactoryFunc());
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
    , m_connection_service(CreateRemoteConnectionService())
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

  m_workspace_panel->setVisible(false);

  ReadSettings();

  RegisterActionsForContext(sup::gui::AppRegisterWidgetUniqueId(this));
}

OperationMonitorView::~OperationMonitorView()
{
  WriteSettings();
  sup::gui::AppUnregisterWidgetUniqueId(this);
}

void OperationMonitorView::SetModels(ApplicationModels *models)
{
  m_models = models;
  m_job_panel->SetModels(models);
  m_action_handler->SetJobContainer(models->GetJobModel()->GetRootItem());
}

bool OperationMonitorView::OnImportJobRequest(const QString &file_name)
{
  auto path = file_name.isEmpty() ? GetOpenSequencerProcedureName(kWorkdirSettingName) : file_name;

  if (!path.isEmpty())
  {
    return m_action_handler->SubmitFileBasedJob(path.toStdString());
  }

  return false;
}

bool OperationMonitorView::HasRunningJobs() const
{
  return m_job_manager->HasRunningJobs();
}

void OperationMonitorView::StopAllJobs()
{
  m_job_manager->StopAllJobs();
}

void OperationMonitorView::RegisterActionsForContext(const sup::gui::AppContext &context)
{
  sup::gui::AppAddActionToCommand(m_toggle_left_sidebar,
                                  sup::gui::constants::kToggleLeftPanelCommandId, context);
  sup::gui::AppAddActionToCommand(m_toggle_right_sidebar,
                                  sup::gui::constants::kToggleRightPanelCommandId, context);
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
  connect(m_job_manager, &JobManager::ActiveInstructionChanged, m_realtime_panel,
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
  m_toggle_left_sidebar = new QAction("Show/hide left panel", this);
  m_toggle_left_sidebar->setToolTip("Show/hide left panel");
  m_toggle_left_sidebar->setIcon(FindIcon("dock-left"));
  connect(m_toggle_left_sidebar, &QAction::triggered, this,
          [this](auto) { m_left_panel->setVisible(!m_left_panel->isVisible()); });

  m_toggle_right_sidebar = new QAction("Show/hide right panel", this);
  m_toggle_right_sidebar->setToolTip("Show/hide right panel");
  m_toggle_right_sidebar->setIcon(FindIcon("dock-right"));
  connect(m_toggle_right_sidebar, &QAction::triggered, this,
          [this](auto) { m_workspace_panel->setVisible(!m_workspace_panel->isVisible()); });
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

}  // namespace oac_tree_gui
