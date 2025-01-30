/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#include "pvmonitor_main_window.h"

#include "pvmonitor_project.h"

#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/views/pvmonitor/monitor_main_window_actions.h>
#include <oac_tree_gui/views/pvmonitor/monitor_widget.h>

#include <QMenuBar>
#include <QSettings>

namespace
{
const QString kGroupName = "MainWindow";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";
const QString kWindowPosSettingName = kGroupName + "/" + "pos";
}  // namespace

namespace oac_tree_gui
{

PvMonitorMainWindow::PvMonitorMainWindow() : m_project(CreateProject())
{
  InitApplication();

  m_project->CreateEmpty();
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
  m_actions = new MonitorMainWindowActions(m_project.get(), this);

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
  m_monitor_widget->SetModel(m_project->GetMonitorModel());
  UpdateProjectNames();
}

void PvMonitorMainWindow::UpdateProjectNames()
{
  m_actions->UpdateProjectNames();
}

std::unique_ptr<PvMonitorProject> PvMonitorMainWindow::CreateProject()
{
  mvvm::ProjectContext context;
  context.loaded_callback = [this]() { OnProjectLoad(); };
  context.modified_callback = [this]() { UpdateProjectNames(); };
  context.saved_callback = [this]() { UpdateProjectNames(); };
  context.application_type = PvMonitorProject::kApplicationType;
  return std::make_unique<PvMonitorProject>(context);
}

}  // namespace oac_tree_gui
