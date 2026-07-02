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

#include "automation_client.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/objects/remote_job_handler.h>
#include <oac_tree_gui/jobsystem/user_context.h>
#include <oac_tree_gui/model/standard_job_items.h>

#include <sup/oac-tree-server/epics_utils.h>
#include <sup/oac-tree-server/exceptions.h>
#include <sup/oac-tree-server/i_client_job_manager.h>
#include <sup/oac-tree-server/ws_utils.h>
#include <sup/oac-tree/job_info.h>

#include <chrono>

namespace oac_tree_gui
{

namespace
{
const std::uint32_t kConnectionTimeout{1};
}

AutomationClient::AutomationClient(const AutomationServerInfo& server_info)
    : m_server_info(server_info), m_client_manager(CreateConnectionManager(m_server_info))
{
  if (auto is_connected =
          m_client_manager->WaitForConnection(std::chrono::seconds(kConnectionTimeout));
      !is_connected)
  {
    throw RuntimeException("Connection to the server ["
                           + ::oac_tree_gui::GetAutomationServerName(GetServerInfo()) + "] has failed within "
                           + std::to_string(kConnectionTimeout) + " sec timeout");
  }
}

AutomationClient::~AutomationClient() = default;

AutomationServerInfo AutomationClient::GetServerInfo() const
{
  return m_server_info;
}

bool AutomationClient::IsConnected() const
{
  return sup::oac_tree_server::IsConnected(*m_client_manager);
}

std::size_t AutomationClient::GetJobCount() const
{
  return m_client_manager->GetNumberOfJobs();
}

std::string AutomationClient::GetProcedureName(std::uint32_t job_index) const
{
  return m_client_manager->GetJobInfo(job_index).GetProcedureName();
}

std::unique_ptr<AbstractJobHandler> AutomationClient::CreateJobHandler(
    RemoteJobItem* job_item, const UserContext& user_context)
{
  auto job_index = job_item->GetRemoteJobIndex();
  return std::make_unique<RemoteJobHandler>(job_item, *m_client_manager, job_index, user_context);
}

std::unique_ptr<sup::oac_tree_server::IClientJobManager> CreateConnectionManager(
    const AutomationServerInfo& connection_info)
{
  if (std::holds_alternative<EPICSServerInfo>(connection_info))
  {
    return sup::oac_tree_server::utils::CreateEPICSClientProtocolManager(
        std::get<EPICSServerInfo>(connection_info).server_name);
  }

  if (std::holds_alternative<WebSocketsServerInfo>(connection_info))
  {
    return sup::oac_tree_server::utils::CreateWSClientJobManager(
        std::get<WebSocketsServerInfo>(connection_info).server_address,
        std::get<WebSocketsServerInfo>(connection_info).server_port);
  }

  throw RuntimeException("Unknown server type");
}

}  // namespace oac_tree_gui
