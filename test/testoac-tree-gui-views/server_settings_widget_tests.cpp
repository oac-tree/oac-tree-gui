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

#include "oac_tree_gui/views/operation/server_settings_widget.h"

#include <gtest/gtest.h>

#include <QComboBox>
#include <QLineEdit>
#include <QMap>
#include <QPushButton>
#include <QSignalSpy>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for ServerSettingsWidget class.
 */
class ServerSettingsWidgetTest : public ::testing::Test
{
protected:
  //! In-memory persistent storage emulating QSettings for ReadSettings/WriteSettings testing.
  QMap<QString, QVariant> m_storage;

  sup::gui::read_variant_func_t GetReadFunc()
  {
    return [this](const QString& key) { return m_storage.value(key); };
  }

  sup::gui::write_variant_func_t GetWriteFunc()
  {
    return [this](const QString& key, const QVariant& value) { m_storage.insert(key, value); };
  }
};

TEST_F(ServerSettingsWidgetTest, InitialState)
{
  const ServerSettingsWidget widget;

  // by default the widget is configured for a WebSockets server
  EXPECT_EQ(widget.GetServerType(), AutomationServerType::kWebSockets);

  const auto info = widget.GetServerInfo();
  ASSERT_TRUE(std::holds_alternative<WebSocketsServerInfo>(info));
  EXPECT_TRUE(std::get<WebSocketsServerInfo>(info).server_address.empty());
}

TEST_F(ServerSettingsWidgetTest, GetServerInfoForWebSockets)
{
  const ServerSettingsWidget widget;

  auto address_line_edit =
      widget.findChild<QLineEdit*>(ServerSettingsWidget::kServerAddressLineEditName);
  auto port_line_edit = widget.findChild<QLineEdit*>(ServerSettingsWidget::kPortLineEditName);
  ASSERT_NE(address_line_edit, nullptr);
  ASSERT_NE(port_line_edit, nullptr);

  address_line_edit->setText("localhost");
  port_line_edit->setText("8080");

  const auto info = widget.GetServerInfo();
  ASSERT_TRUE(std::holds_alternative<WebSocketsServerInfo>(info));
  const auto& ws_info = std::get<WebSocketsServerInfo>(info);
  EXPECT_EQ(ws_info.server_address, std::string("localhost"));
  EXPECT_EQ(ws_info.server_port, 8080);
}

TEST_F(ServerSettingsWidgetTest, GetServerInfoForEPICS)
{
  const ServerSettingsWidget widget;

  // selecting EPICS server type via the combo box
  auto combo = widget.findChild<QComboBox*>(ServerSettingsWidget::kServerTypeComboName);
  ASSERT_NE(combo, nullptr);
  combo->setCurrentIndex(combo->findData(static_cast<int>(AutomationServerType::kEPICS)));
  EXPECT_EQ(widget.GetServerType(), AutomationServerType::kEPICS);

  auto name_line_edit =
      widget.findChild<QLineEdit*>(ServerSettingsWidget::kServerNameLineEditName);
  ASSERT_NE(name_line_edit, nullptr);
  name_line_edit->setText("MyEPICSServer");

  const auto info = widget.GetServerInfo();
  ASSERT_TRUE(std::holds_alternative<EPICSServerInfo>(info));
  EXPECT_EQ(std::get<EPICSServerInfo>(info).server_name, std::string("MyEPICSServer"));
}

TEST_F(ServerSettingsWidgetTest, SetServerType)
{
  ServerSettingsWidget widget;

  auto name_line_edit =
      widget.findChild<QLineEdit*>(ServerSettingsWidget::kServerNameLineEditName);
  auto address_line_edit =
      widget.findChild<QLineEdit*>(ServerSettingsWidget::kServerAddressLineEditName);
  ASSERT_NE(name_line_edit, nullptr);
  ASSERT_NE(address_line_edit, nullptr);

  widget.SetServerType(AutomationServerType::kEPICS);
  EXPECT_EQ(widget.GetServerType(), AutomationServerType::kEPICS);
  EXPECT_TRUE(name_line_edit->isVisibleTo(&widget));
  EXPECT_FALSE(address_line_edit->isVisibleTo(&widget));

  widget.SetServerType(AutomationServerType::kWebSockets);
  EXPECT_EQ(widget.GetServerType(), AutomationServerType::kWebSockets);
  EXPECT_FALSE(name_line_edit->isVisibleTo(&widget));
  EXPECT_TRUE(address_line_edit->isVisibleTo(&widget));
}

TEST_F(ServerSettingsWidgetTest, ConnectButtonEmitsConnectRequest)
{
  const ServerSettingsWidget widget;

  const QSignalSpy spy(&widget, &ServerSettingsWidget::connectRequest);

  auto connect_button = widget.findChild<QPushButton*>(ServerSettingsWidget::kConnectButtonName);
  ASSERT_NE(connect_button, nullptr);
  connect_button->click();

  EXPECT_EQ(spy.count(), 1);
}

//! Settings written by one widget are restored in another one.
TEST_F(ServerSettingsWidgetTest, WriteAndReadSettings)
{
  {
    ServerSettingsWidget widget;
    widget.SetServerType(AutomationServerType::kEPICS);
    widget.findChild<QLineEdit*>(ServerSettingsWidget::kServerNameLineEditName)
        ->setText("MyEPICSServer");
    widget.findChild<QLineEdit*>(ServerSettingsWidget::kServerAddressLineEditName)
        ->setText("localhost");
    widget.findChild<QLineEdit*>(ServerSettingsWidget::kPortLineEditName)->setText("8080");

    widget.WriteSettings(GetWriteFunc());
  }

  // a fresh widget restores the previously stored state
  ServerSettingsWidget widget;
  widget.ReadSettings(GetReadFunc());

  EXPECT_EQ(widget.GetServerType(), AutomationServerType::kEPICS);
  EXPECT_EQ(widget.findChild<QLineEdit*>(ServerSettingsWidget::kServerNameLineEditName)->text(),
            QString("MyEPICSServer"));
  EXPECT_EQ(widget.findChild<QLineEdit*>(ServerSettingsWidget::kServerAddressLineEditName)->text(),
            QString("localhost"));
  EXPECT_EQ(widget.findChild<QLineEdit*>(ServerSettingsWidget::kPortLineEditName)->text(),
            QString("8080"));
}

}  // namespace oac_tree_gui::test
