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

#include "pvmonitor_main_window.h"

#include "pvmonitor_project.h"
#include "sequencer_main_window_context.h"

#include <oac_tree_gui/views/pvmonitor/monitor_main_window_actions.h>
#include <oac_tree_gui/views/pvmonitor/monitor_widget.h>

#include <sup/gui/app/app_view_context.h>
#include <sup/gui/settings/i_settings_manager.h>
#include <sup/gui/widgets/window_info.h>

namespace
{
//! Settings group of main window geometry.
constexpr auto kWindowSettingsGroup = "MainWindow";
}  // namespace

namespace oac_tree_gui
{

PvMonitorMainWindow::PvMonitorMainWindow(SequencerMainWindowContext& context)
    : m_context(context), m_project(CreateProject())
{
  InitApplication();

  m_project->CreateEmpty();
}

bool PvMonitorMainWindow::ImportProcedure(const QString& file_name)
{
  (void)file_name;
  // Not implemented yet
  return false;
}

PvMonitorMainWindow::~PvMonitorMainWindow() = default;

void PvMonitorMainWindow::closeEvent(QCloseEvent* event)
{
  m_monitor_widget->OnStopMonitoringRequest();
  WriteSettings();
  QMainWindow::closeEvent(event);
}

void PvMonitorMainWindow::InitApplication()
{
  ReadSettings();
  m_actions = new MonitorMainWindowActions(m_project.get(), this);

  m_monitor_widget = new MonitorWidget(m_context.GetViewContext());
  setCentralWidget(m_monitor_widget);
}

void PvMonitorMainWindow::ReadSettings()
{
  auto& settings_manager = m_context.GetSettingsManager();
  const auto window_info = settings_manager.Get<sup::gui::WindowInfo>(
      kWindowSettingsGroup, sup::gui::SettingsScope::kApplication);
  if (!restoreGeometry(window_info.geometry))
  {
    resize(QSize(800, 600));
  }
  m_window_settings_connection =
      settings_manager.RegisterProvider(kWindowSettingsGroup, sup::gui::SettingsScope::kApplication,
                                        [this]() { return sup::gui::WindowInfo{saveGeometry()}; });
}

void PvMonitorMainWindow::WriteSettings()
{
  m_context.GetSettingsManager().Collect(sup::gui::SettingsScope::kApplication);
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
