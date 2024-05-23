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

#include "main_window_helper.h"
#include "sequencer_main_window_actions.h"
#include "settings_view.h"

#include <sequencergui/composer/sequencer_composer_view.h>
#include <sequencergui/explorer/sequencer_explorer_view.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/procedure_examples.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/operation/operation_monitor_view.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/application_helper.h>
#include <sup/gui/widgets/style_utils.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/widgets/main_vertical_bar_widget.h>
#include <mvvm/widgets/widget_utils.h>

#include <QCloseEvent>
#include <QSettings>

namespace
{
const QString kGroupName = "MainWindow";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";
const QString kWindowPosSettingName = kGroupName + "/" + "pos";
}  // namespace

namespace sequencergui
{
SequencerMainWindow::SequencerMainWindow() : m_models(std::make_unique<ApplicationModels>())
{
  PopulateModel();

  InitApplication();

  m_explorer_view->SetModel(m_models->GetSequencerModel());
  m_composer_view->SetModel(m_models->GetSequencerModel());
  m_operation_view->SetApplicationModels(m_models.get());
  m_settings_view->SetApplicationModels(m_models.get());

  OnProjectLoad();
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
  sup::gui::AppRegisterMainMenuBar(menuBar(),
                                   {sup::gui::constants::kFileMenu, sup::gui::constants::kEditMenu,
                                    sup::gui::constants::kViewMenu, sup::gui::constants::kToolsMenu,
                                    sup::gui::constants::kHelpMenu});

  m_action_manager = new SequencerMainWindowActions(m_models->GetSequencerModel(), this);

  m_tab_widget = new mvvm::MainVerticalBarWidget;
  m_tab_widget->SetBaseColor("#005291");

  m_explorer_view = new SequencerExplorerView;
  m_tab_widget->AddWidget(m_explorer_view, "Explore",
                          sup::gui::utils::GetIcon("file-search-outline-light.svg"));

  m_composer_view = new SequencerComposerView;
  m_tab_widget->AddWidget(m_composer_view, "Compose",
                          sup::gui::utils::GetIcon("graph-outline-light.svg"));

  m_operation_view = new OperationMonitorView(OperationMonitorView::kIdeMode);
  m_tab_widget->AddWidget(m_operation_view, "Run",
                          sup::gui::utils::GetIcon("chevron-right-circle-outline-light.svg"));

  m_tab_widget->AddSpacer();

  m_settings_view = new SettingsView;
  m_tab_widget->AddWidget(m_settings_view, "", sup::gui::utils::GetIcon("cog-outline-light.svg"));

  m_tab_widget->SetCurrentIndex(0);

  setCentralWidget(m_tab_widget);

  connect(m_action_manager, &SequencerMainWindowActions::RestartApplicationRequest, this,
          &SequencerMainWindow::OnRestartRequest);
  connect(m_action_manager, &SequencerMainWindowActions::ProjectLoaded, this,
          &SequencerMainWindow::OnProjectLoad);
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
}

void SequencerMainWindow::PopulateModel()
{
  //
  auto model = m_models->GetSequencerModel();
  auto procedure = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  procedure->SetDisplayName("Untitled");

  //  Examples::AddCopyProcedure(m_models->GetSequencerModel());
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
  m_composer_view->OnProjectLoad();
}

}  // namespace sequencergui
