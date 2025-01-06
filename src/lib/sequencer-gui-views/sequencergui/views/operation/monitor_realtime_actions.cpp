/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "monitor_realtime_actions.h"

#include <sequencergui/model/item_constants.h>
#include <sequencergui/style/style_helper.h>
#include <sup/gui/widgets/action_menu.h>

#include <QAction>
#include <QMenu>
#include <QSettings>
#include <QToolButton>
#include <QWidgetAction>

namespace
{

const QString kGroupName("MonitorRealTimeToolBar");
const QString kTickTimeOutSettingName = kGroupName + "/" + "tick_timeout";

QString GetDelayText(int delay)
{
  QString name = delay < 1000 ? QString("%1 msec").arg(delay) : QString("%1 sec").arg(delay / 1000);
  return name;
}

}  // namespace

namespace sequencergui
{

MonitorRealTimeActions::MonitorRealTimeActions(QObject *parent)
    : QObject(parent)
    , m_run_action(new QAction("Run", this))
    , m_pause_action(new QAction("Pause", this))
    , m_step_action(new QAction("Step", this))
    , m_stop_action(new QAction("Stop", this))
    , m_reset_action(new QAction("Reset", this))
    , m_delay_button(new QToolButton)
    , m_delay_action(new QWidgetAction(this))
    , m_settings_action(new sup::gui::ActionMenu)
    , m_delay_menu(CreateDelayMenu())
    , m_settings_menu(CreateSettingsMenu())
{
  ReadSettings();

  m_run_action->setIcon(FindIcon("arrow-right-drop-circle-outline"));
  m_run_action->setToolTip("Run procedure");
  connect(m_run_action, &QAction::triggered, this, &MonitorRealTimeActions::RunRequest);
  m_action_map.Add(ActionKey::kRun, m_run_action);

  m_pause_action->setIcon(FindIcon("pause-circle-outline"));
  m_pause_action->setToolTip("Pause sequence at the next occasion");
  connect(m_pause_action, &QAction::triggered, this, &MonitorRealTimeActions::PauseRequest);
  m_action_map.Add(ActionKey::kPause, m_pause_action);

  m_step_action->setIcon(FindIcon("play-pause"));
  m_step_action->setToolTip("Execute single instruction");
  connect(m_step_action, &QAction::triggered, this, &MonitorRealTimeActions::StepRequest);
  m_action_map.Add(ActionKey::kStep, m_step_action);

  m_stop_action->setIcon(FindIcon("stop-circle-outline"));
  m_stop_action->setToolTip("Stop procedure");
  connect(m_stop_action, &QAction::triggered, this, &MonitorRealTimeActions::StopRequest);
  m_action_map.Add(ActionKey::kStop, m_stop_action);

  m_reset_action->setIcon(FindIcon("page-first"));
  m_reset_action->setToolTip(
      "Reset sequencer procedure runner to the initial state\n"
      "Works only on finished/halted procedures");
  connect(m_reset_action, &QAction::triggered, this, &MonitorRealTimeActions::ResetRequest);
  m_action_map.Add(ActionKey::kReset, m_reset_action);

  m_delay_button->setText(GetDelayText(GetCurrentTickTimeout()));
  m_delay_button->setIcon(FindIcon("speedometer-slow"));
  m_delay_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_delay_button->setToolTip("Artificial delay after each change of instruction status");
  m_delay_button->setMenu(m_delay_menu.get());
  m_delay_button->setPopupMode(QToolButton::InstantPopup);
  m_delay_action->setDefaultWidget(m_delay_button);
  m_action_map.Add(ActionKey::kDelay, m_delay_action);

  m_settings_action->setText("Other");
  m_settings_action->setIcon(FindIcon("menu"));
  m_settings_action->setToolTip("Other settings");
  m_settings_action->setMenu(m_settings_menu.get());
  m_action_map.Add(ActionKey::kSettings, m_settings_action);
}

MonitorRealTimeActions::~MonitorRealTimeActions()
{
  WriteSettings();
}

QList<QAction *> MonitorRealTimeActions::GetActions(const std::vector<ActionKey> &action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

int MonitorRealTimeActions::GetCurrentTickTimeout()
{
  return m_current_tick_timeout;
}

void MonitorRealTimeActions::SetCurrentTickTimeout(int msec)
{
  m_current_tick_timeout = msec;
  m_delay_button->setText(GetDelayText(msec));
}

void MonitorRealTimeActions::ReadSettings()
{
  QSettings settings;
  m_current_tick_timeout =
      settings.value(kTickTimeOutSettingName, itemconstants::kDefaultTickTimeoutMsec).toInt();
}

void MonitorRealTimeActions::WriteSettings()
{
  QSettings settings;
  settings.setValue(kTickTimeOutSettingName, m_current_tick_timeout);
}

std::unique_ptr<QMenu> MonitorRealTimeActions::CreateDelayMenu()
{
  const std::vector<int> delay_values = {10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};

  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);
  for (const auto &delay : delay_values)
  {
    auto name = GetDelayText(delay);
    auto action = result->addAction(name);
    auto on_action = [this, delay, name]()
    {
      m_delay_button->setText(name);
      m_current_tick_timeout = delay;
      emit ChangeDelayRequest(delay);
    };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

std::unique_ptr<QMenu> MonitorRealTimeActions::CreateSettingsMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto scroll_action = result->addAction("Automatically scroll to current instruction");
  scroll_action->setToolTip(
      "Automatically scrolls tree view to display currently executed instruction");
  scroll_action->setCheckable(true);
  scroll_action->setChecked(true);
  connect(scroll_action, &QAction::toggled, this,
          &MonitorRealTimeActions::ScrollToSelectionRequest);

  return result;
}

}  // namespace sequencergui
