/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#ifndef LIBTEST_UTILS_TEST_AUTOMATION_SERVER_H_
#define LIBTEST_UTILS_TEST_AUTOMATION_SERVER_H_

#include <atomic>
#include <future>
#include <string>

namespace sequencergui::test
{

/**
 * @brief The TestAutomationServer class runs sequencer automation server in a thread.
 */
class TestAutomationServer
{
public:
  TestAutomationServer() = default;

  void Start(const std::string& server_name, const std::string& procedure_string);

  void Stop();

  void WaitForResult();

private:
  std::future<void> m_future;
  std::atomic<bool> m_continue{true};
};

}  // namespace sequencergui::test

#endif  // LIBTEST_UTILS_TESTUTILS_TEST_UTILS_H_
