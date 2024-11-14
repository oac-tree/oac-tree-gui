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

#include "automation_main_window.h"

#include "automation_monitor_view.h"

#include <sequencergui/model/application_models.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/mainwindow/main_window_helper.h>

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

AutomationMainWindow::AutomationMainWindow() : m_models(std::make_unique<ApplicationModels>())
{
  m_models->CreateEmpty();
  InitApplication();
}

AutomationMainWindow::~AutomationMainWindow() = default;

void AutomationMainWindow::closeEvent(QCloseEvent* event)
{
  if (CanCloseApplication())
  {
    QMainWindow::closeEvent(event);
    return;
  }

  event->ignore();
}

void AutomationMainWindow::InitApplication()
{
  ReadSettings();

  sup::gui::AppRegisterMenuBar(menuBar(),
                               {sup::gui::constants::kFileMenu, sup::gui::constants::kViewMenu,
                                sup::gui::constants::kHelpMenu});

  m_monitor_view = new AutomationMonitorView;
  m_monitor_view->SetModels(m_models->GetSequencerModel(), m_models->GetJobModel());

  setCentralWidget(m_monitor_view);
}

void AutomationMainWindow::ReadSettings()
{
  const QSettings settings;
  resize(settings.value(kWindowSizeSettingName, QSize(800, 600)).toSize());
  move(settings.value(kWindowPosSettingName, QPoint(200, 200)).toPoint());
}

void AutomationMainWindow::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWindowSizeSettingName, size());
  settings.setValue(kWindowPosSettingName, pos());
}

bool AutomationMainWindow::CanCloseApplication()
{
  WriteSettings();
  return true;
}

void AutomationMainWindow::OnRestartRequest(sup::gui::AppExitCode exit_code)
{
  if (CanCloseApplication())
  {
    sup::gui::ShutdownApplication();
    QCoreApplication::exit(exit_code);
  }
}

}  // namespace sequencergui
