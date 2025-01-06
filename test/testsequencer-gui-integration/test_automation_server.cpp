/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "test_automation_server.h"

#include <sup/auto-server/automation_server.h>
#include <sup/auto-server/control_protocol_server.h>
#include <sup/auto-server/epics_config_utils.h>
#include <sup/auto-server/info_protocol_server.h>
#include <sup/auto-server/sup_auto_protocol.h>
#include <sup/epics/epics_protocol_factory.h>
#include <sup/sequencer/sequence_parser.h>

#include <testutils/test_utils.h>

#include <thread>

namespace testutils
{

void TestAutomationServer::Start(const std::string &server_name,
                                 const std::string &procedure_string)
{
  auto worker = [this, &server_name, &procedure_string]()
  {
    auto procedure =
        sup::sequencer::ParseProcedureString(testutils::CreateProcedureString(procedure_string));

    auto anyvalue_manager_registry = sup::auto_server::utils::CreateEPICSAnyValueManagerRegistry(1);
    sup::auto_server::AutomationServer auto_server{server_name, *anyvalue_manager_registry};
    auto_server.AddJob(std::move(procedure));

    // Instantiate RPC server for obtaining job information
    sup::auto_server::InfoProtocolServer info_server_protocol{auto_server};
    const sup::epics::PvAccessRPCServerConfig info_server_config{server_name};
    auto info_server_stack =
        sup::epics::CreateEPICSRPCServerStack(info_server_protocol, info_server_config);

    // Instantiate RPC server for controlling jobs
    sup::auto_server::ControlProtocolServer control_server_protocol{auto_server};
    auto control_service_name = sup::auto_server::GetControlServerName(server_name);
    const sup::epics::PvAccessRPCServerConfig control_server_config{control_service_name};
    auto control_server_stack =
        sup::epics::CreateEPICSRPCServerStack(control_server_protocol, control_server_config);

    while (m_continue)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  };

  m_future = std::async(std::launch::async, worker);
}

void TestAutomationServer::Stop()
{
  m_continue = false;
  m_future.wait();
}

void TestAutomationServer::WaitForResult()
{
  m_future.wait();
}

}  // namespace testutils
