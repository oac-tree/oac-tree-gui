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
#include <oac_tree_gui/model/remote_connection_info.h>

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
 * decorators forwarding behavioral calls to a single mocking object. Each decorator reports the
 * server name it was created for, so the service can manage several distinct clients backed by the
 * same mock.
 */
class RemoteConnectionServiceTest : public ::testing::Test
{
public:
  RemoteConnectionServiceTest()
  {
    // by default a freshly created client reports a live connection
    ON_CALL(m_mock_client, IsConnected()).WillByDefault(Return(true));
  }

  std::unique_ptr<RemoteConnectionService> CreateService()
  {
    auto factory_func = test::AutomationClientDecoratorCreateFunc(m_mock_client);
    return std::make_unique<RemoteConnectionService>(factory_func, m_message_func.AsStdFunction());
  }

  testing::NiceMock<test::MockAutomationClient> m_mock_client;
  testing::MockFunction<void(const sup::gui::MessageEvent&)> m_message_func;

  const AutomationServerInfo m_info_abc{EPICSServerInfo{"abc"}};
  const AutomationServerInfo m_info_def{EPICSServerInfo{"def"}};
};

TEST_F(RemoteConnectionServiceTest, Connect)
{
  auto service = CreateService();

  EXPECT_TRUE(service->GetServerInfos().empty());
  EXPECT_FALSE(service->HasClient(m_info_abc));

  EXPECT_TRUE(service->Connect(m_info_abc));
  EXPECT_EQ(service->GetServerInfos(), std::vector<AutomationServerInfo>({m_info_abc}));
  EXPECT_TRUE(service->HasClient(m_info_abc));

  // connecting again to the same server is a no-op
  EXPECT_TRUE(service->Connect(m_info_abc));
  EXPECT_EQ(service->GetServerInfos(), std::vector<AutomationServerInfo>({m_info_abc}));
}

TEST_F(RemoteConnectionServiceTest, ThrowOnConnect)
{
  auto factory_func = [](const AutomationServerInfo&) -> std::unique_ptr<IAutomationClient>
  { throw RuntimeException("Connection problem"); };

  // failed client creation must be reported via the message callback
  EXPECT_CALL(m_message_func, Call(::testing::_)).Times(1);

  RemoteConnectionService service(factory_func, m_message_func.AsStdFunction());

  EXPECT_FALSE(service.Connect(m_info_abc));
}

TEST_F(RemoteConnectionServiceTest, Disconnect)
{
  auto service = CreateService();

  EXPECT_TRUE(service->Connect(m_info_abc));
  EXPECT_TRUE(service->HasClient(m_info_abc));

  // disconnecting non-existing server is a no-op
  service->Disconnect(m_info_def);
  EXPECT_TRUE(service->HasClient(m_info_abc));
  EXPECT_EQ(service->GetServerInfos(), std::vector<AutomationServerInfo>({m_info_abc}));

  // disconnecting existing server removes the client
  service->Disconnect(m_info_abc);
  EXPECT_FALSE(service->HasClient(m_info_abc));
  EXPECT_TRUE(service->GetServerInfos().empty());
}

TEST_F(RemoteConnectionServiceTest, GetAutomationClient)
{
  auto service = CreateService();

  EXPECT_TRUE(service->Connect(m_info_abc));
  EXPECT_TRUE(service->HasClient(m_info_abc));

  // the client returned by the service is a decorator forwarding calls to our mock
  EXPECT_CALL(m_mock_client, GetJobCount()).WillOnce(Return(42));
  EXPECT_EQ(service->GetAutomationClient(m_info_abc).GetJobCount(), 42);

  EXPECT_THROW(service->GetAutomationClient(m_info_def), RuntimeException);
}

