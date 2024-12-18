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

#include "sequencer_main_window.h"

#include "color_palette.h"
#include "main_window_helper.h"
#include "sequencer_main_window_actions.h"
#include "settings_helper.h"

#include <sequencergui/model/application_models.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/settings_model.h>
#include <sequencergui/views/composer/sequencer_composer_view.h>
#include <sequencergui/views/explorer/sequencer_explorer_view.h>
#include <sequencergui/views/operation/operation_monitor_view.h>
#include <sup/gui/style/style_helper.h>

#include <mvvm/widgets/main_vertical_bar_widget.h>
#include <mvvm/widgets/widget_utils.h>

#include <QCloseEvent>
#include <QSettings>

namespace sequencergui
{

namespace
{
const QString kGroupName = "MainWindow";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";
const QString kWindowPosSettingName = kGroupName + "/" + "pos";

}  // namespace

SequencerMainWindow::SequencerMainWindow() : m_models(CreateProject())
{
  InitApplication();

  m_models->CreateEmpty();
}

bool SequencerMainWindow::ImportProcedure(const QString& file_name)
{
  return file_name.isEmpty() ? false : m_operation_view->OnImportJobRequest(file_name);
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
  using sup::gui::utils::AppIconColorFlavor;

  m_action_manager = new SequencerMainWindowActions(m_models.get(), this);

  m_tab_widget = new mvvm::MainVerticalBarWidget;
  m_tab_widget->SetBaseColor(GetMainToolBarColor());

  m_explorer_view = new SequencerExplorerView;
  m_tab_widget->AddWidget(
      m_explorer_view, "Explore",
      sup::gui::utils::GetIcon("file-search-outline", AppIconColorFlavor::kForDarkThemes));

  m_composer_view = new SequencerComposerView;
  m_tab_widget->AddWidget(
      m_composer_view, "Compose",
      sup::gui::utils::GetIcon("graph-outline", AppIconColorFlavor::kForDarkThemes));

  m_operation_view = new OperationMonitorView(OperationPresentationMode::kIdeMode);
  m_tab_widget->AddWidget(
      m_operation_view, "Run",
      sup::gui::utils::GetIcon("chevron-right-circle-outline", AppIconColorFlavor::kForDarkThemes));

  m_tab_widget->AddSpacer();

  m_tab_widget->SetCurrentIndex(0);

  setCentralWidget(m_tab_widget);

  connect(m_action_manager, &SequencerMainWindowActions::RestartApplicationRequest, this,
          &SequencerMainWindow::OnRestartRequest);
}

void SequencerMainWindow::ReadSettings()
{
  const QSettings settings;
  const auto default_size = QSize(mvvm::utils::UnitSize(80), mvvm::utils::UnitSize(60));
  resize(settings.value(kWindowSizeSettingName, default_size).toSize());

  const auto default_pos = QPoint(mvvm::utils::UnitSize(20), mvvm::utils::UnitSize(40));
  move(settings.value(kWindowPosSettingName, default_pos).toPoint());

  // global persistent setting stored in SettingsModel
  ReadGlobalSettings();
}

void SequencerMainWindow::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWindowSizeSettingName, size());
  settings.setValue(kWindowPosSettingName, pos());
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
    sup::gui::ShutdownApplication();
    QCoreApplication::exit(exit_code);
  }
}

void SequencerMainWindow::OnProjectLoad()
{
  m_explorer_view->SetModel(m_models->GetSequencerModel());
  m_composer_view->SetModel(m_models->GetSequencerModel());
  m_operation_view->SetModels(m_models.get());

  const auto enable_undo = GetGlobalSettings().Data<bool>(kUseUndoSetting);
  const auto undo_limit = GetGlobalSettings().Data<int>(kUndoLimitSetting);
  m_models->GetSequencerModel()->SetUndoEnabled(enable_undo, undo_limit);

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

}  // namespace sequencergui
