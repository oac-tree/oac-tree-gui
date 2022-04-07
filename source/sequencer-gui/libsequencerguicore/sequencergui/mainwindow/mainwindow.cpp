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

#include "sequencergui/mainwindow/mainwindow.h"

#include "sequencergui/composer/sequencercomposerview.h"
#include "sequencergui/explorer/sequencerexplorerview.h"
#include "sequencergui/mainwindow/actionmanager.h"
#include "sequencergui/mainwindow/settingsview.h"
#include "sequencergui/mainwindow/styleutils.h"
#include "sequencergui/model/procedureexamples.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/monitor/sequencermonitorview.h"

#include "mvvm/widgets/mainverticalbarwidget.h"

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
MainWindow::MainWindow() : m_model(std::make_unique<SequencerModel>())
{
  PopulateModel();

  InitApplication();

  m_xml_view->SetModel(m_model.get());
  m_settings_view->SetModel(m_model.get());
  m_composer_view->SetModel(m_model.get());
  m_monitor_view->SetModel(m_model.get());
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
  QCoreApplication::setApplicationName("sequencer-gui");
  QCoreApplication::setApplicationVersion("0.1");
  QCoreApplication::setOrganizationName("sequencer");

  QSettings settings;
  if (settings.childGroups().contains(main_window_group))
  {
    settings.beginGroup(main_window_group);
    resize(settings.value(size_key, QSize(400, 400)).toSize());
    move(settings.value(pos_key, QPoint(200, 200)).toPoint());
    settings.endGroup();
  }

  InitComponents();
}

void MainWindow::InitComponents()
{
  m_action_manager = new ActionManager(m_model.get(), this);

  m_tab_widget = new mvvm::MainVerticalBarWidget;

  m_xml_view = new SequencerExplorerView;
  m_tab_widget->addWidget(m_xml_view, "Explore",
                          StyleUtils::GetIcon("file-search-outline-light.svg"));

  m_composer_view = new SequencerComposerView;
  m_tab_widget->addWidget(m_composer_view, "Compose",
                          StyleUtils::GetIcon("graph-outline-light.svg"));

  m_monitor_view = new SequencerMonitorView;
  m_tab_widget->addWidget(m_monitor_view, "Run",
                          StyleUtils::GetIcon("chevron-right-circle-outline-light.svg"));

  m_tab_widget->addSpacer();

  m_settings_view = new SettingsView;
  m_tab_widget->addWidget(m_settings_view, "", StyleUtils::GetIcon("cog-outline-light.svg"));

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
    Examples::AddCopyProcedure(m_model.get());
  //  Examples::AddLocalIncludeProcedure(m_model.get());
  //  Examples::AddUserChoiceProcedure(m_model.get());
  //  Examples::AddInputProcedure(m_model.get());
  //  Examples::AddComplexAlignmentProcedure(m_model.get());
  //  Examples::AddComplexAlignmentProcedureV2(m_model.get());
}

}  // namespace sequencergui
