/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/jobsystem/remote_connection_service.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/abstract_job_handler.h>
#include <sequencergui/jobsystem/i_automation_client.h>

#include <gtest/gtest.h>

namespace sequencergui
{

/**
 * @brief Tests for RemoteConnectionServiceTest class.
 */
class RemoteConnectionServiceTest : public ::testing::Test
{
public:
  class TestClient : public IAutomationClient
  {
  public:
    explicit TestClient(const std::string& name) : m_name(name) {}

    std::string GetServerName() const override { return m_name; };

    size_t GetJobCount() const override { return 42; }

    std::string GetProcedureName(size_t job_index) const override
    {
      (void)job_index;
      return {};
    }

    std::unique_ptr<AbstractJobHandler> CreateJobHandler(RemoteJobItem* job_item,
                                                         const UserContext& user_context) override
    {
      (void)job_item;
      (void)user_context;
      return {};
    }

    std::string m_name;
  };

  /**
   * @brief Returns lamba to create test clients.
   */
  static RemoteConnectionService::create_client_t CreateFunc()
  {
    auto result = [](const std::string& name) { return std::make_unique<TestClient>(name); };
    return result;
  }
};

TEST_F(RemoteConnectionServiceTest, Connect)
{
  RemoteConnectionService service(CreateFunc());
  EXPECT_TRUE(service.GetServerNames().empty());
  EXPECT_FALSE(service.HasClient("abc"));

  EXPECT_TRUE(service.Connect("abc"));
  EXPECT_EQ(service.GetServerNames(), std::vector<std::string>({"abc"}));
  EXPECT_TRUE(service.HasClient("abc"));

  // same name, do nothing
  EXPECT_TRUE(service.Connect("abc"));
  EXPECT_EQ(service.GetServerNames(), std::vector<std::string>({"abc"}));

  service.Connect("def");
  EXPECT_EQ(service.GetServerNames(), std::vector<std::string>({"abc", "def"}));
}

TEST_F(RemoteConnectionServiceTest, ThrowOnConnect)
{
  auto factory_func = [](const std::string&) -> std::unique_ptr<IAutomationClient>
  { throw RuntimeException("Connection problem"); };

  RemoteConnectionService service(factory_func);

  EXPECT_FALSE(service.Connect("abc"));
}

TEST_F(RemoteConnectionServiceTest, Disconnect)
{
  RemoteConnectionService service(CreateFunc());
  EXPECT_TRUE(service.GetServerNames().empty());
  EXPECT_FALSE(service.HasClient("abc"));

  EXPECT_TRUE(service.Connect("a1"));
  EXPECT_TRUE(service.Connect("a2"));
  EXPECT_TRUE(service.Connect("a3"));
  EXPECT_EQ(service.GetServerNames(), std::vector<std::string>({"a1", "a2", "a3"}));

  service.Disconnect("def");
  EXPECT_EQ(service.GetServerNames(), std::vector<std::string>({"a1", "a2", "a3"}));

  service.Disconnect("a2");
  EXPECT_EQ(service.GetServerNames(), std::vector<std::string>({"a1", "a3"}));
}

TEST_F(RemoteConnectionServiceTest, GetAutomationClient)
{
  auto client = std::make_unique<TestClient>("abc");
  auto client_ptr = client.get();

  auto factory_func = [&client](const std::string&) { return std::move(client); };

  RemoteConnectionService service(factory_func);

  EXPECT_TRUE(service.Connect("abc"));

  EXPECT_TRUE(service.HasClient("abc"));
  EXPECT_EQ(&service.GetAutomationClient("abc"), client_ptr);
  EXPECT_THROW(service.GetAutomationClient("def"), RuntimeException);
}

}  // namespace sequencergui
