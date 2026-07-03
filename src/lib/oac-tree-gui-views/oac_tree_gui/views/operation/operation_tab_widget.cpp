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

#include "operation_tab_widget.h"

#include "monitor_realtime_actions.h"
#include "operation_realtime_widget.h"

#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/style/style_helper.h>

#include <mvvm/style/mvvm_style_helper.h>

#include <QTabWidget>
#include <QToolBar>
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

OperationTabWidget::OperationTabWidget(QWidget* parent_widget)
    : sup::gui::SessionItemWidget(parent_widget)
    , m_actions(new MonitorRealTimeActions(this))
    , m_tool_bar(new QToolBar)
    , m_tab_widget(new QTabWidget)
    , m_realtime_widget(new OperationRealTimeWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_tool_bar->setIconSize(NarrowToolBarIconSize());
  m_tool_bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  // to have fixed height when no actions are added
  m_tool_bar->setFixedHeight(NarrowToolBarIconSize().height() + mvvm::style::UnitSize(1.9));
  m_tool_bar->addActions(GetToolBarActions(m_actions));

  layout->addWidget(m_tool_bar);
  layout->addWidget(m_tab_widget);

  AddTab(m_realtime_widget, "Real time", "Real time instruction tree with message panel");

  SetupConnections();
}

void OperationTabWidget::SetItem(mvvm::SessionItem* job_item)
{
  SetCurrentJob(dynamic_cast<JobItem*>(job_item));
}

void OperationTabWidget::SetCurrentJob(JobItem* job_item)
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

void OperationTabWidget::SetSelectedInstructions(const std::vector<const InstructionItem*>& items)
{
  m_realtime_widget->SetSelectedInstructions(items);
}

void OperationTabWidget::SetJobLog(JobLog* job_log)
{
  m_realtime_widget->SetJobLog(job_log);
}

void OperationTabWidget::SetupConnections()
{
  // forward signals from a toolbar further up
  connect(m_actions, &MonitorRealTimeActions::RunRequest, this, &OperationTabWidget::RunRequest);
  connect(m_actions, &MonitorRealTimeActions::PauseRequest, this,
          &OperationTabWidget::PauseRequest);
  connect(m_actions, &MonitorRealTimeActions::StepRequest, this, &OperationTabWidget::StepRequest);
  connect(m_actions, &MonitorRealTimeActions::StopRequest, this, &OperationTabWidget::StopRequest);
  connect(m_actions, &MonitorRealTimeActions::ResetRequest, this,
          &OperationTabWidget::ResetRequest);
  connect(m_actions, &MonitorRealTimeActions::ChangeDelayRequest, this,
          &OperationTabWidget::ChangeDelayRequest);

  connect(m_actions, &MonitorRealTimeActions::ScrollToSelectionRequest, m_realtime_widget,
          &OperationRealTimeWidget::SetViewportFollowsSelectionFlag);

  connect(m_realtime_widget, &OperationRealTimeWidget::ToggleBreakpointRequest, this,
          &OperationTabWidget::ToggleBreakpointRequest);
}

void OperationTabWidget::AddTab(QWidget* widget, const QString& label, const QString& tooltip)
{
  m_tab_widget->addTab(widget, label);
  m_tab_widget->setTabToolTip(m_tab_widget->indexOf(widget), tooltip);
}

}  // namespace oac_tree_gui
