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

#include "oac_tree_gui/settings/settings_manager.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/settings/in_memory_settings_storage.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

namespace
{

constexpr auto kGroup = "Group";
constexpr auto kOtherGroup = "OtherGroup";

struct TestInfo
{
  bool flag{false};
  mvvm::int32 count{0};
};

bool operator==(const TestInfo& lhs, const TestInfo& rhs)
{
  return lhs.flag == rhs.flag && lhs.count == rhs.count;
}

template <typename Archive>
void Visit(Archive& archive, TestInfo& info)
{
  archive("flag", info.flag);
  archive("count", info.count);
}

}  // namespace

/**
 * @brief Tests for SettingsManager class.
 */
class SettingsManagerTest : public ::testing::Test
{
public:
  SettingsManagerTest()
  {
    m_manager.SetStorage(SettingsScope::kApplication, &m_app_storage);
    m_manager.SetStorage(SettingsScope::kProject, &m_project_storage);
  }

  //! Returns a subscriber callback forwarding to the given mock.
  static std::function<void(const TestInfo&)> Callback(
      ::testing::MockFunction<void(const TestInfo&)>& mock)
  {
    return mock.AsStdFunction();
  }

  InMemorySettingsStorage m_app_storage;
  InMemorySettingsStorage m_project_storage;
  SettingsManager m_manager;
  ::testing::MockFunction<void(const TestInfo&)> m_mock_callback;
};

TEST_F(SettingsManagerTest, InitialState)
{
  const SettingsManager manager;
  EXPECT_EQ(manager.GetStorage(SettingsScope::kApplication), nullptr);
  EXPECT_EQ(manager.GetStorage(SettingsScope::kProject), nullptr);
}

TEST_F(SettingsManagerTest, SetStorage)
{
  EXPECT_EQ(m_manager.GetStorage(SettingsScope::kApplication), &m_app_storage);
  EXPECT_EQ(m_manager.GetStorage(SettingsScope::kProject), &m_project_storage);

  m_manager.SetStorage(SettingsScope::kProject, nullptr);
  EXPECT_EQ(m_manager.GetStorage(SettingsScope::kProject), nullptr);
}

TEST_F(SettingsManagerTest, SetAndGet)
{
  const TestInfo app_info{true, 1};
  const TestInfo project_info{false, 2};

  m_manager.Set(kGroup, app_info, SettingsScope::kApplication);
  m_manager.Set(kGroup, project_info, SettingsScope::kProject);

  EXPECT_EQ(m_manager.Get<TestInfo>(kGroup, SettingsScope::kApplication), app_info);
  EXPECT_EQ(m_manager.Get<TestInfo>(kGroup, SettingsScope::kProject), project_info);
  EXPECT_EQ(m_app_storage.Read("Group/count"), mvvm::variant_t{mvvm::int32{1}});
  EXPECT_EQ(m_project_storage.Read("Group/count"), mvvm::variant_t{mvvm::int32{2}});
}

TEST_F(SettingsManagerTest, GetWithoutStorage)
{
  const SettingsManager manager;
  EXPECT_THROW(manager.Get<TestInfo>(kGroup, SettingsScope::kApplication), RuntimeException);
  EXPECT_THROW(manager.Get<TestInfo>(kGroup, SettingsScope::kProject), RuntimeException);
}

TEST_F(SettingsManagerTest, SetWithoutStorage)
{
  SettingsManager manager;
  auto connection =
      manager.Subscribe<TestInfo>(kGroup, SettingsScope::kProject, Callback(m_mock_callback));

  EXPECT_CALL(m_mock_callback, Call(::testing::_)).Times(0);
  EXPECT_THROW(manager.Set(kGroup, TestInfo{true, 1}, SettingsScope::kProject), RuntimeException);
}

