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

#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/monitor_main_window_actions.h>
#include <sequencergui/pvmonitor/monitor_model.h>
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

PvMonitorMainWindow::PvMonitorMainWindow() : m_model(std::make_unique<MonitorModel>())
{
  InitApplication();
  OnProjectLoad();
}

void PvMonitorMainWindow::OnProjectLoad()
{
  m_monitor_widget->SetWorkspaceItem(m_model->GetWorkspaceItem());
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
  m_actions = new MonitorMainWindowActions(m_model.get(), this);

  m_monitor_widget = new MonitorWidget(m_model.get());
  setCentralWidget(m_monitor_widget);

  connect(m_actions, &MonitorMainWindowActions::ProjectLoaded, this,
          &PvMonitorMainWindow::OnProjectLoad);
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

}  // namespace sequencergui
