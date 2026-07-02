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

#ifndef OAC_TREE_GUI_JOBSYSTEM_REMOTE_CONNECTION_SERVICE_H_
#define OAC_TREE_GUI_JOBSYSTEM_REMOTE_CONNECTION_SERVICE_H_

#include <oac_tree_gui/jobsystem/automation_client_factory.h>
#include <oac_tree_gui/jobsystem/i_remote_connection_service.h>
#include <oac_tree_gui/model/remote_connection_info.h>

#include <functional>

namespace sup::gui
{
struct MessageEvent;
}

namespace oac_tree_gui
{

/**
 * @brief The RemoteConnectionService is a defaut implementation of service to connect with remote
 * automation jobs.
 */
class RemoteConnectionService : public IRemoteConnectionService
{
public:
  using message_func_t = std::function<void(const sup::gui::MessageEvent&)>;

  explicit RemoteConnectionService(const AutomationClientFunc& create_client,
                                   const message_func_t& message_func);

  bool Connect(const AutomationServerInfo& server_info) override;

  void Disconnect(const AutomationServerInfo& server_info) override;

  bool IsConnected(const AutomationServerInfo& server_info) const override;

  bool HasClient(const AutomationServerInfo& server_info) const override;

  std::vector<AutomationServerInfo> GetServerInfos() const override;

  IAutomationClient& GetAutomationClient(const AutomationServerInfo& server_info) override;

  std::unique_ptr<AbstractJobHandler> CreateJobHandler(RemoteJobItem* job_item,
                                                       const UserContext& user_context) override;

private:
  /**
   * @brief Returns client for given server, or nullptr if no such client exists.
   */
  IAutomationClient* FindClient(const AutomationServerInfo& server_info) const;

  /**
   * @brief Removes client for given server from the list, if it exists.
   */
  void RemoveClient(const AutomationServerInfo& server_info);

  //!< collection of remote clients, one client per server
  std::vector<std::unique_ptr<IAutomationClient>> m_clients;

  AutomationClientFunc m_create_client;
  message_func_t m_message_func;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_REMOTE_CONNECTION_SERVICE_H_
