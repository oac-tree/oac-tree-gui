/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "remote_connection_dialog.h"

#include <oac-tree-gui/core/exceptions.h>
#include <oac-tree-gui/jobsystem/i_automation_client.h>
#include <oac-tree-gui/jobsystem/remote_connection_info.h>
#include <oac-tree-gui/jobsystem/remote_connection_service.h>
#include <sup/gui/widgets/dialog_helper.h>

#include <mvvm/widgets/widget_utils.h>

#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QKeyEvent>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QSettings>
#include <QStandardItemModel>
#include <QVBoxLayout>

namespace oac_tree_gui
{

namespace
{

const QString kGroupName = "RemoteConnectionDialog";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";

/**
 * @brief Creates item for job list model.
 */
std::unique_ptr<QStandardItem> CreateItem(const std::string &name)
{
  auto result = std::make_unique<QStandardItem>(QString::fromStdString(name));
  result->setEditable(false);
  result->setToolTip(QString::fromStdString(name));
  return result;
}

}  // namespace

RemoteConnectionDialog::RemoteConnectionDialog(RemoteConnectionService *connection_service,
                                               QWidget *parent_widget)
    : QDialog(parent_widget)
    , m_server_name_line_edit(new QLineEdit)
    , m_connect_button(new QPushButton("Connect"))
    , m_job_list_view(new QListView)
    , m_job_info_model(new QStandardItemModel(this))
    , m_connection_service(connection_service)
{
  if (!m_connection_service)
  {
    throw RuntimeException("Uninitialized server");
  }

  setWindowTitle("Connect to server");

  auto layout = new QVBoxLayout(this);
  layout->addLayout(CreateConnectLayout().release());
  layout->addWidget(m_job_list_view);
  layout->addLayout(
      sup::gui::CreateButtonLayout(this, "Attach to selected jobs", "Cancel").release());

  m_server_name_line_edit->setClearButtonEnabled(true);
  m_server_name_line_edit->setPlaceholderText("Server name");
  m_connect_button->setFixedWidth(mvvm::utils::UnitSize(12));

  m_job_list_view->setAlternatingRowColors(true);
  m_job_list_view->setModel(m_job_info_model);
  m_job_list_view->setSelectionMode(QAbstractItemView::MultiSelection);

  connect(m_connect_button, &QPushButton::clicked, this, &RemoteConnectionDialog::OnConnectRequest);

  connect(m_server_name_line_edit, &QLineEdit::returnPressed, this,
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

  return {m_current_server_name, indexes};
}

void RemoteConnectionDialog::keyPressEvent(QKeyEvent *event)
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
  const auto server_name = m_server_name_line_edit->text().toStdString();
  if (auto is_connected = m_connection_service->Connect(server_name); is_connected)
  {
    PopulateJobInfoModel(server_name);
    m_current_server_name = server_name;
  }

  unsetCursor();
  QApplication::processEvents();
}

std::unique_ptr<QHBoxLayout> RemoteConnectionDialog::CreateConnectLayout()
{
  auto result = std::make_unique<QHBoxLayout>();
  result->addWidget(m_server_name_line_edit);
  result->addWidget(m_connect_button);
  return result;
}

void RemoteConnectionDialog::ReadSettings()
{
  const QSettings settings;
  const auto default_size = QSize(mvvm::utils::UnitSize(30), mvvm::utils::UnitSize(30));
  resize(settings.value(kWindowSizeSettingName, default_size).toSize());
}

void RemoteConnectionDialog::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWindowSizeSettingName, size());
}

void RemoteConnectionDialog::PopulateJobInfoModel(const std::string &server_name)
{
  m_job_info_model->clear();

  auto parent_item = m_job_info_model->invisibleRootItem();
  auto &client = m_connection_service->GetAutomationClient(server_name);
  for (std::size_t job_index = 0; job_index < client.GetJobCount(); ++job_index)
  {
    auto procedure_name = client.GetProcedureName(job_index);
    parent_item->appendRow(CreateItem(procedure_name).release());
  }
}

std::optional<RemoteConnectionInfo> GetDialogRemoteConnectionInfo(
    RemoteConnectionService &connection_service, QWidget *parent)
{
  RemoteConnectionDialog dialog(&connection_service, parent);

  if (dialog.exec() == QDialog::Accepted)
  {
    return dialog.GetResult();
  }
  return {};
}

}  // namespace oac_tree_gui
