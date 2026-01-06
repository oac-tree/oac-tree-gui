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

#include "monitor_widget_actions.h"

#include <oac_tree_gui/style/style_helper.h>

#include <QAction>

namespace oac_tree_gui
{

MonitorWidgetActions::MonitorWidgetActions(QObject* parent_object)
    : QObject(parent_object), m_start_action(new QAction(this)), m_stop_action(new QAction(this))
{
  m_start_action->setText("Start");
  m_start_action->setIcon(FindIcon("chevron-right-circle-outline"));
  m_start_action->setToolTip("Start monitoring workspace variables");
  m_action_map.Add(ActionKey::kStartWorkspace, m_start_action);
  connect(m_start_action, &QAction::triggered, this, &MonitorWidgetActions::StartMonitoringRequest);

  m_stop_action->setText("Stop");
  m_stop_action->setIcon(FindIcon("stop-circle-outline"));
  m_stop_action->setToolTip("Stop monitoring");
  m_stop_action->setEnabled(false);
  m_action_map.Add(ActionKey::kStopWorkspace, m_stop_action);
  connect(m_stop_action, &QAction::triggered, this, &MonitorWidgetActions::StopMonitoringRequest);
}

QList<QAction*> MonitorWidgetActions::GetActions(const std::vector<ActionKey>& action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

void MonitorWidgetActions::SetIsRunning(bool is_running)
{
  m_start_action->setEnabled(!is_running);
  m_stop_action->setEnabled(is_running);
}

}  // namespace oac_tree_gui
