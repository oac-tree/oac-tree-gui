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

#include "test_automation_server.h"

#include <sup/epics/epics_protocol_factory.h>
#include <sup/oac-tree-server/automation_server.h>
#include <sup/oac-tree-server/epics_utils.h>
#include <sup/oac-tree-server/ws_utils.h>
#include <sup/oac-tree/sequence_parser.h>

#include <testutils/test_utils.h>

#include <thread>

namespace oac_tree_gui::test
{

namespace
{

/**
 * @brief Creates automation server protocol factory.
 */
std::unique_ptr<sup::oac_tree_server::IServerProtocolFactory> CreateProtocolFactory(
    const AutomationServerInfo& server_info)
{
  if (std::holds_alternative<EPICSServerInfo>(server_info))
  {
    return sup::oac_tree_server::utils::CreateEPICSServerProtocolFactory(
        std::get<EPICSServerInfo>(server_info).server_name, 1);
  }

  if (std::holds_alternative<WebSocketsServerInfo>(server_info))
  {
    return sup::oac_tree_server::utils::CreateWSServerProtocolFactory(
        std::get<WebSocketsServerInfo>(server_info).server_port);
  }

  return {};
}

}  // namespace

void TestAutomationServer::Start(const AutomationServerInfo& server_info,
                                 const std::string& procedure_string)
{
  // std::async decay-copies its arguments, so temporaries passed to Start() remain valid
  m_future = std::async(std::launch::async, &TestAutomationServer::RunServer, this, server_info,
                        procedure_string);
}

void TestAutomationServer::RunServer(AutomationServerInfo server_info, std::string procedure_string)
{
  auto procedure =
      sup::oac_tree::ParseProcedureString(test::CreateProcedureString(procedure_string));

  auto server_protocol_factory = CreateProtocolFactory(server_info);

  sup::oac_tree_server::AutomationServer auto_server{*server_protocol_factory};
  auto_server.AddJob(std::move(procedure));

  // Instantiate main server for obtaining job information and control
  auto main_server = server_protocol_factory->CreateMainServer(auto_server);

  while (m_continue)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
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

}  // namespace oac_tree_gui::test
