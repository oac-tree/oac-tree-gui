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

#ifndef LIBTEST_UTILS_TEST_AUTOMATION_SERVER_H_
#define LIBTEST_UTILS_TEST_AUTOMATION_SERVER_H_

#include <atomic>
#include <chrono>
#include <future>
#include <string>

#include <oac_tree_gui/model/remote_connection_info.h>

namespace oac_tree_gui::test
{

/**
 * @brief The TestAutomationServer class runs sequencer automation server in a thread.
 */
class TestAutomationServer
{
public:
  TestAutomationServer() = default;

  /**
   * @brief Starts the server in a thread and waits until it accepts a connection.
   *
   * @param server_info how clients should reach the server.
   * @param procedure_string the procedure the server exposes as a job.
   * @param timeout how long to wait for the server to come up.
   * @return false if the server did not accept a connection in time.
   */
  bool Start(const AutomationServerInfo& server_info, const std::string& procedure_string,
             std::chrono::milliseconds timeout = std::chrono::seconds(30));

  void Stop();

  void WaitForResult();

private:
  void RunServer(AutomationServerInfo server_info, std::string procedure_string);

  std::future<void> m_future;
  std::atomic<bool> m_continue{true};
};

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_TEST_UTILS_H_
