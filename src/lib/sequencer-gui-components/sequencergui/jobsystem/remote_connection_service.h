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

#ifndef SEQUENCERGUI_JOBSYSTEM_REMOET_CONNECTION_SERVICE_H_
#define SEQUENCERGUI_JOBSYSTEM_REMOET_CONNECTION_SERVICE_H_

#include <functional>
#include <memory>
#include <vector>

namespace sequencergui
{

class IAutomationClient;

/**
 * @brief The RemoteConnectionService class holds collection of clients, connected with remote
 * servers.
 *
 * One client is connected with one server.
 */
class RemoteConnectionService
{
public:
  using create_client_t = std::function<std::unique_ptr<IAutomationClient>(const std::string&)>;
  explicit RemoteConnectionService(const create_client_t& create_connection);

  /**
   * @brief Connect with the server using the given server name.
   *
   * If connection was already established, does nothing.
   */
  void Connect(const std::string& server_name);

  /**
   * @brief Checks if client exists and server is responding.
   */
  bool IsConnected(const std::string& server_name) const;

  /**
   * @brief Checks if client exists for server with the given name.
   */
  bool HasClient(const std::string& server_name) const;

private:
  //!< collection of remote clients, one client per server name
  std::vector<std::unique_ptr<IAutomationClient>> m_clients;

  create_client_t m_create_connection;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_REMOET_CONNECTION_SERVICE_H_
