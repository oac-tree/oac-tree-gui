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
#include <sup/oac-tree-server/ws_utils.h>
#include <sup/oac-tree/job_info.h>

namespace oac_tree_gui
{

AutomationClient::AutomationClient(const std::string& server_name)
    : m_server_name(server_name)
    , m_client_manager(CreateConnectionManager(GetAutomationServerInfo(server_name)))
{
  try
  {
    // current way to check if connection was established
    [[maybe_unused]] auto job_count = m_client_manager->GetNumberOfJobs();
  }
  catch (const sup::oac_tree_server::InvalidOperationException& ex)
  {
    throw RuntimeException("Connection to the server [" + GetServerName()
                           + "] has failed with the message [" + ex.what() + "]");
  }
}  // namespace oac_tree_gui

AutomationClient::~AutomationClient() = default;

std::string AutomationClient::GetServerName() const
{
  return m_server_name;
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
