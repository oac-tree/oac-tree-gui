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

#include "remote_connection_service.h"

#include "i_automation_client.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/abstract_job_handler.h>
#include <sequencergui/model/standard_job_items.h>

#include <algorithm>

namespace sequencergui
{

RemoteConnectionService::RemoteConnectionService(const create_client_t& create_connection)
    : m_create_client(create_connection)
{
  if (!create_connection)
  {
    throw RuntimeException("Uninitialised function to create connections");
  }
}

bool RemoteConnectionService::Connect(const std::string& server_name)
{
  if (HasClient(server_name))
  {
    return true;
  }

  std::unique_ptr<IAutomationClient> client;
  try
  {
    client = m_create_client(server_name);
  }
  catch (const RuntimeException&)
  {
    return false;
  }

  m_clients.push_back(std::move(client));
  return true;
}

void RemoteConnectionService::Disconnect(const std::string& server_name)
{
  auto on_element = [&server_name](auto& element)
  { return element->GetServerName() == server_name; };
  m_clients.erase(std::remove_if(m_clients.begin(), m_clients.end(), on_element), m_clients.end());
}

bool RemoteConnectionService::IsConnected(const std::string& server_name) const
{
  // how to check if connection is alive
  return HasClient(server_name);
}

bool RemoteConnectionService::HasClient(const std::string& server_name) const
{
  auto on_element = [&server_name](auto& element)
  { return element->GetServerName() == server_name; };
  auto pos = std::find_if(m_clients.begin(), m_clients.end(), on_element);
  return pos != m_clients.end();
}

std::vector<std::string> RemoteConnectionService::GetServerNames() const
{
  std::vector<std::string> result;
  auto on_element = [](const auto& element) { return element->GetServerName(); };
  std::transform(m_clients.begin(), m_clients.end(), std::back_inserter(result), on_element);
  return result;
}

IAutomationClient& RemoteConnectionService::GetAutomationClient(const std::string& server_name)
{
  auto on_element = [&server_name](auto& element)
  { return element->GetServerName() == server_name; };
  auto pos = std::find_if(m_clients.begin(), m_clients.end(), on_element);

  if (pos == m_clients.end())
  {
    throw RuntimeException("No client for server [" + server_name + "]");
  }

  IAutomationClient* client = pos->get();
  return *client;
}

std::unique_ptr<AbstractJobHandler> RemoteConnectionService::CreateJobHandler(
    RemoteJobItem* job_item, const UserContext& user_context)
{
  if (!job_item)
  {
    throw RuntimeException("JobItem is not initialized");
  }
  auto server_name = job_item->GetServerName();

  if (!Connect(server_name))
  {
    throw RuntimeException("Can't connect to server");
  }

  return GetAutomationClient(server_name).CreateJobHandler(job_item, user_context);
}

}  // namespace sequencergui
