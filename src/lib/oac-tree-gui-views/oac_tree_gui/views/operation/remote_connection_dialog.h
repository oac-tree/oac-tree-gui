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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_REMOTE_CONNECTION_DIALOG_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_REMOTE_CONNECTION_DIALOG_H_

#include <QDialog>
#include <memory>
#include <optional>

class QListView;
class QLineEdit;
class QPushButton;
class QStandardItemModel;
class QHBoxLayout;

namespace oac_tree_gui
{

class IRemoteConnectionService;
struct RemoteConnectionInfo;

/**
 * @brief The RemoteConnectionDialog class is a modal dialog to connect with remote server and
 * import jobs.
 */
class RemoteConnectionDialog : public QDialog
{
  Q_OBJECT

public:
  explicit RemoteConnectionDialog(IRemoteConnectionService* connection_service,
                                  QWidget* parent_widget = nullptr);
  ~RemoteConnectionDialog() override;

  /**
   * @brief Returns users choice with the server name to connect, and the list of remote jobs.
   */
  RemoteConnectionInfo GetResult() const;

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
   * @brief Loads dialog persistence widget settings from disk.
   */
  void ReadSettings();

  /**
   * @brief Writes persistence widget settings on disk.
   */
  void WriteSettings();

  /**
   * @brief Populates the model with information about remote jobs.
   */
  void PopulateJobInfoModel(const std::string& server_name);

  QLineEdit* m_server_name_line_edit{nullptr};
  QPushButton* m_connect_button{nullptr};
  QListView* m_job_list_view{nullptr};
  QStandardItemModel* m_job_info_model{nullptr};

  IRemoteConnectionService* m_connection_service{nullptr};
  std::string m_current_server_name;
};

/**
 * @brief Summons remote connection dialog and returns remote job import information.
 *
 * The optional result can be empty, if dialog was canceled by the user.
 *
 * @param connection_service The main service to use fo connection.
 * @return Optional result of the user choice.
 */
std::optional<RemoteConnectionInfo> GetDialogRemoteConnectionInfo(
    IRemoteConnectionService& connection_service, QWidget* parent = nullptr);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_REMOTE_CONNECTION_DIALOG_H_
