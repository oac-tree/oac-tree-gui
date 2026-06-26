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

#include "remote_connection_dialog.h"

#include "server_settings_widget.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/i_automation_client.h>
#include <oac_tree_gui/jobsystem/remote_connection_info.h>
#include <oac_tree_gui/jobsystem/remote_connection_service.h>

#include <sup/gui/widgets/dialog_helper.h>
#include <sup/gui/widgets/settings_callbacks.h>

#include <mvvm/style/mvvm_style_helper.h>

#include <QApplication>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QKeyEvent>
#include <QListView>
#include <QSettings>
#include <QStandardItemModel>

namespace oac_tree_gui
{

namespace
{

const QString kGroupName = "RemoteConnectionDialog";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";

/**
 * @brief Creates item for job list model.
 */
std::unique_ptr<QStandardItem> CreateItem(const std::string& name)
{
  auto result = std::make_unique<QStandardItem>(QString::fromStdString(name));
  result->setEditable(false);
  result->setToolTip(QString::fromStdString(name));
  return result;
}

}  // namespace

RemoteConnectionDialog::RemoteConnectionDialog(IRemoteConnectionService* connection_service,
                                               QWidget* parent_widget)
    : QDialog(parent_widget)
    , m_server_settings_widget(new ServerSettingsWidget(this))
    , m_job_list_view(new QListView)
    , m_job_info_model(new QStandardItemModel(this))
    , m_connection_service(connection_service)
{
  if (m_connection_service == nullptr)
  {
    throw RuntimeException("Uninitialized server");
  }

  setWindowTitle("Connect to server");

  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_server_settings_widget);
  layout->addWidget(m_job_list_view);
  layout->addLayout(
      sup::gui::CreateButtonLayout(this, "Attach to selected jobs", "Cancel").release());

  m_job_list_view->setAlternatingRowColors(true);
  m_job_list_view->setModel(m_job_info_model);
  m_job_list_view->setSelectionMode(QAbstractItemView::MultiSelection);

  connect(m_server_settings_widget, &ServerSettingsWidget::connectRequest, this,
          &RemoteConnectionDialog::OnConnectRequest);

  ReadSettings();
}

RemoteConnectionDialog::~RemoteConnectionDialog()
{
  WriteSettings();
}

RemoteConnectionInfo RemoteConnectionDialog::GetResult() const
{
  // collecting selected jobs, sorting accoring to the job index
  std::set<std::size_t> indexes;
  for (auto index : m_job_list_view->selectionModel()->selectedIndexes())
  {
    indexes.insert(static_cast<std::size_t>(index.row()));
  }

  return {m_current_server_info, indexes};
}

void RemoteConnectionDialog::keyPressEvent(QKeyEvent* event)
{
  // Prevent dialog from closing on press-enter-key event. This seems to be the only reliable way
  // to avoid forwarding such events from QLineEdit to "OK" QPushButton.

  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    return;
  }

  QDialog::keyPressEvent(event);
}

void RemoteConnectionDialog::OnConnectRequest()
{
  setCursor(Qt::WaitCursor);
  QApplication::processEvents();

  // TODO establish connection in a thread with possibility to cancel hanging
  const auto server_info = m_server_settings_widget->GetServerInfo();
  const auto server_name = GetServerName(server_info);
  if (auto is_connected = m_connection_service->Connect(server_name); is_connected)
  {
    PopulateJobInfoModel(server_name);
    m_current_server_info = server_info;
  }

  unsetCursor();
  QApplication::processEvents();
}

void RemoteConnectionDialog::ReadSettings()
{
  const QSettings settings;
  const auto default_size = QSize(mvvm::style::UnitSize(30), mvvm::style::UnitSize(30));
  resize(settings.value(kWindowSizeSettingName, default_size).toSize());

  m_server_settings_widget->ReadSettings(sup::gui::GetSettingsReadFunc());
}

void RemoteConnectionDialog::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWindowSizeSettingName, size());

  m_server_settings_widget->WriteSettings(sup::gui::GetSettingsWriteFunc());
}

void RemoteConnectionDialog::PopulateJobInfoModel(const std::string& server_name)
{
  m_job_info_model->clear();

  auto parent_item = m_job_info_model->invisibleRootItem();
  auto& client = m_connection_service->GetAutomationClient(server_name);
  for (std::size_t job_index = 0; job_index < client.GetJobCount(); ++job_index)
  {
    auto procedure_name = client.GetProcedureName(job_index);
    parent_item->appendRow(CreateItem(procedure_name).release());
  }
}

std::optional<RemoteConnectionInfo> GetDialogRemoteConnectionInfo(
    IRemoteConnectionService& connection_service, QWidget* parent)
{
  RemoteConnectionDialog dialog(&connection_service, parent);

  if (dialog.exec() == QDialog::Accepted)
  {
    return dialog.GetResult();
  }
  return {};
}

}  // namespace oac_tree_gui
