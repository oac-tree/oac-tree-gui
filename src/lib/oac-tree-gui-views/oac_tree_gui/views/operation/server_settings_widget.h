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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_SERVER_SETTINGS_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_SERVER_SETTINGS_WIDGET_H_

#include <oac_tree_gui/model/remote_connection_info.h>

#include <QWidget>
#include <string>

class QComboBox;
class QLineEdit;
class QPushButton;
class QIntValidator;

namespace oac_tree_gui
{

/**
 * @brief The ServerSettingsInfo struct holds the persistent state of the server settings widget.
 */
struct ServerSettingsInfo
{
  AutomationServerType server_type{AutomationServerType::kWebSockets};
  std::string server_name;     //!< EPICS server name
  std::string server_address;  //!< WebSockets server address
  std::string server_port;     //!< WebSockets server port, as entered by the user
};

template <typename Archive>
void Visit(Archive& archive, ServerSettingsInfo& info)
{
  archive("server_type", info.server_type);
  archive("server_name", info.server_name);
  archive("server_address", info.server_address);
  archive("server_port", info.server_port);
}

/**
 * @brief The ServerSettingsWidget class allows to define the type of remote server.
 *
 * Contains server type selector (WebSockets, EPICS), server name, port, and connect button.
 */
class ServerSettingsWidget : public QWidget
{
  Q_OBJECT

public:
  // object names of internal widgets (used to look them up in tests)
  static constexpr auto kServerTypeComboName = "server_type_combo";
  static constexpr auto kServerNameLineEditName = "server_name_line_edit";
  static constexpr auto kServerAddressLineEditName = "server_address_line_edit";
  static constexpr auto kPortLineEditName = "port_line_edit";
  static constexpr auto kConnectButtonName = "connect_button";

  explicit ServerSettingsWidget(QWidget* parent_widget = nullptr);

  AutomationServerType GetServerType() const;

  void SetServerType(AutomationServerType server_type);

  AutomationServerInfo GetServerInfo() const;

  /**
   * @brief Returns the current state of the widget.
   */
  ServerSettingsInfo GetServerSettingsInfo() const;

  /**
   * @brief Sets the state of the widget.
   */
  void SetServerSettingsInfo(const ServerSettingsInfo& info);

signals:
  void connectRequest();

private:
  void OnServerTypeComboChanged();

  QComboBox* m_server_type_combo{nullptr};
  QLineEdit* m_server_name_line_edit{nullptr};
  QLineEdit* m_server_address_line_edit{nullptr};
  QLineEdit* m_port_line_edit{nullptr};
  QPushButton* m_connect_button{nullptr};
  QIntValidator* m_port_validator{nullptr};
};

}  // namespace oac_tree_gui

#endif
