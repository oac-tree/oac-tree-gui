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

#include "automation_manager.h"

#include "remote_job_observer.h"

#include <sup/auto-server/automation_client.h>
#include <sup/auto-server/automation_protocol_client.h>
#include <sup/auto-server/epics_anyvalue_listener.h>
#include <sup/epics/pv_access_rpc_client.h>
#include <sup/protocol/protocol_rpc_client.h>

#include <QDebug>
#include <iostream>

namespace sequencergui
{

AutomationManager::AutomationManager(const std::string &server_name)
{
  std::cout << "AutomationManager::AutomationManager " << server_name << "\n";
  // typical RPC client stack:
  auto rpc_client_config = sup::epics::GetDefaultRPCClientConfig(server_name);
  sup::epics::PvAccessRPCClient pv_access_rpc_client{rpc_client_config};
  sup::protocol::ProtocolRPCClient protocol_rpc_client{pv_access_rpc_client};

  // automation client classes:
  sup::auto_server::AutomationProtocolClient auto_protocol_client{protocol_rpc_client};

  std::cout << auto_protocol_client.GetServerPrefix() << " "
            << auto_protocol_client.GetNumberOfJobs() << std::endl;
}

AutomationManager::~AutomationManager() = default;

}  // namespace sequencergui
