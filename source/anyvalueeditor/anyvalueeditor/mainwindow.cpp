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

#include "anyvalueeditor/mainwindow.h"

#include <QCoreApplication>
#include <QSettings>

#include "AnyValue.h"

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
  InitComponents();
  setCentralWidget(new QWidget);

  sup::dto::AnyValue two_scalars = {{
    {"signed", {sup::dto::SignedInteger8, 1}},
    {"unsigned", {sup::dto::UnsignedInteger8, 12}}
  }};

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
}

void MainWindow::InitComponents() {}

void MainWindow::WriteSettings()
{
  QSettings settings;
  settings.beginGroup(main_window_group);
  settings.setValue(size_key, size());
  settings.setValue(pos_key, pos());
  settings.endGroup();
}

}  // namespace anyvalueeditor
