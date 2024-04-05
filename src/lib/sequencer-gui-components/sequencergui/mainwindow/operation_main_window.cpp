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

#include "operation_main_window.h"

#include "main_window_helper.h"
#include "operation_main_window_actions.h"

#include <sequencergui/model/application_models.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/operation/operation_monitor_view.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/application_helper.h>

#include <QCloseEvent>
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

OperationMainWindow::OperationMainWindow() : m_models(std::make_unique<ApplicationModels>())
{
  PopulateModel();
  InitApplication();

  connect(m_action_manager, &OperationMainWindowActions::ImportJobRequest, this,
          [this]() { m_operation_view->OnImportJobRequest(); });
}

bool OperationMainWindow::ImportProcedure(const QString& file_name)
{
  return file_name.isEmpty() ? false : m_operation_view->OnImportJobRequest(file_name);
}

OperationMainWindow::~OperationMainWindow() = default;

void OperationMainWindow::closeEvent(QCloseEvent* event)
{
  if (CanCloseApplication())
  {
    QMainWindow::closeEvent(event);
    return;
  }

  event->ignore();
}

void OperationMainWindow::PopulateModel() {}

void OperationMainWindow::InitApplication()
{
  ReadSettings();

  sup::gui::AppRegisterMainMenuBar(menuBar(),
                                   {sup::gui::constants::kFileMenu, sup::gui::constants::kViewMenu,
                                    sup::gui::constants::kHelpMenu});

  m_action_manager = new OperationMainWindowActions(m_models->GetSequencerModel(), this);
  connect(m_action_manager, &OperationMainWindowActions::RestartApplicationRequest, this,
          &OperationMainWindow::OnRestartRequest);

  m_operation_view = new OperationMonitorView(OperationMonitorView::kOperationMode, this);
  m_operation_view->SetApplicationModels(m_models.get());

  setCentralWidget(m_operation_view);
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

bool OperationMainWindow::CanCloseApplication()
{
  if (m_operation_view->HasRunningJobs())
  {
    if (ShouldStopRunningJobs())
    {
      m_operation_view->StopAllJobs();
    }
    else
    {
      return false;
    }
  }

  WriteSettings();
  return true;
}

void OperationMainWindow::OnRestartRequest(sup::gui::AppExitCode exit_code)
{
  if (CanCloseApplication())
  {
    sup::gui::ShutdownApplication();
    QCoreApplication::exit(exit_code);
  }
}

}  // namespace sequencergui
