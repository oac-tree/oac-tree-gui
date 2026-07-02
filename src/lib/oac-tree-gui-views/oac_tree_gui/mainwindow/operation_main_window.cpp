/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "operation_main_window.h"

#include "main_window_helper.h"
#include "operation_main_window_actions.h"

#include <oac_tree_gui/mainwindow/sequencer_main_window_context.h>
#include <oac_tree_gui/model/application_models.h>
#include <oac_tree_gui/views/operation/operation_monitor_view.h>

#include <mvvm/widgets/app_restarter.h>

#include <QCloseEvent>
#include <QSettings>

namespace
{
const QString kGroupName = "OperationMainWindow";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";
const QString kWindowPosSettingName = kGroupName + "/" + "pos";

}  // namespace

namespace oac_tree_gui
{

OperationMainWindow::OperationMainWindow(SequencerMainWindowContext& context)
    : m_context(context), m_models(std::make_unique<ApplicationModels>())
{
  m_models->CreateEmpty();

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

void OperationMainWindow::InitApplication()
{
  ReadSettings();

  m_action_manager = new OperationMainWindowActions(m_context.GetCommandService(), this);
  connect(m_action_manager, &OperationMainWindowActions::ExitRequest, this,
          &OperationMainWindow::OnExitRequest);

  m_operation_view = new OperationMonitorView(m_context.GetCommandService(),
                                              OperationPresentationMode::kOperationMode, this);
  m_operation_view->SetModels(m_models.get());

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
    switch (ShouldStopRunningJobs())
    {
    case RunningJobsCloseAction::kStopAllJobs:
      m_operation_view->StopJobs(StopScope::kAll);
      break;
    case RunningJobsCloseAction::kStopLocalJobs:
      m_operation_view->StopJobs(StopScope::kLocalOnly);
      break;
    case RunningJobsCloseAction::kNoAction:
      return false;
    }
  }

  WriteSettings();
  return true;
}

void OperationMainWindow::OnExitRequest(mvvm::AppExitRequest exit_request)
{
  if (CanCloseApplication())
  {
    mvvm::AppRestarter::StoreExitRequest(exit_request);
    close();
  }
}

}  // namespace oac_tree_gui
