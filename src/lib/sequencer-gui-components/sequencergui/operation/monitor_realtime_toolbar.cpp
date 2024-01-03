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

#include "monitor_realtime_toolbar.h"

#include <sequencergui/jobsystem/job_utils.h>
#include <sup/gui/widgets/style_utils.h>

#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSettings>
#include <QToolButton>

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
MonitorRealTimeToolBar::MonitorRealTimeToolBar(QWidget *parent)
    : QToolBar(parent)
    , m_run_button(new QToolButton)
    , m_pause_button(new QToolButton)
    , m_step_button(new QToolButton)
    , m_stop_button(new QToolButton)
    , m_delay_button(new QToolButton)
    , m_settings_button(new QToolButton)
    , m_delay_menu(CreateDelayMenu())
    , m_settings_menu(CreateSettingsMenu())
{
  ReadSettings();

  setIconSize(sup::gui::utils::ToolBarIconSize());

  m_run_button->setText("Run");
  m_run_button->setIcon(sup::gui::utils::GetIcon("arrow-right-drop-circle-outline.svg"));
  m_run_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_run_button->setToolTip("Run procedure");
  connect(m_run_button, &QToolButton::clicked, this, &MonitorRealTimeToolBar::runRequest);
  addWidget(m_run_button);

  m_pause_button->setText("Pause");
  m_pause_button->setIcon(sup::gui::utils::GetIcon("pause-circle-outline.svg"));
  m_pause_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_pause_button->setToolTip("Pause sequence at the next occasion");
  connect(m_pause_button, &QToolButton::clicked, this, &MonitorRealTimeToolBar::pauseRequest);
  addWidget(m_pause_button);

  m_step_button->setText("Step");
  m_step_button->setIcon(sup::gui::utils::GetIcon("play-pause.svg"));
  m_step_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_step_button->setToolTip("Execute single instruction");
  connect(m_step_button, &QToolButton::clicked, this, &MonitorRealTimeToolBar::stepRequest);
  addWidget(m_step_button);

  m_stop_button->setText("Stop");
  m_stop_button->setIcon(sup::gui::utils::GetIcon("stop-circle-outline.svg"));
  m_stop_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_stop_button->setToolTip("Stop procedure");
  connect(m_stop_button, &QToolButton::clicked, this, &MonitorRealTimeToolBar::stopRequest);
  addWidget(m_stop_button);

  m_delay_button->setText(GetDelayText(GetCurrentTickTimeout()));
  m_delay_button->setIcon(sup::gui::utils::GetIcon("speedometer-slow.svg"));
  m_delay_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_delay_button->setToolTip("Artificial delay after each change of instruction status");
  m_delay_button->setMenu(m_delay_menu.get());
  m_delay_button->setPopupMode(QToolButton::InstantPopup);
  addWidget(m_delay_button);

  m_settings_button->setText("Other");
  m_settings_button->setIcon(sup::gui::utils::GetIcon("menu.svg"));
  m_settings_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_settings_button->setToolTip("Other settings");
  m_settings_button->setMenu(m_settings_menu.get());
  m_settings_button->setPopupMode(QToolButton::InstantPopup);
  addWidget(m_settings_button);
}

void MonitorRealTimeToolBar::ReadSettings()
{
  QSettings settings;
  m_current_tick_timeout =
      settings.value(kTickTimeOutSettingName, GetDefaultTickTimeoutMsc()).toInt();
}

void MonitorRealTimeToolBar::WriteSettings()
{
  QSettings settings;
  settings.setValue(kTickTimeOutSettingName, m_current_tick_timeout);
}

MonitorRealTimeToolBar::~MonitorRealTimeToolBar()
{
  WriteSettings();
}

int MonitorRealTimeToolBar::GetCurrentTickTimeout()
{
  return m_current_tick_timeout;
}

std::unique_ptr<QMenu> MonitorRealTimeToolBar::CreateDelayMenu()
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
      emit changeDelayRequest(delay);
    };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

std::unique_ptr<QMenu> MonitorRealTimeToolBar::CreateSettingsMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto scroll_action = result->addAction("Automatically scroll to current instruction");
  scroll_action->setToolTip(
      "Automatically scrolls tree view to display currently executed instruction");
  scroll_action->setCheckable(true);
  scroll_action->setChecked(true);
  connect(scroll_action, &QAction::toggled, this,
          &MonitorRealTimeToolBar::scrollToSelectionRequest);

  return result;
}

}  // namespace sequencergui
