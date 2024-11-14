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

#include "pvmonitor_main_window.h"

#include "pvmonitor_project.h"

#include <sequencergui/model/workspace_item.h>
#include <sequencergui/views/pvmonitor/monitor_main_window_actions.h>
#include <sequencergui/views/pvmonitor/monitor_widget.h>

#include <QMenuBar>
#include <QSettings>

namespace
{
const QString kGroupName = "MainWindow";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";
const QString kWindowPosSettingName = kGroupName + "/" + "pos";
}  // namespace

namespace sequencergui
{

PvMonitorMainWindow::PvMonitorMainWindow() : m_project_agent(CreateProjectAgent())
{
  InitApplication();

  m_project_agent->CreateEmpty();
}

PvMonitorMainWindow::~PvMonitorMainWindow() = default;

void PvMonitorMainWindow::closeEvent(QCloseEvent* event)
{
  WriteSettings();
  QMainWindow::closeEvent(event);
}

void PvMonitorMainWindow::InitApplication()
{
  ReadSettings();
  m_actions = new MonitorMainWindowActions(m_project_agent.get(), this);

  m_monitor_widget = new MonitorWidget;
  setCentralWidget(m_monitor_widget);
}

void PvMonitorMainWindow::ReadSettings()
{
  const QSettings settings;
  resize(settings.value(kWindowSizeSettingName, QSize(800, 600)).toSize());
  move(settings.value(kWindowPosSettingName, QPoint(200, 200)).toPoint());
}

void PvMonitorMainWindow::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWindowSizeSettingName, size());
  settings.setValue(kWindowPosSettingName, pos());
}

void PvMonitorMainWindow::OnProjectLoad()
{
  m_monitor_widget->SetModel(m_project_agent->GetMonitorModel());
}

void PvMonitorMainWindow::OnProjectModified() {}

std::unique_ptr<PvMonitorProject> PvMonitorMainWindow::CreateProjectAgent()
{
  auto modified_callback = [this]() { OnProjectModified(); };
  auto loaded_callback = [this]() { OnProjectLoad(); };
  return std::make_unique<PvMonitorProject>(modified_callback, loaded_callback);
}

}  // namespace sequencergui
