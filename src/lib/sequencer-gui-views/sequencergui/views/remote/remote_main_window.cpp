/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "remote_main_window.h"

#include "remote_monitor_view.h"

#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/app/application_helper.h>

#include <QCloseEvent>
#include <QCoreApplication>
#include <QMenuBar>
#include <QSettings>

namespace
{
const QString kGroupName = "RemoteMainWindow";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";
const QString kWindowPosSettingName = kGroupName + "/" + "pos";

}  // namespace

namespace sequencergui
{

RemoteMainWindow::RemoteMainWindow()
{
  InitApplication();
}

RemoteMainWindow::~RemoteMainWindow() = default;

void RemoteMainWindow::closeEvent(QCloseEvent* event)
{
  if (CanCloseApplication())
  {
    QMainWindow::closeEvent(event);
    return;
  }

  event->ignore();
}

void RemoteMainWindow::InitApplication()
{
  ReadSettings();

  sup::gui::AppRegisterMenuBar(menuBar(),
                               {sup::gui::constants::kFileMenu, sup::gui::constants::kViewMenu,
                                sup::gui::constants::kHelpMenu});

  m_monitor_view = new RemoteMonitorView;

  setCentralWidget(m_monitor_view);
}

void RemoteMainWindow::ReadSettings()
{
  const QSettings settings;
  resize(settings.value(kWindowSizeSettingName, QSize(800, 600)).toSize());
  move(settings.value(kWindowPosSettingName, QPoint(200, 200)).toPoint());
}

void RemoteMainWindow::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWindowSizeSettingName, size());
  settings.setValue(kWindowPosSettingName, pos());
}

bool RemoteMainWindow::CanCloseApplication()
{
  WriteSettings();
  return true;
}

void RemoteMainWindow::OnRestartRequest(sup::gui::AppExitCode exit_code)
{
  if (CanCloseApplication())
  {
    sup::gui::ShutdownApplication();
    QCoreApplication::exit(exit_code);
  }
}

}  // namespace sequencergui
