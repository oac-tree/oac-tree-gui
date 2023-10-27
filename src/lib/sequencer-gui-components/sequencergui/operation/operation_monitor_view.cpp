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

#include "operation_monitor_view.h"

#include "operation_action_handler.h"
#include "operation_job_panel.h"
#include "operation_realtime_panel.h"
#include "operation_workspace_panel.h"
#include "procedure_action_handler.h"

#include <sequencergui/components/message_handler_factory.h>
#include <sequencergui/jobsystem/job_handler.h>
#include <sequencergui/jobsystem/job_manager.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/widgets/item_stack_widget.h>
#include <sequencergui/widgets/style_utils.h>
#include <sup/gui/app/app_action_helper.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/widgets/all_items_tree_view.h>
#include <mvvm/widgets/top_items_tree_view.h>
#include <mvvm/widgets/widget_utils.h>

#include <QSettings>
#include <QSplitter>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidgetAction>

namespace
{
const QString kGroupName("OperationMonitorView");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";
const QString kLeftPanelIsVisibleSettingName = kGroupName + "/" + "left_panel";
const QString kRightPanelIsVisibleSettingName = kGroupName + "/" + "right_panel";
}  // namespace

namespace sequencergui
{

OperationMonitorView::OperationMonitorView(Mode mode, QWidget *parent)
    : QWidget(parent)
    , m_job_panel(new OperationJobPanel)
    , m_realtime_panel(new OperationRealTimePanel)
    , m_workspace_tree_panel(
          new OperationWorkspacePanel(OperationWorkspacePanel::Mode::kWorkspaceTree))
    , m_workspace_table_panel(
          new OperationWorkspacePanel(OperationWorkspacePanel::Mode::kWorkspaceTable))
    , m_left_panel(CreateLeftPanel(mode))
    , m_right_panel(CreateRightPanel())
    , m_splitter(new QSplitter)
    , m_job_manager(new JobManager(this))
    , m_actions(new OperationActionHandler(
          m_job_manager, [this] { return m_job_panel->GetSelectedJob(); }, this))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);

  m_splitter->addWidget(m_left_panel);
  m_splitter->addWidget(CreateCentralPanel());
  m_splitter->addWidget(m_right_panel);
  m_splitter->setSizes(QList<int>() << mvvm::utils::UnitSize(30) << mvvm::utils::UnitSize(90)
                                    << mvvm::utils::UnitSize(30));

  layout->addWidget(m_splitter);

  SetupConnections();
  SetupWidgetActions();

  m_actions->SetMessageHandler(CreateMessageBoxHandler());
  m_job_manager->SetMessagePanel(m_realtime_panel->GetMessagePanel());
  ReadSettings();
}

OperationMonitorView::~OperationMonitorView()
{
  WriteSettings();
}

void OperationMonitorView::SetApplicationModels(ApplicationModels *models)
{
  m_models = models;
  m_job_panel->SetApplicationModels(models);
  m_actions->SetJobModel(models->GetJobModel());
}

bool OperationMonitorView::OnImportJobRequest(const QString &file_name)
{
  auto model = m_models->GetSequencerModel();

  ProcedureActionHandler handler;
  auto procedure = handler.LoadProcedureFromFile(file_name);

  if (procedure)
  {
    auto procedure_ptr = procedure.get();
    model->InsertItem(std::move(procedure), model->GetProcedureContainer(),
                      mvvm::TagIndex::Append());
    return m_actions->OnSubmitJobRequest(procedure_ptr);
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
  WriteSettings();
}

void OperationMonitorView::ReadSettings()
{
  const QSettings settings;

  if (settings.contains(kSplitterSettingName))
  {
    m_splitter->restoreState(settings.value(kSplitterSettingName).toByteArray());
  }

  // see comments to the method SetupWidgetActions)_
  m_left_panel_is_visible = settings.value(kLeftPanelIsVisibleSettingName, true).toBool();
  m_right_panel_is_visible = settings.value(kRightPanelIsVisibleSettingName, true).toBool();
  m_left_panel->setVisible(m_left_panel_is_visible);
  m_right_panel->setVisible(m_right_panel_is_visible);
}

void OperationMonitorView::WriteSettings()
{
  QSettings settings;
  settings.setValue(kSplitterSettingName, m_splitter->saveState());
  settings.setValue(kLeftPanelIsVisibleSettingName, m_left_panel_is_visible);
  settings.setValue(kRightPanelIsVisibleSettingName, m_right_panel_is_visible);
}

void OperationMonitorView::SetupConnections()
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
  m_job_manager->OnChangeDelayRequest(m_realtime_panel->GetCurrentTickTimeout());

  // instruction next leave request from JobManager to MonitorRealTimeWidget
  connect(m_job_manager, &JobManager::NextLeavesChanged, m_realtime_panel,
          &OperationRealTimePanel::SetSelectedInstructions);

  // job selection request from MonitorPanel
  connect(m_job_panel, &OperationJobPanel::JobSelected, this, &OperationMonitorView::OnJobSelected);

  // job submission request
  connect(m_job_panel, &OperationJobPanel::SubmitProcedureRequest, m_actions,
          &OperationActionHandler::OnSubmitJobRequest);

  // import request
  connect(m_job_panel, &OperationJobPanel::ImportJobRequest, this,
          [this]() { OnImportJobRequest(); });

  // job removal request
  connect(m_job_panel, &OperationJobPanel::RemoveJobRequest, m_actions,
          &OperationActionHandler::OnRemoveJobRequest);

  // job removal request with cleanup after
  connect(m_job_panel, &OperationJobPanel::RemoveAndCleanupJobRequest, m_actions,
          &OperationActionHandler::OnRemoveJobAndCleanupRequest);

  // job regenerate request
  connect(m_job_panel, &OperationJobPanel::RegenerateJobRequest, m_actions,
          &OperationActionHandler::OnRegenerateJobRequest);

  // job selection request from SequencerMonitorActions
  connect(m_actions, &OperationActionHandler::MakeJobSelectedRequest, m_job_panel,
          &OperationJobPanel::SetSelectedJob);

  auto on_toggle_breakpoint_request = [this](auto *instruction)
  {
    if (auto handler = m_job_manager->GetCurrentJobHandler(); handler)
    {
      handler->OnToggleBreakpointRequest(instruction);
    }
  };
  connect(m_realtime_panel, &OperationRealTimePanel::ToggleBreakpointRequest, this,
          on_toggle_breakpoint_request);
}

