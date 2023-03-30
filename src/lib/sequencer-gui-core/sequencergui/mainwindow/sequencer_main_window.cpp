/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include "sequencer_main_window_actions.h"
#include "settings_view.h"

#include <sequencergui/composer/sequencer_composer_view.h>
#include <sequencergui/explorer/sequencer_explorer_view.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_examples.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/monitor/sequencer_monitor_view.h>
#include <sequencergui/utils/style_utils.h>

#include <mvvm/widgets/main_vertical_bar_widget.h>
#include <mvvm/widgets/widget_utils.h>

#include <QCloseEvent>
#include <QCoreApplication>
#include <QSettings>

namespace
{
const QString main_window_group = "MainWindow";
const QString size_key = "size";
const QString pos_key = "pos";
}  // namespace

namespace sequencergui
{
SequencerMainWindow::SequencerMainWindow() : m_models(std::make_unique<ApplicationModels>())
{
  PopulateModel();

  InitApplication();

  m_explorer_view->SetModel(m_models->GetSequencerModel());
  m_composer_view->SetModel(m_models->GetSequencerModel());
  m_monitor_view->SetApplicationModels(m_models.get());
  m_settings_view->SetApplicationModels(m_models.get());
}

SequencerMainWindow::~SequencerMainWindow() = default;

void SequencerMainWindow::closeEvent(QCloseEvent* event)
{
  if (m_action_manager->CanCloseApplication())
  {
    WriteSettings();
    QMainWindow::closeEvent(event);
  }
  else
  {
    event->ignore();
  }
}

void SequencerMainWindow::InitApplication()
{
  QSettings settings;
  if (settings.childGroups().contains(main_window_group))
  {
    settings.beginGroup(main_window_group);
    resize(settings.value(size_key, QSize(mvvm::utils::UnitSize(80), mvvm::utils::UnitSize(60)))
               .toSize());
    move(settings.value(pos_key, QPoint(mvvm::utils::UnitSize(20), mvvm::utils::UnitSize(40)))
             .toPoint());
    settings.endGroup();
  }

  InitComponents();
}

void SequencerMainWindow::InitComponents()
{
  m_action_manager = new SequencerMainWindowActions(m_models->GetSequencerModel(), this);

  m_tab_widget = new mvvm::MainVerticalBarWidget;
  m_tab_widget->SetBaseColor("#005291");

  m_explorer_view = new SequencerExplorerView;
  m_tab_widget->AddWidget(m_explorer_view, "Explore",
                          styleutils::GetIcon("file-search-outline-light.svg"));

  m_composer_view = new SequencerComposerView;
  m_tab_widget->AddWidget(m_composer_view, "Compose",
                          styleutils::GetIcon("graph-outline-light.svg"));

  m_monitor_view = new SequencerMonitorView;
  m_tab_widget->AddWidget(m_monitor_view, "Run",
                          styleutils::GetIcon("chevron-right-circle-outline-light.svg"));

  m_tab_widget->AddSpacer();

  m_settings_view = new SettingsView;
  m_tab_widget->AddWidget(m_settings_view, "", styleutils::GetIcon("cog-outline-light.svg"));

  m_tab_widget->SetCurrentIndex(0);

  setCentralWidget(m_tab_widget);
}

void SequencerMainWindow::WriteSettings()
{
  QSettings settings;
  settings.beginGroup(main_window_group);
  settings.setValue(size_key, size());
  settings.setValue(pos_key, pos());
  settings.endGroup();
}

void SequencerMainWindow::PopulateModel()
{
  //  Examples::AddCopyProcedure(m_models->GetSequencerModel());
  //  Examples::AddLocalIncludeProcedure(m_models->GetSequencerModel());
  //  Examples::AddUserChoiceProcedure(m_models->GetSequencerModel());
  //  Examples::AddInputProcedure(m_models->GetSequencerModel());
  //  Examples::AddComplexAlignmentProcedure(m_models->GetSequencerModel());
  //  Examples::AddComplexAlignmentProcedureV2(m_models->GetSequencerModel());

  //  auto job_model = m_models->GetJobModel();
  //  job_model->InsertItem<JobItem>();
  //  job_model->InsertItem<JobItem>()->SetStatus("ccc");
}

}  // namespace sequencergui
