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
#include <sup/oac-tree-server/control_protocol_server.h>
#include <sup/oac-tree-server/epics_config_utils.h>
#include <sup/oac-tree-server/info_protocol_server.h>
#include <sup/oac-tree-server/oac_tree_protocol.h>
#include <sup/oac-tree/sequence_parser.h>

#include <testutils/test_utils.h>

#include <thread>

namespace oac_tree_gui::test
{

void TestAutomationServer::Start(const std::string& server_name,
                                 const std::string& procedure_string)
{
  auto worker = [this, &server_name, &procedure_string]()
  {
    auto procedure =
        sup::oac_tree::ParseProcedureString(test::CreateProcedureString(procedure_string));

    auto server_protocol_factory =
      sup::oac_tree_server::utils::CreateEPICSServerProtocolFactory(server_name, 1);

    sup::oac_tree_server::AutomationServer auto_server{*server_protocol_factory};
    auto_server.AddJob(std::move(procedure));

    // Instantiate main server for obtaining job information and control
    auto main_server = server_protocol_factory->CreateMainServer(auto_server);

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

}  // namespace oac_tree_gui::test
