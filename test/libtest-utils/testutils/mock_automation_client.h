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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_AUTOMATION_CLIENT_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_AUTOMATION_CLIENT_H_

#include <oac-tree-gui/jobsystem/i_automation_client.h>

#include <gmock/gmock.h>

#include <functional>

namespace oac_tree_gui::test
{

/**
 * @brief The MockAutomationClient class helps to test RemoteConnectionService.
 */
class MockAutomationClient : public oac_tree_gui::IAutomationClient
{
public:
  MOCK_METHOD(std::string, GetServerName, (), (const, override));
  MOCK_METHOD(std::size_t, GetJobCount, (), (const, override));
  MOCK_METHOD(std::string, GetProcedureName, (std::size_t), (const, override));
  MOCK_METHOD(std::unique_ptr<oac_tree_gui::AbstractJobHandler>, CreateJobHandler,
              (oac_tree_gui::RemoteJobItem*, const oac_tree_gui::UserContext&), (override));
};

/**
 * @brief The AutomationClientDecorator class forwards all calls to underlying IAutomationClient
 * class.
 *
 * It is used in situations when we have to use unique_ptr<IAutomationClient>, and do not want to
 * loose an ownerhsip on gtest mocking object.
 */
class AutomationClientDecorator : public oac_tree_gui::IAutomationClient
{
public:
  AutomationClientDecorator(oac_tree_gui::IAutomationClient& decoratee);

  std::string GetServerName() const override;

  std::size_t GetJobCount() const override;

  std::string GetProcedureName(std::size_t job_index) const override;

  std::unique_ptr<oac_tree_gui::AbstractJobHandler> CreateJobHandler(
      oac_tree_gui::RemoteJobItem* job_item,
      const oac_tree_gui::UserContext& user_context) override;

private:
  oac_tree_gui::IAutomationClient& m_decoratee;
};

/**
 * @brief Creates simple forward decorator around mocking object.
 */
std::unique_ptr<oac_tree_gui::IAutomationClient> CreateAutomationClientDecorator(
    oac_tree_gui::IAutomationClient& decoratee);

/**
 * @brief Creates factory functions to create decorators around mocking objects.
 */
std::function<std::unique_ptr<oac_tree_gui::IAutomationClient>(const std::string& server_name)>
AutomationClientDecoratorCreateFunc(oac_tree_gui::IAutomationClient& decoratee);

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_AUTOMATION_CLIENT_H_
