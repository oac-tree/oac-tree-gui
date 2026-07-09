/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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
#include "operation_splittable_widget.h"

#include <oac_tree_gui/jobsystem/automation_client.h>
#include <oac_tree_gui/jobsystem/i_job_handler.h>
#include <oac_tree_gui/jobsystem/objects/job_manager.h>
#include <oac_tree_gui/jobsystem/remote_connection_service.h>
#include <oac_tree_gui/mainwindow/main_window_helper.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/remote_connection_info.h>
#include <oac_tree_gui/operation/objects/operation_action_handler.h>
#include <oac_tree_gui/operation/operation_action_helper.h>
#include <oac_tree_gui/style/style_helper.h>
#include <oac_tree_gui/views/editors/user_input_dialogs.h>
#include <oac_tree_gui/views/operation/remote_connection_dialog.h>

#include <sup/gui/app/app_command_context.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/app/i_app_command_service.h>
#include <sup/gui/widgets/custom_splitter.h>
#include <sup/gui/widgets/item_stack_widget.h>
#include <sup/gui/widgets/message_helper.h>

#include <mvvm/model/item_selection.h>
#include <mvvm/model/model_utils.h>

#include <QAction>
#include <QIcon>
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
std::function<std::unique_ptr<IAutomationClient>(const AutomationServerInfo&)>
GetClientFactoryFunc()
{
  return [](const AutomationServerInfo& server_info)
  { return std::make_unique<AutomationClient>(server_info); };
}

/**
 * @brief Creates remote connection service to talk with sequencer remote server.
 */
std::unique_ptr<RemoteConnectionService> CreateRemoteConnectionService()
{
  auto on_error = [](const sup::gui::MessageEvent& event) { SendWarningMessage(event); };
  return std::make_unique<RemoteConnectionService>(GetClientFactoryFunc(), on_error);
}

}  // namespace

OperationMonitorView::OperationMonitorView(sup::gui::IAppCommandService& command_service,
                                           OperationPresentationMode mode, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_command_service(command_service)
    , m_presentation_mode(mode)
    , m_job_panel(new OperationJobPanel)
    , m_splittable_widget(new OperationSplittableWidget)
    , m_left_panel(CreateLeftPanel())
    , m_splitter(new sup::gui::CustomSplitter(kSplitterSettingName))
    , m_connection_service(CreateRemoteConnectionService())
    , m_job_manager(new JobManager(
          GetJobHandlerFactoryFunc(CreateDefaultUserContext(this), *m_connection_service), this))
    , m_action_handler(new OperationActionHandler(m_job_manager, CreateOperationContext(), this))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);

  m_splitter->addWidget(m_left_panel);
  m_splitter->addWidget(m_splittable_widget);
  m_splitter->setSizes({300, 900});

  layout->addWidget(m_splitter);

  SetupConnections();
  SetupWidgetActions();

  ReadSettings();

  auto context = m_command_service.RegisterWidgetUniqueId(this);
  RegisterActionsForContext(context);
}

OperationMonitorView::~OperationMonitorView()
{
  WriteSettings();
  m_command_service.UnregisterWidgetUniqueId(this);
}

void OperationMonitorView::SetModels(ApplicationModels* models)
{
  m_models = models;
  m_job_panel->SetModels(models);
  m_splittable_widget->SetModel(models->GetJobModel());
  m_action_handler->SetJobContainer(models->GetJobModel()->GetRootItem());
}

bool OperationMonitorView::OnImportJobRequest(const QString& file_name)
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

void OperationMonitorView::StopJobs(StopScope scope)
{
  m_job_manager->StopJobs(scope);
}

void OperationMonitorView::RegisterActionsForContext(const sup::gui::AppCommandContext& context)
{
  m_command_service.AddActionToCommand(m_toggle_left_sidebar,
                                       sup::gui::constants::kToggleLeftPanelCommandId, context);
}

OperationActionHandler* OperationMonitorView::GetOperationActionHandler()
{
  return m_action_handler;
}

void OperationMonitorView::showEvent(QShowEvent* event)
{
  Q_UNUSED(event);
  if (m_job_panel->GetSelectedJob() == nullptr)
  {
    if (auto job = ::mvvm::utils::GetTopItem<JobItem>(*m_models->GetJobModel()); job)
    {
      m_job_panel->SetSelectedJob(job);
    }
  }
}

