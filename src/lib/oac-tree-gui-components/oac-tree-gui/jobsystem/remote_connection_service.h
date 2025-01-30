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

#ifndef SEQUENCERGUI_JOBSYSTEM_REMOTE_CONNECTION_SERVICE_H_
#define SEQUENCERGUI_JOBSYSTEM_REMOTE_CONNECTION_SERVICE_H_

#include <oac-tree-gui/jobsystem/i_remote_connection_service.h>

#include <functional>

namespace oac_tree_gui
{

/**
 * @brief The RemoteConnectionService is a defaut implementation of service to connect with remote
 * automation jobs.
 */
class RemoteConnectionService : public IRemoteConnectionService
{
public:
  using create_client_t = std::function<std::unique_ptr<IAutomationClient>(const std::string&)>;
  explicit RemoteConnectionService(const create_client_t& create_connection);

  bool Connect(const std::string& server_name) override;

  void Disconnect(const std::string& server_name) override;

  bool IsConnected(const std::string& server_name) const override;

  bool HasClient(const std::string& server_name) const override;

  std::vector<std::string> GetServerNames() const override;

  IAutomationClient& GetAutomationClient(const std::string& server_name) override;

  std::unique_ptr<AbstractJobHandler> CreateJobHandler(RemoteJobItem* job_item,
                                                       const UserContext& user_context) override;

private:
  //!< collection of remote clients, one client per server name
  std::vector<std::unique_ptr<IAutomationClient>> m_clients;

  create_client_t m_create_client;
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_JOBSYSTEM_REMOTE_CONNECTION_SERVICE_H_