//! Connecting to several servers yields several distinct clients.
TEST_F(RemoteConnectionServiceTest, ConnectMultipleClients)
{
  auto service = CreateService();

  EXPECT_TRUE(service->Connect(m_info_abc));
  EXPECT_TRUE(service->Connect(m_info_def));

  EXPECT_EQ(service->GetServerInfos(),
            std::vector<AutomationServerInfo>({m_info_abc, m_info_def}));
  EXPECT_TRUE(service->HasClient(m_info_abc));
  EXPECT_TRUE(service->HasClient(m_info_def));

  // each server resolves to its own client
  EXPECT_EQ(service->GetAutomationClient(m_info_abc).GetServerInfo(), m_info_abc);
  EXPECT_EQ(service->GetAutomationClient(m_info_def).GetServerInfo(), m_info_def);

  // disconnecting one server leaves the other intact
  service->Disconnect(m_info_abc);
  EXPECT_FALSE(service->HasClient(m_info_abc));
  EXPECT_EQ(service->GetServerInfos(), std::vector<AutomationServerInfo>({m_info_def}));
}

//! The service creates one client per distinct server and reuses it on repeated connect.
TEST_F(RemoteConnectionServiceTest, CreatesClientPerServer)
{
  test::MockAutomationClientFactory factory;

  AutomationServerInfo info1(EPICSServerInfo{"abc"});
  AutomationServerInfo info2(EPICSServerInfo{"def"});

  EXPECT_CALL(factory, CreateClient(info1))
      .WillOnce([this, &info1](const AutomationServerInfo&)
                { return test::CreateAutomationClientDecorator(m_mock_client, info1); });
  EXPECT_CALL(factory, CreateClient(info2))
      .WillOnce([this, &info2](const AutomationServerInfo&)
                { return test::CreateAutomationClientDecorator(m_mock_client, info2); });

  RemoteConnectionService service(factory.CreateFunc(), m_message_func.AsStdFunction());

  EXPECT_TRUE(service.Connect(info1));
  EXPECT_TRUE(service.Connect(info2));

  // connecting again to an existing server does not create a new client
  EXPECT_TRUE(service.Connect(info1));

  EXPECT_EQ(service.GetServerInfos(), std::vector<AutomationServerInfo>({info1, info2}));
}

//! IsConnected reflects the liveness reported by the client, not mere existence.
TEST_F(RemoteConnectionServiceTest, IsConnectedReflectsClientLiveness)
{
  auto service = CreateService();

  // no client yet
  EXPECT_FALSE(service->IsConnected(m_info_abc));

  EXPECT_TRUE(service->Connect(m_info_abc));
  EXPECT_TRUE(service->HasClient(m_info_abc));
  EXPECT_TRUE(service->IsConnected(m_info_abc));

  // the client is still present, but its connection has dropped
  ON_CALL(m_mock_client, IsConnected()).WillByDefault(Return(false));
  EXPECT_TRUE(service->HasClient(m_info_abc));
  EXPECT_FALSE(service->IsConnected(m_info_abc));
}

//! Connecting to a server whose client has a broken connection drops the stale client and
//! re-creates it.
TEST_F(RemoteConnectionServiceTest, ReconnectDropsStaleClient)
{
  test::MockAutomationClientFactory factory;

  // client is created twice: initial connect, and re-connect after the connection breaks
  EXPECT_CALL(factory, CreateClient(m_info_abc))
      .Times(2)
      .WillRepeatedly([this](const AutomationServerInfo&)
                      { return test::CreateAutomationClientDecorator(m_mock_client, m_info_abc); });

  RemoteConnectionService service(factory.CreateFunc(), m_message_func.AsStdFunction());

  EXPECT_TRUE(service.Connect(m_info_abc));

  // connection breaks; the next connect must prune the dead client and create a fresh one
  ON_CALL(m_mock_client, IsConnected()).WillByDefault(Return(false));
  EXPECT_TRUE(service.Connect(m_info_abc));

  EXPECT_EQ(service.GetServerInfos(), std::vector<AutomationServerInfo>({m_info_abc}));
}

}  // namespace oac_tree_gui
