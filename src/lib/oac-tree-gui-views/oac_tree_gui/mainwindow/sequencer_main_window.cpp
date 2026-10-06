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

#include "sequencer_main_window.h"

#include "color_palette.h"
#include "main_window_helper.h"
#include "sequencer_main_window_actions.h"

#include <oac_tree_gui/mainwindow/sequencer_main_window_context.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/project_model.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/style/style_helper.h>
#include <oac_tree_gui/views/composer/sequencer_composer_view.h>
#include <oac_tree_gui/views/explorer/sequencer_explorer_view.h>
#include <oac_tree_gui/views/operation/operation_monitor_view.h>

#include <sup/gui/app/app_view_context.h>
#include <sup/gui/model/undo_settings_item.h>
#include <sup/gui/settings/i_settings_manager.h>
#include <sup/gui/settings/project_settings_storage.h>

#include <mvvm/widgets/app_restarter.h>
#include <mvvm/widgets/main_vertical_bar_widget.h>

#include <QCloseEvent>
#include <QSettings>

namespace oac_tree_gui
{

namespace
{
const QString kGroupName = "MainWindow";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";
const QString kWindowPosSettingName = kGroupName + "/" + "pos";

}  // namespace

SequencerMainWindow::SequencerMainWindow(SequencerMainWindowContext& context)
    : m_context(context)
    , m_models(CreateProject())
    , m_project_settings_storage(std::make_unique<sup::gui::ProjectSettingsStorage>(
          [this]() { return m_models->GetProjectModel(); }))
{
  m_context.SetProjectSettingsStorage(m_project_settings_storage.get());
  InitApplication();

  m_models->CreateEmpty();
}

bool SequencerMainWindow::ImportProcedure(const QString& file_name)
{
  return file_name.isEmpty() ? false : m_operation_view->OnImportJobRequest(file_name);
}

SequencerMainWindowContext& SequencerMainWindow::GetContext() const
{
  return m_context;
}

SequencerMainWindow::~SequencerMainWindow()
{
  m_context.SetProjectSettingsStorage(nullptr);
}

void SequencerMainWindow::closeEvent(QCloseEvent* event)
{
  if (CanCloseApplication())
  {
    QMainWindow::closeEvent(event);
    return;
  }

  event->ignore();
}

void SequencerMainWindow::InitApplication()
{
  ReadSettings();
  InitComponents();

  m_undo_settings_connection = m_context.GetSettingsManager().Subscribe<sup::gui::UndoInfo>(
      sup::gui::kUndoSettingsGroup, sup::gui::SettingsScope::kApplication,
      [this](const sup::gui::UndoInfo& undo_info) { ApplyUndoSettings(undo_info); });
}

void SequencerMainWindow::InitComponents()
{
  m_action_manager = new SequencerMainWindowActions(m_context, m_models.get(), this);

  m_tab_widget = new mvvm::MainVerticalBarWidget;
  m_tab_widget->SetBaseColor(GetMainToolBarColor());

  m_explorer_view = new SequencerExplorerView(m_context.GetViewContext());
  m_tab_widget->AddWidget(m_explorer_view, "Explore",
                          FindIcon("file-search-outline", mvvm::ColorFlavor::kForDarkThemes));

  m_composer_view = new SequencerComposerView(m_context.GetViewContext());
  m_tab_widget->AddWidget(m_composer_view, "Compose",
                          FindIcon("graph-outline", mvvm::ColorFlavor::kForDarkThemes));

  m_operation_view =
      new OperationMonitorView(m_context.GetViewContext(), OperationPresentationMode::kIdeMode);
  m_tab_widget->AddWidget(
      m_operation_view, "Run",
      FindIcon("chevron-right-circle-outline", mvvm::ColorFlavor::kForDarkThemes));

  m_tab_widget->AddSpacer();

  m_tab_widget->SetCurrentIndex(0);

  setCentralWidget(m_tab_widget);

  connect(m_action_manager, &SequencerMainWindowActions::ExitRequest, this,
          &SequencerMainWindow::OnExitRequest);

  m_action_manager->SetupStatusBar(m_tab_widget->GetStatusBar());
}

void SequencerMainWindow::ReadSettings()
{
  const QSettings settings;
  resize(settings.value(kWindowSizeSettingName, QSize(1024, 768)).toSize());

  move(settings.value(kWindowPosSettingName, QPoint(200, 400)).toPoint());
}

void SequencerMainWindow::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWindowSizeSettingName, size());
  settings.setValue(kWindowPosSettingName, pos());

  m_context.GetSettingsManager().Collect(sup::gui::SettingsScope::kApplication);
}

bool SequencerMainWindow::CanCloseApplication()
{
  if (m_operation_view->HasRunningJobs())
  {
    switch (ShouldStopRunningJobs())
    {
    case RunningJobsCloseAction::kStopAllJobs:
      m_operation_view->StopJobs(StopScope::kAll);
      break;
    case RunningJobsCloseAction::kStopLocalJobs:
      m_operation_view->StopJobs(StopScope::kLocalOnly);
      break;
    case RunningJobsCloseAction::kNoAction:
      return false;
    }
  }

  // settings providers may access models destroyed by project close
  WriteSettings();

  return m_action_manager->CloseCurrentProject();
}

void SequencerMainWindow::OnExitRequest(mvvm::AppExitRequest exit_request)
{
  mvvm::AppRestarter::StoreExitRequest(exit_request);
  if (!close())
  {
    // close cancelled by the user
    mvvm::AppRestarter::StoreExitRequest(mvvm::AppExitRequest::NormalExit);
  }
}

void SequencerMainWindow::OnProjectLoad()
{
  ApplyUndoSettings(m_context.GetSettingsManager().Get<sup::gui::UndoInfo>(
      sup::gui::kUndoSettingsGroup, sup::gui::SettingsScope::kApplication));

  m_explorer_view->SetModel(m_models->GetSequencerModel());
  m_composer_view->SetModels(m_models.get());
  m_operation_view->SetModels(m_models.get());

  // after views got new models, so procedure ids are resolved
  m_context.GetSettingsManager().NotifyAll(sup::gui::SettingsScope::kProject);

  UpdateProjectNames();
}

void SequencerMainWindow::UpdateProjectNames()
{
  m_action_manager->UpdateProjectNames();
}

void SequencerMainWindow::ApplyUndoSettings(const sup::gui::UndoInfo& undo_info)
{
  if (auto sequencer_model = m_models->GetSequencerModel(); sequencer_model)
  {
    sequencer_model->SetUndoEnabled(undo_info.is_enabled, undo_info.undo_limit);
  }
}

std::unique_ptr<ApplicationModels> SequencerMainWindow::CreateProject()
{
  mvvm::ProjectContext context;
  context.modified_callback = [this]() { UpdateProjectNames(); };
  context.loaded_callback = [this]() { OnProjectLoad(); };
  context.about_to_save_callback = [this]()
  { m_context.GetSettingsManager().Collect(sup::gui::SettingsScope::kProject); };
  context.saved_callback = [this]() { UpdateProjectNames(); };
  context.application_type = ApplicationModels::kApplicationType;
  return std::make_unique<ApplicationModels>(context);
}

}  // namespace oac_tree_gui