TEST_F(SettingsManagerTest, NotifyWithoutStorage)
{
  m_manager.SetStorage(SettingsScope::kProject, nullptr);
  auto connection =
      m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kProject, Callback(m_mock_callback));

  EXPECT_CALL(m_mock_callback, Call(::testing::_)).Times(0);
  EXPECT_NO_THROW(m_manager.NotifyGroup(kGroup, SettingsScope::kProject));
  EXPECT_NO_THROW(m_manager.NotifyAll(SettingsScope::kProject));
}

TEST_F(SettingsManagerTest, SubscriberCalledOnSet)
{
  auto connection =
      m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kApplication, Callback(m_mock_callback));

  const TestInfo info{true, 42};
  EXPECT_CALL(m_mock_callback, Call(info)).Times(1);
  m_manager.Set(kGroup, info, SettingsScope::kApplication);
}

TEST_F(SettingsManagerTest, SubscriberNotCalledForOtherGroupOrScope)
{
  auto connection =
      m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kApplication, Callback(m_mock_callback));

  EXPECT_CALL(m_mock_callback, Call(::testing::_)).Times(0);
  m_manager.Set(kOtherGroup, TestInfo{true, 1}, SettingsScope::kApplication);
  m_manager.Set(kGroup, TestInfo{true, 1}, SettingsScope::kProject);
}

TEST_F(SettingsManagerTest, SubscriberNotCalledAfterDisconnect)
{
  auto connection =
      m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kApplication, Callback(m_mock_callback));
  connection.Disconnect();

  EXPECT_CALL(m_mock_callback, Call(::testing::_)).Times(0);
  m_manager.Set(kGroup, TestInfo{true, 1}, SettingsScope::kApplication);
}

TEST_F(SettingsManagerTest, SubscriberNotCalledAfterConnectionDestroyed)
{
  {
    auto connection = m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kApplication,
                                                    Callback(m_mock_callback));
  }

  EXPECT_CALL(m_mock_callback, Call(::testing::_)).Times(0);
  m_manager.Set(kGroup, TestInfo{true, 1}, SettingsScope::kApplication);
}

TEST_F(SettingsManagerTest, SeveralSubscribersOfGroup)
{
  ::testing::MockFunction<void(const TestInfo&)> mock_callback2;
  auto connection1 =
      m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kApplication, Callback(m_mock_callback));
  auto connection2 =
      m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kApplication, Callback(mock_callback2));

  const TestInfo info{true, 1};
  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_mock_callback, Call(info)).Times(1);
    EXPECT_CALL(mock_callback2, Call(info)).Times(1);
  }
  m_manager.Set(kGroup, info, SettingsScope::kApplication);
}

TEST_F(SettingsManagerTest, UnsubscribeDuringNotification)
{
  ::testing::MockFunction<void(const TestInfo&)> mock_callback2;
  ScopedCallback connection2;

  auto on_first = [&connection2](const TestInfo&) { connection2.Disconnect(); };
  auto connection1 = m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kApplication, on_first);
  connection2 =
      m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kApplication, Callback(mock_callback2));

  EXPECT_CALL(mock_callback2, Call(::testing::_)).Times(0);
  m_manager.Set(kGroup, TestInfo{true, 1}, SettingsScope::kApplication);
}

TEST_F(SettingsManagerTest, UnsubscribeSelfDuringNotification)
{
  ScopedCallback connection;
  std::int32_t call_count{0};
  std::string captured_value;

  // capture is used after own removal, long string to have it on heap
  const std::string expected("capture outliving the removal of own entry");
  auto on_notify = [&connection, &call_count, &captured_value, tag = expected](const TestInfo&)
  {
    connection.Disconnect();
    ++call_count;
    captured_value = tag;
  };
  connection = m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kApplication, on_notify);

  m_manager.Set(kGroup, TestInfo{true, 1}, SettingsScope::kApplication);
  EXPECT_FALSE(connection.IsConnected());
  EXPECT_EQ(call_count, 1);
  EXPECT_EQ(captured_value, expected);

  // not called anymore
  m_manager.Set(kGroup, TestInfo{true, 2}, SettingsScope::kApplication);
  EXPECT_EQ(call_count, 1);
}

