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

#include "operation_realtime_panel.h"

#include "monitor_realtime_actions.h"
#include "operation_realtime_widget.h"

#include <oac_tree_gui/model/job_item.h>

#include <QVBoxLayout>

namespace
{

QList<QAction*> GetToolBarActions(oac_tree_gui::MonitorRealTimeActions* actions)
{
  using ActionKey = oac_tree_gui::MonitorRealTimeActions::ActionKey;
  return actions->GetActions({ActionKey::kRun, ActionKey::kPause, ActionKey::kStep,
                              ActionKey::kStop, ActionKey::kReset, ActionKey::kDelay,
                              ActionKey::kSettings});
}

}  // namespace

namespace oac_tree_gui
{

OperationRealTimePanel::OperationRealTimePanel(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_actions(new MonitorRealTimeActions(this))
    , m_realtime_widget(new OperationRealTimeWidget)
{
  setWindowTitle("Operations");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_realtime_widget);

  SetupConnections();

  addActions(GetToolBarActions(m_actions));
}

OperationRealTimePanel::~OperationRealTimePanel() = default;

void OperationRealTimePanel::SetCurrentJob(JobItem* job_item)
{
  if (job_item != nullptr)
  {
    m_realtime_widget->SetProcedure(job_item->GetExpandedProcedure());
    m_actions->SetCurrentTickTimeout(job_item->GetTickTimeout().count());
  }
  else
  {
    m_realtime_widget->SetProcedure(nullptr);
  }
}

void OperationRealTimePanel::SetSelectedInstructions(
    const std::vector<const InstructionItem*>& items)
{
  m_realtime_widget->SetSelectedInstructions(items);
}

void OperationRealTimePanel::SetJobLog(JobLog* job_log)
{
  m_realtime_widget->SetJobLog(job_log);
}

int OperationRealTimePanel::GetCurrentTickTimeout()
{
  return m_actions->GetCurrentTickTimeout();
}

void OperationRealTimePanel::SetupConnections()
{
  // forward signals from a toolbar further up
  connect(m_actions, &MonitorRealTimeActions::RunRequest, this,
          &OperationRealTimePanel::RunRequest);
  connect(m_actions, &MonitorRealTimeActions::PauseRequest, this,
          &OperationRealTimePanel::PauseRequest);
  connect(m_actions, &MonitorRealTimeActions::StepRequest, this,
          &OperationRealTimePanel::StepRequest);
  connect(m_actions, &MonitorRealTimeActions::StopRequest, this,
          &OperationRealTimePanel::StopRequest);
  connect(m_actions, &MonitorRealTimeActions::ResetRequest, this,
          &OperationRealTimePanel::ResetRequest);
  connect(m_actions, &MonitorRealTimeActions::ChangeDelayRequest, this,
          &OperationRealTimePanel::ChangeDelayRequest);

  connect(m_realtime_widget, &OperationRealTimeWidget::ToggleBreakpointRequest, this,
          &OperationRealTimePanel::ToggleBreakpointRequest);
  connect(m_actions, &MonitorRealTimeActions::ScrollToSelectionRequest, m_realtime_widget,
          &OperationRealTimeWidget::SetViewportFollowsSelectionFlag);
}

}  // namespace oac_tree_gui
