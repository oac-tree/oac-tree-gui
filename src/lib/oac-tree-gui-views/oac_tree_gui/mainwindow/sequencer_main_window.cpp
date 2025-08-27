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

#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/sequencer_settings_constants.h>
#include <oac_tree_gui/model/sequencer_settings_model.h>
#include <oac_tree_gui/style/style_helper.h>
#include <oac_tree_gui/views/composer/sequencer_composer_view.h>
#include <oac_tree_gui/views/explorer/sequencer_explorer_view.h>
#include <oac_tree_gui/views/operation/operation_monitor_view.h>

#include <sup/gui/mainwindow/settings_helper.h>

#include <mvvm/widgets/main_vertical_bar_widget.h>
#include <mvvm/widgets/widget_utils.h>

#include <QCloseEvent>
#include <QSettings>
#include <QStatusBar>

namespace oac_tree_gui
{

namespace
{
const QString kGroupName = "MainWindow";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";
const QString kWindowPosSettingName = kGroupName + "/" + "pos";

}  // namespace

SequencerMainWindow::SequencerMainWindow(SequencerMainWindowContext& context)
    : m_context(context), m_models(CreateProject())
{
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

SequencerMainWindow::~SequencerMainWindow() = default;

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
}

void SequencerMainWindow::InitComponents()
{
  using sup::gui::IconColorFlavor;

  m_action_manager = new SequencerMainWindowActions(m_context, m_models.get(), this);

  m_tab_widget = new mvvm::MainVerticalBarWidget;
  m_tab_widget->SetBaseColor(GetMainToolBarColor());

  m_explorer_view = new SequencerExplorerView(m_context.GetCommandService());
  m_tab_widget->AddWidget(m_explorer_view, "Explore",
                          FindIcon("file-search-outline", IconColorFlavor::kForDarkThemes));

  m_composer_view = new SequencerComposerView(m_context.GetCommandService());
  m_tab_widget->AddWidget(m_composer_view, "Compose",
                          FindIcon("graph-outline", IconColorFlavor::kForDarkThemes));

  m_operation_view =
      new OperationMonitorView(m_context.GetCommandService(), OperationPresentationMode::kIdeMode);
  m_tab_widget->AddWidget(
      m_operation_view, "Run",
      FindIcon("chevron-right-circle-outline", IconColorFlavor::kForDarkThemes));

  m_tab_widget->AddSpacer();

  m_tab_widget->SetCurrentIndex(0);

  setCentralWidget(m_tab_widget);

  connect(m_action_manager, &SequencerMainWindowActions::RestartApplicationRequest, this,
          &SequencerMainWindow::OnRestartRequest);

  m_action_manager->SetupStatusBar(m_tab_widget->GetStatusBar());
}

void SequencerMainWindow::ReadSettings()
{
  const QSettings settings;
  const auto default_size = QSize(mvvm::utils::UnitSize(80), mvvm::utils::UnitSize(60));
  resize(settings.value(kWindowSizeSettingName, default_size).toSize());

  const auto default_pos = QPoint(mvvm::utils::UnitSize(20), mvvm::utils::UnitSize(40));
  move(settings.value(kWindowPosSettingName, default_pos).toPoint());
}

void SequencerMainWindow::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWindowSizeSettingName, size());
  settings.setValue(kWindowPosSettingName, pos());

  ::sup::gui::WriteApplicationSettings(GetContext().GetSettingsModel());
}

bool SequencerMainWindow::CanCloseApplication()
{
  if (m_operation_view->HasRunningJobs())
  {
    if (ShouldStopRunningJobs())
    {
      m_operation_view->StopAllJobs();
    }
    else
    {
      return false;
    }
  }

  if (m_action_manager->CloseCurrentProject())
  {
    WriteSettings();
    return true;
  }

  return false;
}

void SequencerMainWindow::OnRestartRequest(sup::gui::AppExitCode exit_code)
{
  if (CanCloseApplication())
  {
    QCoreApplication::exit(exit_code);
  }
}

void SequencerMainWindow::OnProjectLoad()
{
  const auto enable_undo = m_context.GetSettingsModel().Data<bool>(constants::kUseUndoSetting);
  const auto undo_limit = m_context.GetSettingsModel().Data<int>(constants::kUndoLimitSetting);
  m_models->GetSequencerModel()->SetUndoEnabled(enable_undo, undo_limit);

  m_explorer_view->SetModel(m_models->GetSequencerModel());
  m_composer_view->SetModel(m_models->GetSequencerModel());
  m_operation_view->SetModels(m_models.get());

  UpdateProjectNames();
}

void SequencerMainWindow::UpdateProjectNames()
{
  m_action_manager->UpdateProjectNames();
}

std::unique_ptr<ApplicationModels> SequencerMainWindow::CreateProject()
{
  mvvm::ProjectContext context;
  context.modified_callback = [this]() { UpdateProjectNames(); };
  context.loaded_callback = [this]() { OnProjectLoad(); };
  context.saved_callback = [this]() { UpdateProjectNames(); };
  context.application_type = ApplicationModels::kApplicationType;
  return std::make_unique<ApplicationModels>(context);
}

}  // namespace oac_tree_gui
