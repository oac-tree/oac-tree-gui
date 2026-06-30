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

#include "remote_connection_service.h"

#include "i_automation_client.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/objects/abstract_job_handler.h>
#include <oac_tree_gui/model/standard_job_items.h>

#include <sup/gui/core/message_event.h>

#include <algorithm>

namespace oac_tree_gui
{

RemoteConnectionService::RemoteConnectionService(const AutomationClientFunc& create_connection,
                                                 const message_func_t& message_func)
    : m_create_client(create_connection), m_message_func(message_func)
{
  if (!m_create_client)
  {
    throw RuntimeException("Uninitialised function to create connections");
  }
}

bool RemoteConnectionService::Connect(const AutomationServerInfo& server_info)
{
  if (HasClient(server_info))
  {
    return true;
  }

  std::unique_ptr<IAutomationClient> client;
  try
  {
    client = m_create_client(server_info);
  }
  catch (const RuntimeException& ex)
  {
    if (m_message_func)
    {
      sup::gui::MessageEvent message;
      message.title = "Connection failed";
      message.text = "Connection failed";
      message.informative = "Exception was thrown";
      message.detailed = ex.what();
      m_message_func(message);
    }

    return false;
  }

  m_clients.push_back(std::move(client));
  return true;
}

void RemoteConnectionService::Disconnect(const AutomationServerInfo& server_info)
{
  auto on_element = [&server_info](auto& element)
  { return element->GetServerInfo() == server_info; };
  (void)m_clients.erase(std::remove_if(m_clients.begin(), m_clients.end(), on_element),
                        m_clients.end());
}

bool RemoteConnectionService::IsConnected(const AutomationServerInfo& server_info) const
{
  // how to check if connection is alive
  return HasClient(server_info);
}

bool RemoteConnectionService::HasClient(const AutomationServerInfo& server_info) const
{
  auto on_element = [&server_info](auto& element)
  { return element->GetServerInfo() == server_info; };
  auto pos = std::find_if(m_clients.begin(), m_clients.end(), on_element);
  return pos != m_clients.end();
}

std::vector<AutomationServerInfo> RemoteConnectionService::GetServerInfos() const
{
  std::vector<AutomationServerInfo> result;
  auto on_element = [](const auto& element) { return element->GetServerInfo(); };
  (void)std::transform(m_clients.begin(), m_clients.end(), std::back_inserter(result), on_element);
  return result;
}

IAutomationClient& RemoteConnectionService::GetAutomationClient(
    const AutomationServerInfo& server_info)
{
  auto on_element = [&server_info](auto& element)
  { return element->GetServerInfo() == server_info; };
  auto pos = std::find_if(m_clients.begin(), m_clients.end(), on_element);

  if (pos == m_clients.end())
  {
    throw RuntimeException("No client for server [" + GetAutomationServerName(server_info) + "]");
  }

  IAutomationClient* client = pos->get();
  return *client;
}

std::unique_ptr<AbstractJobHandler> RemoteConnectionService::CreateJobHandler(
    RemoteJobItem* job_item, const UserContext& user_context)
{
  if (job_item == nullptr)
  {
    throw RuntimeException("JobItem is not initialized");
  }
  const auto server_info = job_item->GetAutomationServerInfo();

  if (!Connect(server_info))
  {
    throw RuntimeException("Can't connect to server");
  }

  return GetAutomationClient(server_info).CreateJobHandler(job_item, user_context);
}

}  // namespace oac_tree_gui
