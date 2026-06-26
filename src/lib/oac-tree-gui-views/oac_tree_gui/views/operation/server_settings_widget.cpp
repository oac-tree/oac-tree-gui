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

#include "server_settings_widget.h"

#include <oac_tree_gui/core/exceptions.h>

#include <mvvm/style/mvvm_style_helper.h>

#include <QComboBox>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLineEdit>
#include <QPushButton>

namespace oac_tree_gui
{

namespace
{
const std::int32_t kMinRegisteredUserPort = 1024;
const std::int32_t kMaxRegisteredUserPort = 49151;
}  // namespace

ServerSettingsWidget::ServerSettingsWidget(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_server_type_combo(new QComboBox)
    , m_server_name_line_edit(new QLineEdit)
    , m_server_address_line_edit(new QLineEdit)
    , m_port_line_edit(new QLineEdit)
    , m_connect_button(new QPushButton("Connect"))
    , m_port_validator(new QIntValidator)
{
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_server_type_combo->addItem("Web Sockets", static_cast<int>(AutomationServerType::kWebSockets));
  m_server_type_combo->addItem("EPICS", static_cast<int>(AutomationServerType::kEPICS));
  m_server_type_combo->setCurrentIndex(0);
  connect(m_server_type_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &ServerSettingsWidget::OnServerTypeComboChanged);
  layout->addWidget(m_server_type_combo);

  m_server_name_line_edit->setPlaceholderText("EPICS server name");
  m_server_name_line_edit->setClearButtonEnabled(true);
  layout->addWidget(m_server_name_line_edit);

  m_server_address_line_edit->setPlaceholderText("WebSockets server address");
  m_server_address_line_edit->setClearButtonEnabled(true);
  layout->addWidget(m_server_address_line_edit);

  m_port_line_edit->setPlaceholderText(QString::number(kMinRegisteredUserPort));
  m_port_line_edit->setFixedWidth(mvvm::style::UnitSize(4));
  m_port_validator->setRange(kMinRegisteredUserPort, kMaxRegisteredUserPort);
  m_port_line_edit->setValidator(m_port_validator);
  layout->addWidget(m_port_line_edit);

  connect(m_connect_button, &QPushButton::clicked, this, &ServerSettingsWidget::connectRequest);
  m_connect_button->setFixedWidth(mvvm::style::UnitSize(12));

  layout->addWidget(m_connect_button);

  SetServerType(AutomationServerType::kWebSockets);

  ReadSettings();
}

ServerSettingsWidget::~ServerSettingsWidget()
{
  WriteSettings();
}

AutomationServerType ServerSettingsWidget::GetServerType() const
{
  return static_cast<AutomationServerType>(m_server_type_combo->currentData().toInt());
}

void ServerSettingsWidget::SetServerType(AutomationServerType server_type)
{
  // visible for WebSockets
  m_server_address_line_edit->setVisible(server_type == AutomationServerType::kWebSockets);
  m_port_line_edit->setVisible(server_type == AutomationServerType::kWebSockets);

  // visible for EPICS
  m_server_name_line_edit->setVisible(server_type == AutomationServerType::kEPICS);
}

AutomationServerInfo ServerSettingsWidget::GetServerInfo() const
{
  const AutomationServerType server_type = GetServerType();
  if (server_type == AutomationServerType::kWebSockets)
  {
    WebSocketsServerInfo info;
    info.server_address = m_server_address_line_edit->text().toStdString();
    info.server_port = static_cast<std::uint16_t>(m_port_line_edit->text().toUInt());
    return info;
  }
  else if (server_type == AutomationServerType::kEPICS)
  {
    EPICSServerInfo info;
    info.server_name = m_server_name_line_edit->text().toStdString();
    return info;
  }
  else
  {
    throw RuntimeException("Unknown server type");
  }
}

void ServerSettingsWidget::OnServerTypeComboChanged()
{
  SetServerType(GetServerType());
}

void ServerSettingsWidget::ReadSettings(const sup::gui::read_variant_func_t& read_func) {}

void ServerSettingsWidget::WriteSettings(const sup::gui::write_variant_func_t& write_func) {}

}  // namespace oac_tree_gui
