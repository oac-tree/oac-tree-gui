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

#include <oac_tree_gui/model/component_types.h>

#include <sup/gui/widgets/settings_callbacks.h>

#include <QWidget>

class QComboBox;
class QLineEdit;
class QPushButton;

namespace oac_tree_gui
{

/**
 * @brief The ServerSettingsWidget class allows to define the type of remote server.
 *
 * Contains server type selector (WebSockets, EPICS), server name, port, and connect button.
 */
class ServerSettingsWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ServerSettingsWidget(QWidget* parent_widget = nullptr);

  ~ServerSettingsWidget() override;

  ServerSettingsWidget(const ServerSettingsWidget&) = delete;
  ServerSettingsWidget& operator=(const ServerSettingsWidget&) = delete;
  ServerSettingsWidget(ServerSettingsWidget&&) = delete;
  ServerSettingsWidget& operator=(ServerSettingsWidget&&) = delete;

  AutomationServerType GetServerType() const;

  void SetServerType(AutomationServerType server_type);

signals:
  void connectRequest();

private:
  void OnServerTypeComboChanged();

  /**
   * @brief Loads dialog persistence widget settings from disk.
   */
  void ReadSettings(
      const sup::gui::read_variant_func_t& read_func = sup::gui::GetSettingsReadFunc());

  /**
   * @brief Writes persistence widget settings on disk.
   */
  void WriteSettings(
      const sup::gui::write_variant_func_t& write_func = sup::gui::GetSettingsWriteFunc());

  QComboBox* m_server_type_combo{nullptr};
  QLineEdit* m_server_name_line_edit{nullptr};
  QLineEdit* m_server_address_line_edit{nullptr};
  QLineEdit* m_port_line_edit{nullptr};
  QPushButton* m_connect_button{nullptr};
};

}  // namespace oac_tree_gui

#endif