void OperationMonitorView::closeEvent(QCloseEvent* event)
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

  // Execution requests carry the JobItem of the emitting panel, so they connect straight to the
  // handler.

  // start request
  connect(m_splittable_widget, &OperationSplittableWidget::RunRequest, m_action_handler,
          &OperationActionHandler::OnStartJobRequest);

  // pause request
  connect(m_splittable_widget, &OperationSplittableWidget::PauseRequest, m_action_handler,
          &OperationActionHandler::OnPauseJobRequest);

  // step request
  connect(m_splittable_widget, &OperationSplittableWidget::StepRequest, m_action_handler,
          &OperationActionHandler::OnMakeStepRequest);

  // stop request
  connect(m_splittable_widget, &OperationSplittableWidget::StopRequest, m_action_handler,
          &OperationActionHandler::OnStopJobRequest);

  // reset request
  connect(m_splittable_widget, &OperationSplittableWidget::ResetRequest, m_action_handler,
          &OperationActionHandler::OnResetJobRequest);

  // change delay request: update the job's tick timeout and reflect it in every panel showing it
  connect(m_splittable_widget, &OperationSplittableWidget::ChangeDelayRequest, m_action_handler,
          [this](JobItem* job, int msec)
          {
            m_action_handler->OnSetTickTimeoutRequest(job, std::chrono::milliseconds{msec});
            m_splittable_widget->SetTickTimeout(job, msec);
          });
  // seed the default tick timeout applied to newly submitted jobs
  m_action_handler->OnSetTickTimeoutRequest(
      nullptr, std::chrono::milliseconds{m_splittable_widget->GetCurrentTickTimeout()});

  // active instructions of a job are routed to every panel showing that job
  connect(m_job_manager, &JobManager::ActiveInstructionChanged, m_splittable_widget,
          [this](JobItem* job)
          {
            m_splittable_widget->SetSelectedInstructions(job,
                                                         m_job_manager->GetActiveInstructions(job));
          });

  // keep the manager's active-jobs set in sync with the jobs shown across the panels, and seed each
  // freshly shown panel with the current active instructions and log of its job
  connect(m_splittable_widget, &OperationSplittableWidget::activeJobsChanged, this,
          [this](const std::vector<JobItem*>& jobs)
          {
            m_job_manager->SetActiveJobs(jobs);
            for (auto* job : jobs)
            {
              m_splittable_widget->SetSelectedInstructions(
                  job, m_job_manager->GetActiveInstructions(job));
              if (auto handler = m_job_manager->GetJobHandler(job); handler != nullptr)
              {
                m_splittable_widget->SetJobLog(job, handler->GetJobLog());
              }
            }
          });

  // job selection request from MonitorPanel
  connect(m_job_panel, &OperationJobPanel::JobSelected, this, &OperationMonitorView::OnJobSelected);

  // job submission request
  connect(m_job_panel, &OperationJobPanel::SubmitProcedureRequest, m_action_handler,
          [this](const auto* item)
          { m_action_handler->SubmitLocalJob(const_cast<ProcedureItem*>(item)); });

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

  connect(
      m_splittable_widget, &OperationSplittableWidget::ToggleBreakpointRequest, m_action_handler,
      [this](JobItem* job, const auto* instruction)
      { m_action_handler->OnToggleBreakpoint(job, const_cast<InstructionItem*>(instruction)); });

  // propagate combo selection from current active panel to list with jobs
  auto on_selected_job = [this](const mvvm::ItemSelection& selection)
  { m_job_panel->SetSelectedJob(selection.GetItem<JobItem>()); };
  connect(m_splittable_widget, &OperationSplittableWidget::focusWidgetJobSelectionChanged, this,
          on_selected_job);
}

void OperationMonitorView::SetupWidgetActions()
{
  m_toggle_left_sidebar = new QAction("Show/hide left panel", this);
  m_toggle_left_sidebar->setToolTip("Show/hide left panel");
  m_toggle_left_sidebar->setIcon(FindIcon("dock-left"));
  connect(m_toggle_left_sidebar, &QAction::triggered, this,
          [this](auto) { m_left_panel->setVisible(!m_left_panel->isVisible()); });
}

//! Setup widgets to show currently selected job.
void OperationMonitorView::OnJobSelected(const JobItem* selected_item)
{
  auto job_item = const_cast<JobItem*>(selected_item);

  // showing the job in the focused panel triggers activeJobsChanged, which seeds that panel with
  // the job's active instructions and log
  m_splittable_widget->SetCurrentJob(job_item);
}

OperationActionContext OperationMonitorView::CreateOperationContext()
{
  OperationActionContext result;
  result.selected_job = [this] { return m_job_panel->GetSelectedJob(); };
  result.send_message = [](const auto& event) { sup::gui::SendWarningMessage(event); };
  result.get_remote_connection_info = [this]()
  { return GetDialogRemoteConnectionInfo(*m_connection_service, this); };
  result.drop_remote_connection = [this](const AutomationServerInfo& server_info)
  { m_connection_service->Disconnect(server_info); };
  result.confirm_job_removal = [](const std::string& question)
  { return ShouldRemoveRunningJob(question); };
  return result;
}

QWidget* OperationMonitorView::CreateLeftPanel()
{
  auto result = new sup::gui::ItemStackWidget;
  result->SetPanelStyle(sup::gui::ItemStackWidget::PanelStyle::kButtonTextUnderIcon);
  auto actions = m_presentation_mode == OperationPresentationMode::kIdeMode
                     ? m_job_panel->GetSequencerMonitorViewActions()
                     : m_job_panel->GetOperationMonitorViewActions();
  result->AddWidget(m_job_panel, actions);
  return result;
}

}  // namespace oac_tree_gui
