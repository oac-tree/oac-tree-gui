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

#include "automation_client.h"

#include <sup/auto-server/automation_client.h>
#include <sup/auto-server/automation_protocol_client.h>
#include <sup/auto-server/epics_anyvalue_listener.h>
#include <sup/epics/pv_access_rpc_client.h>
#include <sup/protocol/protocol_rpc_client.h>

#include <iostream>

namespace sequencergui
{

struct AutomationClient::AutomationManagerImpl
{
  sup::epics::PvAccessRPCClient pv_access_rpc_client;
  sup::protocol::ProtocolRPCClient protocol_rpc_client;
  sup::auto_server::AutomationProtocolClient auto_protocol_client;

  explicit AutomationManagerImpl(const std::string& server_name)
      : pv_access_rpc_client(sup::epics::GetDefaultRPCClientConfig(server_name))
      , protocol_rpc_client(pv_access_rpc_client)
      , auto_protocol_client(protocol_rpc_client)
  {
  }
};

AutomationClient::AutomationClient(const std::string& server_name)
    : p_impl(std::make_unique<AutomationManagerImpl>(server_name))
{
  std::cout << p_impl->auto_protocol_client.GetServerPrefix() << " "
            << p_impl->auto_protocol_client.GetNumberOfJobs() << "\n";
}

AutomationClient::~AutomationClient() = default;

size_t AutomationClient::GetJobCount() const
{
  return p_impl->auto_protocol_client.GetNumberOfJobs();
}

sup::auto_server::JobInfo AutomationClient::GetJobInfo(size_t job_index) const
{
  return p_impl->auto_protocol_client.GetJobInfo(job_index);
}

}  // namespace sequencergui
