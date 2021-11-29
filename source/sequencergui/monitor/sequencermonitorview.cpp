/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/monitor/sequencermonitorview.h"

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/monitor/jobmanager.h"
#include "sequencergui/monitor/monitorpanel.h"
#include "sequencergui/monitor/monitortreewidget.h"
#include "sequencergui/monitor/monitorworkspacewidget.h"

#include "mvvm/widgets/allitemstreeview.h"
#include "mvvm/widgets/topitemstreeview.h"

#include <QSplitter>
#include <QVBoxLayout>

namespace sequi
{

SequencerMonitorView::SequencerMonitorView(QWidget *parent)
    : QWidget(parent)
    , m_monitor_panel(new MonitorPanel)
    , m_tree_widget(new MonitorTreeWidget)
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

void SequencerMonitorView::SetModel(SequencerModel *model)
{
  m_model = model;
  m_job_manager->setModel(model);
  m_job_manager->setCurrentProcedure(GetFirstProcedure());
  m_monitor_panel->SetModel(model);
  m_monitor_panel->SetSelectedProcedure(GetFirstProcedure());
  m_tree_widget->SetModel(model, GetFirstProcedure());
  m_workspace_widget->SetModel(model, GetFirstProcedure());

}

void SequencerMonitorView::SetupConnections()
{
  // Process request from MonitorTreeWidget to JobManager
  auto on_start = [this]()
  { m_job_manager->onStartProcedureRequest(m_monitor_panel->GetSelectedProcedure()); };
  connect(m_tree_widget, &MonitorTreeWidget::runRequest, this, on_start);

  // Pause request from MonitorTreeWidget to JobManager
  connect(m_tree_widget, &MonitorTreeWidget::pauseRequest, m_job_manager,
          &JobManager::onPauseProcedureRequest);

  // Stop request from MonitorTreeWidget to JobManager
  connect(m_tree_widget, &MonitorTreeWidget::stopRequest, m_job_manager,
          &JobManager::onStopProcedureRequest);

  // Process step button click
  auto on_step = [this]() { m_job_manager->onMakeStepRequest(); };
  connect(m_tree_widget, &MonitorTreeWidget::stepRequest, this, on_step);

  // Selection request from JobManager to this
  auto on_selection = [this](auto instruction) { m_tree_widget->SetSelected(instruction); };
  connect(m_job_manager, &JobManager::InstructionStatusChanged, this, on_selection);

  auto on_procedure_selected = [this](auto procedure_item)
  {
    m_job_manager->setCurrentProcedure(procedure_item);
    m_tree_widget->SetModel(m_model, procedure_item);
    m_workspace_widget->SetModel(m_model, procedure_item);
  };
  connect(m_monitor_panel, &MonitorPanel::procedureSelected, this, on_procedure_selected);

  connect(m_tree_widget, &MonitorTreeWidget::changeDelayRequest, m_job_manager,
          &JobManager::onChangeDelayRequest);
}

ProcedureItem *SequencerMonitorView::GetFirstProcedure()
{
  auto procedure_container = m_model->GetProcedureContainer();
  return procedure_container->IsEmpty() ? nullptr : procedure_container->GetItem<ProcedureItem>("");
}

}  // namespace sequi
