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

#include "operation_main_window.h"

#include "operation_monitor_view.h"

#include <mvvm/model/application_model.h>

#include <QMenuBar>
#include <QSettings>

namespace
{
const QString kGroupName = "OperationMainWindow";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";
const QString kWindowPosSettingName = kGroupName + "/" + "pos";

}  // namespace

namespace sequencergui
{
OperationMainWindow::OperationMainWindow() : m_model(std::make_unique<mvvm::ApplicationModel>())
{
  PopulateModel();
  InitApplication();
}

OperationMainWindow::~OperationMainWindow() = default;

void OperationMainWindow::closeEvent(QCloseEvent* event)
{
  WriteSettings();
  QMainWindow::closeEvent(event);
}

void OperationMainWindow::PopulateModel() {}

void OperationMainWindow::InitApplication()
{
  ReadSettings();

  m_monitor_view = new OperationMonitorView(this);

  setCentralWidget(m_monitor_view);
}

void OperationMainWindow::ReadSettings()
{
  const QSettings settings;
  resize(settings.value(kWindowSizeSettingName, QSize(800, 600)).toSize());
  move(settings.value(kWindowPosSettingName, QPoint(200, 200)).toPoint());
}

void OperationMainWindow::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWindowSizeSettingName, size());
  settings.setValue(kWindowPosSettingName, pos());
}

}  // namespace sequencergui
