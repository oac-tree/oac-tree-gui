/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/monitor/monitortreetoolbar.h"

#include "sequencergui/mainwindow/styleutils.h"

#include "mvvm/widgets/widgetutils.h"

#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>

namespace
{
const int default_delay = 200;

QString GetDelayText(int delay)
{
  QString name = delay < 1000 ? QString("%1 msec").arg(delay) : QString("%1 sec").arg(delay / 1000);
  return name;
}
}  // namespace

namespace sequi
{
MonitorTreeToolBar::MonitorTreeToolBar(QWidget *parent)
    : QToolBar(parent)
    , m_run_button(new QToolButton)
    , m_pause_button(new QToolButton)
    , m_step_button(new QToolButton)
    , m_stop_button(new QToolButton)
    , m_delay_button(new QToolButton)
    , m_delay_menu(CreateDelayMenu())
    , m_app_menu(CreateAppMenu())
{
  setIconSize(StyleUtils::ToolBarIconSize());

  m_run_button->setText("Run");
  m_run_button->setIcon(StyleUtils::GetIcon("arrow-right-drop-circle-outline.svg"));
  m_run_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_run_button->setToolTip("Run procedure");
  connect(m_run_button, &QToolButton::clicked, this, &MonitorTreeToolBar::runRequest);
  addWidget(m_run_button);

  m_pause_button->setText("Pause");
  m_pause_button->setIcon(StyleUtils::GetIcon("pause-circle-outline.svg"));
  m_pause_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_pause_button->setToolTip("Pause sequence at the next occasion");
  connect(m_pause_button, &QToolButton::clicked, this, &MonitorTreeToolBar::pauseRequest);
  addWidget(m_pause_button);

  m_step_button->setText("Step");
  m_step_button->setIcon(StyleUtils::GetIcon("play-pause.svg"));
  m_step_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_step_button->setToolTip("Pause sequence at the next occasion");
  connect(m_step_button, &QToolButton::clicked, this, &MonitorTreeToolBar::stepRequest);
  addWidget(m_step_button);

  m_stop_button->setText("Stop");
  m_stop_button->setIcon(StyleUtils::GetIcon("stop-circle-outline.svg"));
  m_stop_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_stop_button->setToolTip("Stop procedure");
  connect(m_stop_button, &QToolButton::clicked, this, &MonitorTreeToolBar::stopRequest);
  addWidget(m_stop_button);

  addSeparator();

  m_delay_button->setText(GetDelayText(default_delay));
  m_delay_button->setIcon(StyleUtils::GetIcon("speedometer-slow.svg"));
  m_delay_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_delay_button->setToolTip("Artificial delay after each change of instruction status");
  m_delay_button->setMenu(m_delay_menu.get());
  m_delay_button->setPopupMode(QToolButton::InstantPopup);
  connect(m_stop_button, &QToolButton::clicked, this, &MonitorTreeToolBar::stopRequest);
  addWidget(m_delay_button);

  AddDotsMenu();
}

int MonitorTreeToolBar::GetDefaultDelay()
{
  return default_delay;
}

MonitorTreeToolBar::~MonitorTreeToolBar() = default;

void MonitorTreeToolBar::AddDotsMenu()
{
  InsertStrech();

  auto button = new QToolButton;
  button->setIcon(StyleUtils::GetIcon("dots-horizontal.svg"));
  button->setIconSize(StyleUtils::ToolBarIconSize());
  button->setMenu(m_app_menu.get());
  button->setPopupMode(QToolButton::InstantPopup);

  addWidget(button);
}

void MonitorTreeToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

std::unique_ptr<QMenu> MonitorTreeToolBar::CreateDelayMenu()
{
  const std::vector<int> delay_values = {0, 10, 50, 100, 200, 500, 1000, 2000, 5000, 10000};

  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);
  for (const auto &delay : delay_values)
  {
    auto name = GetDelayText(delay);
    auto action = result->addAction(name);
    auto on_action = [this, delay, name]()
    {
      m_delay_button->setText(name);
      emit changeDelayRequest(delay);
    };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

//! Returns menu for "dots" button for widget selection.

std::unique_ptr<QMenu> MonitorTreeToolBar::CreateAppMenu()
{
  auto result = std::make_unique<QMenu>();
  QStringList app_names = {"Tree", "NodeEditor"};

  int index{0};
  for (const auto &name : app_names)
  {
    auto action = result->addAction(name);
    auto on_action = [this, index]() { emit appChangeRequest(index); };
    connect(action, &QAction::triggered, this, on_action);
    ++index;
  }

  return result;
}

}  // namespace sequi
