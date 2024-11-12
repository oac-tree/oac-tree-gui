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

#include "remote_connection_dialog.h"

#include <sup/gui/widgets/dialog_helper.h>

#include <mvvm/widgets/widget_utils.h>

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QSettings>
#include <QStandardItemModel>
#include <QVBoxLayout>

namespace sequencergui
{

namespace
{

const QString kGroupName = "RemoteConnectionDialog";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";

}  // namespace

RemoteConnectionDialog::RemoteConnectionDialog(QWidget *parent)
    : QDialog(parent)
    , m_server_name_line_edit(new QLineEdit)
    , m_connect_button(new QPushButton("Connect"))
    , m_job_list_view(new QListView)
    , m_job_model(new QStandardItemModel(this))
{
  setWindowTitle("Connect to server");

  auto layout = new QVBoxLayout(this);
  layout->addLayout(CreateConnectLayout().release());
  layout->addWidget(m_job_list_view);
  layout->addLayout(sup::gui::CreateButtonLayout(this).release());

  m_server_name_line_edit->setClearButtonEnabled(true);
  m_server_name_line_edit->setPlaceholderText("Server name");
  m_connect_button->setFixedWidth(mvvm::utils::UnitSize(12));

  ReadSettings();
}

RemoteConnectionDialog::~RemoteConnectionDialog()
{
  WriteSettings();
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

}  // namespace sequencergui
