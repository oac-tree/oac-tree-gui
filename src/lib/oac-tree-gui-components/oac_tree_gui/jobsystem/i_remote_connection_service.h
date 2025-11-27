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

#ifndef OAC_TREE_GUI_JOBSYSTEM_I_REMOTE_CONNECTION_SERVICE_H_
#define OAC_TREE_GUI_JOBSYSTEM_I_REMOTE_CONNECTION_SERVICE_H_

#include <memory>
#include <string>
#include <vector>

namespace oac_tree_gui
{

class IAutomationClient;
class AbstractJobHandler;
class RemoteJobItem;
class UserContext;

/**
 * @brief The IRemoteConnectionService class is an interface to hold collection of clients connected
 * with remote automation servers.
 *
 * One client is connected with one server, there can't be two clients connected with single server.
 */
class IRemoteConnectionService
{
public:
  IRemoteConnectionService() = default;
  virtual ~IRemoteConnectionService() = default;

  IRemoteConnectionService(const IRemoteConnectionService&) = delete;
  IRemoteConnectionService& operator=(const IRemoteConnectionService&) = delete;
  IRemoteConnectionService(IRemoteConnectionService&&) = delete;
  IRemoteConnectionService& operator=(IRemoteConnectionService&&) = delete;

  /**
   * @brief Connect with the server using the given server name.
   *
   * If connection was already established, does nothing. Connection remains.
   *
   * @return True if connected.
   */
  virtual bool Connect(const std::string& server_name) = 0;

  /**
   * @brief Disconnect the server by removing underlying client.
   *
   * If connection was already established, does nothing.
   */
  virtual void Disconnect(const std::string& server_name) = 0;

  /**
   * @brief Checks if client exists and server is responding.
   */
  virtual bool IsConnected(const std::string& server_name) const = 0;

  /**
   * @brief Checks if client exists for server with the given name.
   */
  virtual bool HasClient(const std::string& server_name) const = 0;

  /**
   * @brief Returns list of already connected servers.
   */
  virtual std::vector<std::string> GetServerNames() const = 0;

  /**
   * @brief Returns the client connected with the remote server with the given name.
   */
  virtual IAutomationClient& GetAutomationClient(const std::string& server_name) = 0;

  /**
   * @brief Creates job handler for given remote job item.
   */
  virtual std::unique_ptr<AbstractJobHandler> CreateJobHandler(RemoteJobItem* job_item,
                                                               const UserContext& user_context) = 0;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_I_REMOTE_CONNECTION_SERVICE_H_
