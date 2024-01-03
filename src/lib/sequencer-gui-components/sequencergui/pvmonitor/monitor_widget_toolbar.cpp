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

#include "monitor_widget_toolbar.h"

#include "workspace_editor_actions.h"

#include <sequencergui/domain/domain_utils.h>
#include <sup/gui/widgets/style_utils.h>

#include <QAction>
#include <QActionGroup>

namespace sequencergui
{

MonitorWidgetToolBar::MonitorWidgetToolBar(QWidget *parent)
    : QToolBar(parent)
    , m_workspace_editor_actions(new WorkspaceEditorActions(this))
    , m_start_action(new QAction)
    , m_stop_action(new QAction)
{
  setIconSize(sup::gui::utils::ToolBarIconSize());
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  connect(m_workspace_editor_actions, &WorkspaceEditorActions::AddVariableRequest, this,
          &MonitorWidgetToolBar::AddVariableRequest);
  connect(m_workspace_editor_actions, &WorkspaceEditorActions::EditAnyvalueRequest, this,
          &MonitorWidgetToolBar::EditAnyvalueRequest);
  connect(m_workspace_editor_actions, &WorkspaceEditorActions::RemoveVariableRequest, this,
          &MonitorWidgetToolBar::RemoveVariableRequest);

  addActions(m_workspace_editor_actions->GetActions());

  addSeparator();

  m_start_action->setText("Start");
  m_start_action->setIcon(sup::gui::utils::GetIcon("chevron-right-circle-outline.svg"));
  m_start_action->setToolTip("Start monitoring workspace variables");

  connect(m_start_action, &QAction::triggered, this, &MonitorWidgetToolBar::StartMonitoringRequest);
  addAction(m_start_action);

  m_stop_action->setText("Stop");
  m_stop_action->setIcon(sup::gui::utils::GetIcon("stop-circle-outline.svg"));
  m_stop_action->setToolTip("Stop monitoring");
  m_stop_action->setEnabled(false);
  connect(m_stop_action, &QAction::triggered, this, &MonitorWidgetToolBar::StopMonitoringRequest);
  addAction(m_stop_action);

  UpdateActionsState(false);
}

MonitorWidgetToolBar::~MonitorWidgetToolBar() = default;

void MonitorWidgetToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

void MonitorWidgetToolBar::UpdateActionsState(bool is_running)
{
  // when in running state, stop action should be enabled, others disabled, and vice versa
  auto enabled_when_is_running = QList<QAction *>() << m_stop_action;

  auto disabled_when_is_running = QList<QAction *>()
                                  << m_workspace_editor_actions->GetActions() << m_start_action;

  for (auto action : enabled_when_is_running)
  {
    action->setEnabled(is_running);
  }

  for (auto action : disabled_when_is_running)
  {
    action->setEnabled(!is_running);
  }
}

}  // namespace sequencergui
