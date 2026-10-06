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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_REMOTE_CONNECTION_DIALOG_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_REMOTE_CONNECTION_DIALOG_H_

#include <oac_tree_gui/model/remote_connection_info.h>

#include <QDialog>
#include <memory>
#include <optional>

class QTreeView;
class QHBoxLayout;

namespace sup::gui
{
class ISettingsManager;
}  // namespace sup::gui

namespace oac_tree_gui
{

class IRemoteConnectionService;
class JobInfoViewModel;
struct RemoteConnectionInfo;
class ServerSettingsWidget;

/**
 * @brief The RemoteConnectionDialog class is a modal dialog to connect with remote server and
 * import jobs.
 */
class RemoteConnectionDialog : public QDialog
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * Dialog geometry and server settings are restored on construction and stored when the dialog
   * is closed. The settings manager must outlive the dialog.
   */
  RemoteConnectionDialog(IRemoteConnectionService* connection_service,
                         sup::gui::ISettingsManager& settings_manager,
                         QWidget* parent_widget = nullptr);
  ~RemoteConnectionDialog() override;

  RemoteConnectionDialog(const RemoteConnectionDialog&) = delete;
  RemoteConnectionDialog& operator=(const RemoteConnectionDialog&) = delete;
  RemoteConnectionDialog(RemoteConnectionDialog&&) = delete;
  RemoteConnectionDialog& operator=(RemoteConnectionDialog&&) = delete;

  /**
   * @brief Returns users choice with the server name to connect, and the list of remote jobs.
   */
  RemoteConnectionInfo GetResult() const;

  void done(int result) override;

protected:
  void keyPressEvent(QKeyEvent* event) override;

private:
  /**
   * @brief Connect to the server as specified in server name field and show existing jobs.
   */
  void OnConnectRequest();

  /**
   * @brief Creates layout with server name field and connect button.
   */
  std::unique_ptr<QHBoxLayout> CreateConnectLayout();

  /**
   * @brief Restores dialog geometry and server settings.
   */
  void ReadSettings();

  /**
   * @brief Populates the model with information about remote jobs.
   */
  void PopulateJobInfoModel(const AutomationServerInfo& server_info);

  ServerSettingsWidget* m_server_settings_widget{nullptr};
  QTreeView* m_job_list_view{nullptr};
  JobInfoViewModel* m_job_info_model{nullptr};

  IRemoteConnectionService* m_connection_service{nullptr};
  sup::gui::ISettingsManager& m_settings_manager;
  AutomationServerInfo m_current_server_info;
};

/**
 * @brief Summons remote connection dialog and returns remote job import information.
 *
 * The optional result can be empty, if dialog was canceled by the user.
 *
 * @param connection_service The main service to use fo connection.
 * @param settings_manager The manager to store dialog settings.
 * @param parent The parent widget.
 * @return Optional result of the user choice.
 */
std::optional<RemoteConnectionInfo> GetDialogRemoteConnectionInfo(
    IRemoteConnectionService& connection_service, sup::gui::ISettingsManager& settings_manager,
    QWidget* parent = nullptr);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_REMOTE_CONNECTION_DIALOG_H_
