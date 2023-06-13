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

#include "operation_job_panel.h"
#include "operation_realtime_panel.h"
#include "operation_workspace_panel.h"
#include "procedure_action_handler.h"

#include <sequencergui/components/message_handler_factory.h>
#include <sequencergui/jobsystem/job_manager.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/monitor/monitor_property_widget.h>
#include <sequencergui/monitor/monitor_realtime_widget.h>
#include <sequencergui/monitor/sequencer_monitor_actions.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/widgets/widget_utils.h>

#include <QMainWindow>
#include <QSettings>
#include <QSplitter>
#include <QVBoxLayout>

namespace
{
const QString kGroupName("InstructionEditorWidget");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";
}  // namespace

namespace sequencergui
{

OperationMonitorView::OperationMonitorView(QMainWindow *parent)
    : QWidget(parent)
    , m_splitter(new QSplitter)
    , m_job_panel(new OperationJobPanel)
    , m_realtime_panel(new OperationRealTimePanel)
    , m_workspace_panel(new OperationWorkspacePanel)
    , m_job_manager(new JobManager(this))
    , m_actions(new SequencerMonitorActions(
          m_job_manager, [this] { return m_job_panel->GetSelectedJob(); }, this))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 0, 4, 4);
  layout->setSpacing(0);

  m_splitter->addWidget(m_job_panel);
  m_splitter->addWidget(m_realtime_panel);
  m_splitter->addWidget(m_workspace_panel);
  m_splitter->setSizes(QList<int>() << mvvm::utils::UnitSize(30) << mvvm::utils::UnitSize(90)
                                    << mvvm::utils::UnitSize(30));

  parent->addToolBar(m_job_panel->GetToolBar());
  parent->addToolBar(m_realtime_panel->GetToolBar());

  layout->addWidget(m_splitter);

  SetupConnections();

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

void OperationMonitorView::ReadSettings()
{
  const QSettings settings;

  if (settings.contains(kSplitterSettingName))
  {
    m_splitter->restoreState(settings.value(kSplitterSettingName).toByteArray());
  }
}

void OperationMonitorView::WriteSettings()
{
  QSettings settings;
  settings.setValue(kSplitterSettingName, m_splitter->saveState());
}

void OperationMonitorView::SetupConnections()
{
  // Process request from MonitorRealTimeWidget to SequencerMonitorActions

  // start request
  connect(m_realtime_panel, &OperationRealTimePanel::runRequest, m_actions,
          &SequencerMonitorActions::OnStartJobRequest);

  // pause request
  connect(m_realtime_panel, &OperationRealTimePanel::pauseRequest, m_actions,
          &SequencerMonitorActions::OnPauseJobRequest);

  // stop request
  connect(m_realtime_panel, &OperationRealTimePanel::stopRequest, m_actions,
          &SequencerMonitorActions::OnStopJobRequest);

  // step request
  connect(m_realtime_panel, &OperationRealTimePanel::stepRequest, m_actions,
          &SequencerMonitorActions::OnMakeStepRequest);

  // change delay request from MonitorRealTimeWidget to JobManager
  connect(m_realtime_panel, &OperationRealTimePanel::changeDelayRequest, m_job_manager,
          &JobManager::OnChangeDelayRequest);

  // instruction statuc changed request from JobManager to MonitorRealTimeWidget
  connect(m_job_manager, &JobManager::InstructionStatusChanged, m_realtime_panel,
          &OperationRealTimePanel::SetSelectedInstruction);

  // job selection request from OperationJobPanel
  connect(m_job_panel, &OperationJobPanel::JobSelected, this, &OperationMonitorView::OnJobSelected);

  // import request
  connect(m_job_panel, &OperationJobPanel::ImportJobRequest, this,
          &OperationMonitorView::OnImportJobRequest);

  // job removal request
  connect(m_job_panel, &OperationJobPanel::RemoveJobRequest, m_actions,
          &SequencerMonitorActions::OnRemoveJobRequest);

  // job regenerate request
  connect(m_job_panel, &OperationJobPanel::RegenerateJobRequest, m_actions,
          &SequencerMonitorActions::OnRegenerateJobRequest);

  // job selection request from SequencerMonitorActions
  connect(m_actions, &SequencerMonitorActions::MakeJobSelectedRequest, m_job_panel,
          &OperationJobPanel::SetSelectedJob);
}

void OperationMonitorView::OnJobSelected(JobItem *item)
{
  m_job_manager->SetCurrentJob(item);
  m_realtime_panel->SetProcedure(item ? item->GetExpandedProcedure() : nullptr);
  m_workspace_panel->SetProcedure(item ? item->GetExpandedProcedure() : nullptr);
}

void OperationMonitorView::OnImportJobRequest()
{
  auto model = m_models->GetSequencerModel();

  ProcedureActionHandler handler;
  if (auto procedure = handler.LoadProcedureFromFileRequest(); procedure)
  {
    auto procedure_ptr = procedure.get();
    model->InsertItem(std::move(procedure), model->GetProcedureContainer(),
                      mvvm::TagIndex::Append());
    m_actions->OnSubmitJobRequest(procedure_ptr);
  }
}

}  // namespace sequencergui
