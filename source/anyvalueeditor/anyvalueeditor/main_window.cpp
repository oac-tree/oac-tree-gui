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

#include "anyvalueeditor/main_window.h"

#include "sup/dto/anyvalue.h"
#include "anyvalueeditor/editor_widget.h"

#include <QCoreApplication>
#include <QFileDialog>
#include <QMenuBar>
#include <QSettings>

namespace
{
const QString main_window_group = "MainWindow";
const QString size_key = "size";
const QString pos_key = "pos";
}  // namespace

namespace anyvalueeditor
{
MainWindow::MainWindow()
{
  InitApplication();
}

MainWindow::~MainWindow() = default;

void MainWindow::closeEvent(QCloseEvent* event)
{
  WriteSettings();
  QMainWindow::closeEvent(event);
}

void MainWindow::InitApplication()
{
  QCoreApplication::setApplicationName("anyvalue-editor");
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
  InitComponents();
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
      m_editor_widget->ImportAnyValueFromFile(file_name);
    }
  };
  connect(open_action, &QAction::triggered, on_open_action);
}

void MainWindow::InitComponents()
{
  m_editor_widget = new EditorWidget;
  setCentralWidget(m_editor_widget);
}

void MainWindow::WriteSettings()
{
  QSettings settings;
  settings.beginGroup(main_window_group);
  settings.setValue(size_key, size());
  settings.setValue(pos_key, pos());
  settings.endGroup();
}

}  // namespace anyvalueeditor
