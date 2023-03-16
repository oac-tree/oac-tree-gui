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
const QString kMainWindowGroupName("MainWindow");

QString GetWindowSizeSettingName()
{
  return kMainWindowGroupName + "/" + "size";
}

QString GetWindowPosSettingName()
{
  return kMainWindowGroupName + "/" + "position";
}

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
  ReadSettings();
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

void MainWindow::ReadSettings()
{
  const QSettings settings;
  resize(settings.value(GetWindowSizeSettingName(), QSize(800, 600)).toSize());
  move(settings.value(GetWindowPosSettingName(), QPoint(200, 200)).toPoint());
}

void MainWindow::WriteSettings()
{
  QSettings settings;
  settings.setValue(GetWindowSizeSettingName(), size());
  settings.setValue(GetWindowPosSettingName(), pos());
}

}  // namespace anyvalueeditor