TEST_F(SettingsManagerTest, SubscribeDuringNotification)
{
  ScopedCallback connection2;
  auto on_first = [this, &connection2](const TestInfo&)
  {
    connection2 = m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kApplication,
                                                Callback(m_mock_callback));
  };
  auto connection1 = m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kApplication, on_first);

  // new subscriber is not called in the current round
  EXPECT_CALL(m_mock_callback, Call(::testing::_)).Times(0);
  m_manager.Set(kGroup, TestInfo{true, 1}, SettingsScope::kApplication);
}

TEST_F(SettingsManagerTest, CollectCallsProvidersOfScope)
{
  auto app_connection = m_manager.RegisterProvider(kGroup, SettingsScope::kApplication,
                                                   []() { return TestInfo{true, 1}; });
  auto project_connection = m_manager.RegisterProvider(kGroup, SettingsScope::kProject,
                                                       []() { return TestInfo{true, 2}; });

  m_manager.Collect(SettingsScope::kApplication);

  EXPECT_EQ(m_manager.Get<TestInfo>(kGroup, SettingsScope::kApplication), (TestInfo{true, 1}));
  EXPECT_TRUE(m_project_storage.GetKeys().empty());
}

TEST_F(SettingsManagerTest, CollectDoesNotNotify)
{
  auto provider_connection = m_manager.RegisterProvider(kGroup, SettingsScope::kApplication,
                                                        []() { return TestInfo{true, 1}; });
  auto subscriber_connection =
      m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kApplication, Callback(m_mock_callback));

  EXPECT_CALL(m_mock_callback, Call(::testing::_)).Times(0);
  m_manager.Collect(SettingsScope::kApplication);
}

TEST_F(SettingsManagerTest, ProviderNotCalledAfterConnectionDestroyed)
{
  ::testing::MockFunction<TestInfo()> mock_provider;
  {
    auto connection = m_manager.RegisterProvider(kGroup, SettingsScope::kApplication,
                                                 mock_provider.AsStdFunction());
  }

  EXPECT_CALL(mock_provider, Call()).Times(0);
  m_manager.Collect(SettingsScope::kApplication);
  EXPECT_TRUE(m_app_storage.GetKeys().empty());
}

TEST_F(SettingsManagerTest, CollectWithoutStorage)
{
  ::testing::MockFunction<TestInfo()> mock_provider;
  m_manager.SetStorage(SettingsScope::kProject, nullptr);
  auto connection =
      m_manager.RegisterProvider(kGroup, SettingsScope::kProject, mock_provider.AsStdFunction());

  EXPECT_CALL(mock_provider, Call()).Times(0);
  m_manager.Collect(SettingsScope::kProject);
}

TEST_F(SettingsManagerTest, NotifyAll)
{
  ::testing::MockFunction<void(const TestInfo&)> mock_other_group;
  ::testing::MockFunction<void(const TestInfo&)> mock_app_scope;
  auto connection1 =
      m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kProject, Callback(m_mock_callback));
  auto connection2 = m_manager.Subscribe<TestInfo>(kOtherGroup, SettingsScope::kProject,
                                                   Callback(mock_other_group));
  auto connection3 =
      m_manager.Subscribe<TestInfo>(kGroup, SettingsScope::kApplication, Callback(mock_app_scope));

  const TestInfo info{true, 5};
  WriteToStorage(m_project_storage, kGroup, info);

  EXPECT_CALL(m_mock_callback, Call(info)).Times(1);
  EXPECT_CALL(mock_other_group, Call(TestInfo{})).Times(1);
  EXPECT_CALL(mock_app_scope, Call(::testing::_)).Times(0);
  m_manager.NotifyAll(SettingsScope::kProject);
}

}  // namespace oac_tree_gui::test
