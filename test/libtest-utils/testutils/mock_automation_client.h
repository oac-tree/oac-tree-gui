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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_AUTOMATION_CLIENT_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_AUTOMATION_CLIENT_H_

#include <sequencergui/jobsystem/i_automation_client.h>

#include <gmock/gmock.h>

#include <functional>

namespace testutils
{

/**
 * @brief The MockAutomationClient class helps to test RemoteConnectionService.
 */
class MockAutomationClient : public sequencergui::IAutomationClient
{
public:
  MOCK_METHOD(std::string, GetServerName, (), (const, override));
  MOCK_METHOD(size_t, GetJobCount, (), (const, override));
  MOCK_METHOD(std::string, GetProcedureName, (size_t), (const, override));
  MOCK_METHOD(std::unique_ptr<sequencergui::AbstractJobHandler>, CreateJobHandler,
              (sequencergui::RemoteJobItem*, const sequencergui::UserContext&), (override));
};

/**
 * @brief The AutomationClientDecorator class forwards all calls to underlying IAutomationClient
 * class.
 *
 * It is used in situations when we have to use unique_ptr<IAutomationClient>, and do not want to
 * loose an ownerhsip on gtest mocking object.
 */
class AutomationClientDecorator : public sequencergui::IAutomationClient
{
public:
  AutomationClientDecorator(sequencergui::IAutomationClient& decoratee);

  std::string GetServerName() const override;

  size_t GetJobCount() const override;

  std::string GetProcedureName(size_t job_index) const override;

  std::unique_ptr<sequencergui::AbstractJobHandler> CreateJobHandler(
      sequencergui::RemoteJobItem* job_item,
      const sequencergui::UserContext& user_context) override;

private:
  sequencergui::IAutomationClient& m_decoratee;
};

/**
 * @brief Creates simple forward decorator around mocking object.
 */
std::unique_ptr<sequencergui::IAutomationClient> CreateAutomationClientDecorator(
    sequencergui::IAutomationClient& decoratee);

/**
 * @brief Creates factory functions to create decorators around mocking objects.
 */
std::function<std::unique_ptr<sequencergui::IAutomationClient>(const std::string& server_name)>
AutomationClientDecoratorCreateFunc(sequencergui::IAutomationClient& decoratee);

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_AUTOMATION_CLIENT_H_
