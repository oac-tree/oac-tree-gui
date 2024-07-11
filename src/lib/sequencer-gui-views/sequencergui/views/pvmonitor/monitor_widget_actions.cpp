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

#include "monitor_widget_actions.h"

#include <sup/gui/widgets/style_utils.h>

#include <QAction>

namespace sequencergui
{

MonitorWidgetActions::MonitorWidgetActions(QObject *parent)
    : QObject(parent), m_start_action(new QAction(this)), m_stop_action(new QAction(this))
{
  m_start_action->setText("Start");
  m_start_action->setIcon(sup::gui::utils::GetIcon("chevron-right-circle-outline.svg"));
  m_start_action->setToolTip("Start monitoring workspace variables");
  m_action_map.Add(ActionKey::kStartWorkspace, m_start_action);
  connect(m_start_action, &QAction::triggered, this, &MonitorWidgetActions::StartMonitoringRequest);

  m_stop_action->setText("Stop");
  m_stop_action->setIcon(sup::gui::utils::GetIcon("stop-circle-outline.svg"));
  m_stop_action->setToolTip("Stop monitoring");
  m_stop_action->setEnabled(false);
  m_action_map.Add(ActionKey::kStopWorkspace, m_stop_action);
  connect(m_stop_action, &QAction::triggered, this, &MonitorWidgetActions::StopMonitoringRequest);
}

QList<QAction *> MonitorWidgetActions::GetActions(const std::vector<ActionKey> &action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

void MonitorWidgetActions::SetIsRunning(bool is_running)
{
  m_start_action->setEnabled(!is_running);
  m_stop_action->setEnabled(is_running);
}

}  // namespace sequencergui
