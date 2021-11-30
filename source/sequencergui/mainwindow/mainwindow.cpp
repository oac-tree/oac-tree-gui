/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
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
#include "sequencergui/mainwindow/styleutils.h"
#include "sequencergui/mainwindow/settingsview.h"
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

namespace sequi
{
MainWindow::MainWindow()
    : m_model(std::make_unique<SequencerModel>())
    , m_tab_widget(new ModelView::MainVerticalBarWidget)
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
  m_tab_widget->addWidget(m_xml_view, "Explore", StyleUtils::GetIcon("file-search-outline-light.svg"));

  m_composer_view = new SequencerComposerView;
  m_tab_widget->addWidget(m_composer_view, "Compose", StyleUtils::GetIcon("graph-outline-light.svg"));

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
  auto openAction = new QAction("&Open...", this);
  file_menu->addAction(openAction);
  auto onOpenAction = [&]()
  {
    QString file_name = QFileDialog::getOpenFileName(this);
    if (!file_name.isEmpty())
    {
      m_xml_view->SetXMLFile(file_name);
    }
  };
  connect(openAction, &QAction::triggered, onOpenAction);

  menuBar()->addMenu("&Analyse");
  menuBar()->addMenu("&Tools");
  menuBar()->addMenu("&Window");
  menuBar()->addMenu("&Help");
}

void MainWindow::PopulateModel()
{
  // first procedure
  {
    auto procedure_item = m_model->InsertItem<ProcedureItem>(m_model->GetProcedureContainer());
    auto repeat = m_model->InsertItem<RepeatItem>(procedure_item->GetInstructionContainer());
    repeat->SetRepeatCount(-1);
    auto sequence = m_model->InsertItem<SequenceItem>(repeat);

    auto copy0 = m_model->InsertItem<CopyItem>(sequence);
    copy0->SetInput("var_const42");
    copy0->SetOutput("var0");
    m_model->InsertItem<WaitItem>(sequence);

    auto copy1 = m_model->InsertItem<CopyItem>(sequence);
    copy1->SetInput("var_const0");
    copy1->SetOutput("var1");
    m_model->InsertItem<WaitItem>(sequence);

    auto copy2 = m_model->InsertItem<CopyItem>(sequence);
    copy2->SetInput("var_const42");
    copy2->SetOutput("var1");
    m_model->InsertItem<WaitItem>(sequence);

    auto copy3 = m_model->InsertItem<CopyItem>(sequence);
    copy3->SetInput("var_const0");
    copy3->SetOutput("var0");
    m_model->InsertItem<WaitItem>(sequence);

    auto var0 = m_model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
    var0->SetName("var0");
    var0->SetJsonType(R"({"type":"uint32"})");
    var0->SetJsonValue("0");

    auto var1 = m_model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
    var1->SetName("var1");
    var1->SetJsonType(R"({"type":"uint32"})");
    var1->SetJsonValue("0");

    auto var_const42 = m_model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
    var_const42->SetName("var_const42");
    var_const42->SetJsonType(R"({"type":"uint32"})");
    var_const42->SetJsonValue("42");

    auto var_const0 = m_model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
    var_const0->SetName("var_const0");
    var_const0->SetJsonType(R"({"type":"uint32"})");
    var_const0->SetJsonValue("0");
  }

  // second procedure
  {
    auto procedure_item = m_model->InsertItem<ProcedureItem>(m_model->GetProcedureContainer());
    auto sequence = m_model->InsertItem<SequenceItem>(procedure_item->GetInstructionContainer());
    m_model->InsertItem<WaitItem>(sequence);
    m_model->InsertItem<WaitItem>(sequence);
    m_model->InsertItem<WaitItem>(sequence);
    m_model->InsertItem<WaitItem>(sequence);
    m_model->InsertItem<WaitItem>(sequence);
    m_model->InsertItem<WaitItem>(sequence);
    m_model->InsertItem<WaitItem>(sequence);
  }
}

}  // namespace sequi
