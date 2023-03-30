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

#include <sequencergui/model/workspace_item.h>
#include <sequencergui/monitor/main_window_actions.h>
#include <sequencergui/monitor/monitor_model.h>
#include <sequencergui/monitor/monitor_widget.h>

#include <QCoreApplication>
#include <QFileDialog>
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

namespace sequencergui
{
MainWindow::MainWindow() : m_model(std::make_unique<MonitorModel>())
{
  PopulateModel();
  InitApplication();
}

MainWindow::~MainWindow() = default;

void MainWindow::closeEvent(QCloseEvent* event)
{
  WriteSettings();
  QMainWindow::closeEvent(event);
}

void MainWindow::PopulateModel()
{
  m_model->InsertItem<sequencergui::WorkspaceItem>();
}

void MainWindow::InitApplication()
{
  ReadSettings();

  m_monitor_widget = new MonitorWidget(m_model.get());
  setCentralWidget(m_monitor_widget);

  m_actions = new MainWindowActions(m_model.get(), this);
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

}  // namespace suppvmonitor
