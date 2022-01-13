/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
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
#include "sequencergui/mainwindow/sequencerxmlview.h"
#include "sequencergui/mainwindow/settingsview.h"
#include "sequencergui/mainwindow/styleutils.h"
#include "sequencergui/model/procedureexamples.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/monitor/sequencermonitorview.h"

#include "mvvm/widgets/mainverticalbarwidget.h"

#include <QCoreApplication>
#include <QFileDialog>
#include <QMenuBar>
#include <QSettings>
#include <QTabWidget>

namespace
{
const QString main_window_group = "MainWindow";
const QString size_key = "size";
const QString pos_key = "pos";
}  // namespace

namespace sequencergui
{
MainWindow::MainWindow()
    : m_model(std::make_unique<SequencerModel>())
    , m_tab_widget(new mvvm::MainVerticalBarWidget)
{
  InitApplication();
  InitComponents();
  setCentralWidget(m_tab_widget);

  PopulateModel();

  m_xml_view->SetModel(m_model.get());
  m_settings_view->SetModel(m_model.get());
  m_composer_view->SetModel(m_model.get());
  m_monitor_view->SetModel(m_model.get());
}

MainWindow::~MainWindow() = default;

void MainWindow::closeEvent(QCloseEvent* event)
{
  WriteSettings();
  QMainWindow::closeEvent(event);
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

  InitMenu();
}

void MainWindow::InitComponents()
{
  m_xml_view = new SequencerXMLView;
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
}

void MainWindow::WriteSettings()
{
  QSettings settings;
  settings.beginGroup(main_window_group);
  settings.setValue(size_key, size());
  settings.setValue(pos_key, pos());
  settings.endGroup();
}

void MainWindow::InitMenu()
{
  auto file_menu = menuBar()->addMenu("&File");

  // open file
  auto open_action = new QAction("&Open...", this);
  file_menu->addAction(open_action);
  auto on_open_action = [&]()
  {
    QString file_name = QFileDialog::getOpenFileName(this);
    if (!file_name.isEmpty())
    {
      m_xml_view->SetXMLFile(file_name);
    }
  };
  connect(open_action, &QAction::triggered, on_open_action);

  file_menu->addSeparator();

  auto exit_action = new QAction("E&xit Application", this);
  file_menu->addAction(exit_action);
  exit_action->setShortcuts(QKeySequence::Quit);
  exit_action->setStatusTip("Exit the application");
  connect(exit_action, &QAction::triggered, this, &QMainWindow::close);

  menuBar()->addMenu("&Analyse");
  menuBar()->addMenu("&Tools");
  menuBar()->addMenu("&Window");
  menuBar()->addMenu("&Help");
}

void MainWindow::PopulateModel()
{
  //  Examples::AddCopyProcedure(m_model.get());
  //  Examples::AddLocalIncludeProcedure(m_model.get());
  Examples::AddUserChoiceProcedure(m_model.get());
  Examples::AddInputProcedure(m_model.get());
}

}  // namespace sequi
