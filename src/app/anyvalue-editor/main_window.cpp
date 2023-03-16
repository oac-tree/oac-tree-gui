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

#include "main_window.h"

#include <sup/gui/anyvalueeditor/anyvalue_editor.h>

#include <QCoreApplication>
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
  QSettings settings;
  if (settings.childGroups().contains(main_window_group))
  {
    settings.beginGroup(main_window_group);
    resize(settings.value(size_key, QSize(400, 400)).toSize());
    move(settings.value(pos_key, QPoint(200, 200)).toPoint());
    settings.endGroup();
  }

  InitComponents();
  InitMenu();
}

void MainWindow::InitMenu()
{
  Q_ASSERT(m_anyvalue_editor);

  auto file_menu = menuBar()->addMenu("&File");

  // open file
  auto open_action = new QAction("&Open...", this);
  file_menu->addAction(open_action);
  connect(open_action, &QAction::triggered, m_anyvalue_editor,
          &sup::gui::AnyValueEditor::OnImportFromFileRequest);
}

void MainWindow::InitComponents()
{
  m_anyvalue_editor = new sup::gui::AnyValueEditor;
  setCentralWidget(m_anyvalue_editor);
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
