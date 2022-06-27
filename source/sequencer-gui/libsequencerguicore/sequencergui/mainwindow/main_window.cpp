/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/mainwindow/main_window.h"

#include <mvvm/widgets/main_vertical_bar_widget.h>
#include <sequencergui/composer/sequencer_composer_view.h>
#include <sequencergui/explorer/sequencer_explorer_view.h>
#include <sequencergui/mainwindow/action_manager.h>
#include <sequencergui/mainwindow/settings_view.h>
#include <sequencergui/model/application_models.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/procedure_examples.h>
#include <sequencergui/monitor/sequencer_monitor_view.h>
#include <sequencergui/utils/style_utils.h>

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
MainWindow::MainWindow() : m_models(std::make_unique<ApplicationModels>())
{
  PopulateModel();

  InitApplication();

  m_xml_view->SetModel(m_models->GetSequencerModel());
  m_composer_view->SetModel(m_models->GetSequencerModel());
  m_monitor_view->SetApplicationModels(m_models.get());
  m_settings_view->SetApplicationModels(m_models.get());
}

MainWindow::~MainWindow() = default;

void MainWindow::closeEvent(QCloseEvent* event)
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

void MainWindow::InitApplication()
{
  QSettings settings;
  if (settings.childGroups().contains(main_window_group))
  {
    settings.beginGroup(main_window_group);
    resize(settings.value(size_key, QSize(styleutils::UnitSize(80), styleutils::UnitSize(60)))
               .toSize());
    move(settings.value(pos_key, QPoint(styleutils::UnitSize(20), styleutils::UnitSize(40)))
             .toPoint());
    settings.endGroup();
  }

  InitComponents();
}

void MainWindow::InitComponents()
{
  m_action_manager = new ActionManager(m_models->GetSequencerModel(), this);

  m_tab_widget = new mvvm::MainVerticalBarWidget;

  m_xml_view = new SequencerExplorerView;
  m_tab_widget->addWidget(m_xml_view, "Explore",
                          styleutils::GetIcon("file-search-outline-light.svg"));

  m_composer_view = new SequencerComposerView;
  m_tab_widget->addWidget(m_composer_view, "Compose",
                          styleutils::GetIcon("graph-outline-light.svg"));

  m_monitor_view = new SequencerMonitorView;
  m_tab_widget->addWidget(m_monitor_view, "Run",
                          styleutils::GetIcon("chevron-right-circle-outline-light.svg"));

  m_tab_widget->addSpacer();

  m_settings_view = new SettingsView;
  m_tab_widget->addWidget(m_settings_view, "", styleutils::GetIcon("cog-outline-light.svg"));

  m_tab_widget->setCurrentIndex(0);

  setCentralWidget(m_tab_widget);
}

void MainWindow::WriteSettings()
{
  QSettings settings;
  settings.beginGroup(main_window_group);
  settings.setValue(size_key, size());
  settings.setValue(pos_key, pos());
  settings.endGroup();
}

void MainWindow::PopulateModel()
{
  Examples::AddCopyProcedure(m_models->GetSequencerModel());
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
