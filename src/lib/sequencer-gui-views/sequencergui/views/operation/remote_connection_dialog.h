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

#ifndef SEQUENCERGUI_VIEWS_OPERATION_REMOTE_CONNECTION_DIALOG_H_
#define SEQUENCERGUI_VIEWS_OPERATION_REMOTE_CONNECTION_DIALOG_H_

#include <sequencergui/jobsystem/remote_connection_context.h>

#include <QDialog>
#include <memory>

class QListView;
class QLineEdit;
class QPushButton;
class QStandardItemModel;
class QHBoxLayout;

namespace sequencergui
{

class RemoteConnectionService;

/**
 * @brief The RemoteConnectionDialog class is a modal dialog to connect with remote server and
 * import jobs.
 */
class RemoteConnectionDialog : public QDialog
{
  Q_OBJECT

public:
  explicit RemoteConnectionDialog(RemoteConnectionService* connection_service,
                                  QWidget* parent = nullptr);
  ~RemoteConnectionDialog() override;

  /**
   * @brief Returns user choice regarding remote jobs to connect.
   */
  RemoteConnectionContext GetResult() const;

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

  QLineEdit* m_server_name_line_edit{nullptr};
  QPushButton* m_connect_button{nullptr};
  QListView* m_job_list_view{nullptr};
  QStandardItemModel* m_job_model{nullptr};

  RemoteConnectionContext m_connection_context;
  RemoteConnectionService* m_connection_service{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_OPERATION_REMOTE_CONNECTION_DIALOG_H_