void OperationMonitorView::SetupWidgetActions()
{
  // We setup actions to collapse/expand left and right panels. Here we are relying
  // on two additional flags for that: m_show_left_sidebar and m_show_right_sidebar.
  // We could do instead widget->setVisible(!widget->isVisible()), but this will not allow
  // us to save visible/invisible state on application destruction: the reason is that widgets
  // during shutdown phase are always invisible.

  m_show_left_sidebar = new QAction("Show/hide Left Sidebar", this);
  m_show_left_sidebar->setShortcut(QKeySequence(QString("Ctrl+0")));
  m_show_left_sidebar->setStatusTip("Show/hide Left Sidebar");
  m_show_left_sidebar->setIcon(styleutils::GetIcon("dock-left"));
  connect(m_show_left_sidebar, &QAction::triggered, this,
          [this](auto)
          {
            m_left_panel_is_visible = !m_left_panel_is_visible;
            m_left_panel->setVisible(m_left_panel_is_visible);
          });

  m_show_right_sidebar = new QAction("Show/hide Right Sidebar", this);
  m_show_right_sidebar->setShortcut(QKeySequence(QString("Ctrl+Shift+0")));
  m_show_right_sidebar->setStatusTip("Show/hide Right Sidebar");
  m_show_right_sidebar->setIcon(styleutils::GetIcon("dock-right"));
  connect(m_show_right_sidebar, &QAction::triggered, this,
          [this](auto)
          {
            m_right_panel_is_visible = !m_right_panel_is_visible;
            m_right_panel->setVisible(m_right_panel_is_visible);
          });

  sup::gui::AppRegisterAction(sup::gui::constants::kViewMenu, m_show_left_sidebar);
  sup::gui::AppRegisterAction(sup::gui::constants::kViewMenu, m_show_right_sidebar);
}

//! Setup widgets to show currently selected job.
void OperationMonitorView::OnJobSelected(JobItem *item)
{
  m_job_manager->SetCurrentJob(item);
  m_realtime_panel->SetProcedure(item ? item->GetExpandedProcedure() : nullptr);
  m_workspace_tree_panel->SetProcedure(item ? item->GetExpandedProcedure() : nullptr);
  m_workspace_table_panel->SetProcedure(item ? item->GetExpandedProcedure() : nullptr);
}

QWidget *OperationMonitorView::CreateLeftPanel(Mode mode)
{
  auto result = new ItemStackWidget;
  auto actions = mode == kIdeMode ? m_job_panel->GetSequencerMonitorViewActions()
                                  : m_job_panel->GetOperationMonitorViewActions();
  result->AddWidget(m_job_panel, actions);
  return result;
}

//! Create central panel with single OperationRealTimePanel.

QWidget *OperationMonitorView::CreateCentralPanel()
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

QWidget *OperationMonitorView::CreateRightPanel()
{
  auto result = new ItemStackWidget;
  result->AddWidget(m_workspace_tree_panel);
  result->AddWidget(m_workspace_table_panel);
  result->SetCurrentIndex(0);
  return result;
}

}  // namespace sequencergui
