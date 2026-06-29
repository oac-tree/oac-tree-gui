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

#include "oac_tree_gui/jobsystem/remote_connection_service.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/i_automation_client.h>
#include <oac_tree_gui/jobsystem/objects/abstract_job_handler.h>

#include <sup/gui/core/message_event.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_automation_client.h>

using ::testing::Return;

namespace oac_tree_gui
{

/**
 * @brief Tests for RemoteConnectionService class.
 *
 * Clients are created via AutomationClientDecoratorCreateFunc, which produces lightweight
 * decorators forwarding all calls to a single mocking object. As a consequence every client
 * created by the service shares the same mock and reports the same server name.
 */
class RemoteConnectionServiceTest : public ::testing::Test
{
public:
  std::unique_ptr<RemoteConnectionService> CreateService()
  {
    auto factory_func = test::AutomationClientDecoratorCreateFunc(m_mock_client);
    auto message_func = [](const sup::gui::MessageEvent&) {};
    return std::make_unique<RemoteConnectionService>(factory_func, message_func);
  }

  testing::NiceMock<test::MockAutomationClient> m_mock_client;
};

TEST_F(RemoteConnectionServiceTest, Connect)
{
  EXPECT_CALL(m_mock_client, GetServerName()).WillRepeatedly(Return("abc"));

  auto service = CreateService();

  EXPECT_TRUE(service->GetServerNames().empty());
  EXPECT_FALSE(service->HasClient("abc"));

  EXPECT_TRUE(service->Connect("abc"));
  EXPECT_EQ(service->GetServerNames(), std::vector<std::string>({"abc"}));
  EXPECT_TRUE(service->HasClient("abc"));

  // connecting again to the same server is a no-op
  EXPECT_TRUE(service->Connect("abc"));
  EXPECT_EQ(service->GetServerNames(), std::vector<std::string>({"abc"}));
}

TEST_F(RemoteConnectionServiceTest, ThrowOnConnect)
{
  auto factory_func = [](const AutomationServerInfo&) -> std::unique_ptr<IAutomationClient>
  { throw RuntimeException("Connection problem"); };

  auto message_func = [](const sup::gui::MessageEvent&) {};
  RemoteConnectionService service(factory_func, message_func);

  EXPECT_FALSE(service.Connect("abc"));
}

TEST_F(RemoteConnectionServiceTest, Disconnect)
{
  EXPECT_CALL(m_mock_client, GetServerName()).WillRepeatedly(Return("abc"));

  auto service = CreateService();

  EXPECT_TRUE(service->Connect("abc"));
  EXPECT_TRUE(service->HasClient("abc"));

  // disconnecting non-existing server is a no-op
  service->Disconnect("def");
  EXPECT_TRUE(service->HasClient("abc"));
  EXPECT_EQ(service->GetServerNames(), std::vector<std::string>({"abc"}));

  // disconnecting existing server removes the client
  service->Disconnect("abc");
  EXPECT_FALSE(service->HasClient("abc"));
  EXPECT_TRUE(service->GetServerNames().empty());
}

TEST_F(RemoteConnectionServiceTest, GetAutomationClient)
{
  EXPECT_CALL(m_mock_client, GetServerName()).WillRepeatedly(Return("abc"));

  auto service = CreateService();

  EXPECT_TRUE(service->Connect("abc"));
  EXPECT_TRUE(service->HasClient("abc"));

  // the client returned by the service is a decorator forwarding calls to our mock
  EXPECT_CALL(m_mock_client, GetJobCount()).WillOnce(Return(42));
  EXPECT_EQ(service->GetAutomationClient("abc").GetJobCount(), 42);

  EXPECT_THROW(service->GetAutomationClient("def"), RuntimeException);
}

}  // namespace oac_tree_gui
