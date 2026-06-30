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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_AUTOMATION_CLIENT_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_AUTOMATION_CLIENT_H_

#include <oac_tree_gui/jobsystem/automation_client_factory.h>
#include <oac_tree_gui/jobsystem/i_automation_client.h>
#include <oac_tree_gui/model/remote_connection_info.h>

#include <gmock/gmock.h>

#include <optional>
#include <string>

namespace oac_tree_gui::test
{

/**
 * @brief The MockAutomationClient class helps to test RemoteConnectionService.
 */
class MockAutomationClient : public IAutomationClient
{
public:
  MOCK_METHOD(AutomationServerInfo, GetServerInfo, (), (const, override));
  MOCK_METHOD(std::size_t, GetJobCount, (), (const, override));
  MOCK_METHOD(std::string, GetProcedureName, (std::uint32_t), (const, override));
  MOCK_METHOD(std::unique_ptr<AbstractJobHandler>, CreateJobHandler,
              (RemoteJobItem*, const UserContext&), (override));
};

/**
 * @brief The AutomationClientDecorator class forwards all calls to underlying IAutomationClient
 * class.
 *
 * It is used in situations when we have to use unique_ptr<IAutomationClient>, and do not want to
 * loose an ownerhsip on gtest mocking object.
 *
 * When constructed with an explicit server name, GetServerName() reports that name instead of
 * forwarding to the decoratee. This allows a single mocking object to back several decorators, each
 * representing a distinct server.
 */
class AutomationClientDecorator : public IAutomationClient
{
public:
  explicit AutomationClientDecorator(IAutomationClient& decoratee);

  AutomationClientDecorator(IAutomationClient& decoratee, AutomationServerInfo server_info);

  AutomationServerInfo GetServerInfo() const override;

  std::size_t GetJobCount() const override;

  std::string GetProcedureName(std::uint32_t job_index) const override;

  std::unique_ptr<AbstractJobHandler> CreateJobHandler(RemoteJobItem* job_item,
                                                       const UserContext& user_context) override;

private:
  IAutomationClient& m_decoratee;
  std::optional<AutomationServerInfo> m_server_info;
};

/**
 * @brief Creates simple forward decorator around mocking object.
 */
std::unique_ptr<IAutomationClient> CreateAutomationClientDecorator(IAutomationClient& decoratee);

/**
 * @brief Creates a forward decorator around mocking object reporting the given server name.
 */
std::unique_ptr<IAutomationClient> CreateAutomationClientDecorator(
    IAutomationClient& decoratee, AutomationServerInfo server_info);

/**
 * @brief Creates factory functions to create decorators around mocking objects.
 *
 * Each created decorator reports the server name derived from the requested AutomationServerInfo,
 * so the service can manage several distinct clients backed by the same mocking object.
 */
AutomationClientFunc AutomationClientDecoratorCreateFunc(IAutomationClient& decoratee);

/**
 * @brief The MockAutomationClientFactory class helps to test how RemoteConnectionService creates
 * clients.
 *
 * Unlike AutomationClientDecoratorCreateFunc, it exposes the creation call itself as a mocking
 * method, so tests can verify with which AutomationServerInfo and how many times clients are
 * created.
 */
class MockAutomationClientFactory
{
public:
  MOCK_METHOD(std::unique_ptr<IAutomationClient>, CreateClient, (const AutomationServerInfo&));

  /**
   * @brief Returns factory function forwarding to the mocked CreateClient method.
   */
  AutomationClientFunc CreateFunc();
};

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_AUTOMATION_CLIENT_H_
