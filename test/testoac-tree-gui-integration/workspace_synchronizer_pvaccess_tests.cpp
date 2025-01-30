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

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/pvmonitor/monitor_model.h>
#include <oac_tree_gui/pvmonitor/workspace_monitor_helper.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/gui/core/exceptions.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/test/mock_model_listener.h>

#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>
#include <testutils/mock_domain_workspace_listener.h>
#include <testutils/sequencer_test_utils.h>
#include <testutils/test_utils.h>

#include <QTest>

#include "oac_tree_gui/pvmonitor/workspace_synchronizer.h"

namespace oac_tree_gui::test
{

namespace
{
const std::string kTestPrefix("WorkspaceSynchronizerPVAccessTests:");
}  // namespace

//! Tests for WorkspaceSynchronizer class.

class WorkspaceSynchronizerPVAccessTest : public ::testing::Test
{
public:
  using mock_listener_t = ::testing::StrictMock<mvvm::test::MockModelListener>;

  WorkspaceSynchronizerPVAccessTest() { m_workspace_item = m_model.InsertItem<WorkspaceItem>(); }

  //! Disables all tests in the fixture if PVAccess is not available
  void SetUp() override
  {
    if (!IsSequencerPluginEpicsAvailable())
    {
      GTEST_SKIP();
    }
  }

  //! Creates synchronizer for testing.
  std::unique_ptr<WorkspaceSynchronizer> CreateSynchronizer()
  {
    // populate sequencer workspace so it match WorkspaceItem
    PopulateDomainWorkspace(*m_workspace_item, m_workspace);
    return std::make_unique<WorkspaceSynchronizer>(m_workspace_item, &m_workspace);
  }

  MonitorModel m_model;
  WorkspaceItem* m_workspace_item{nullptr};
  sup::oac_tree::Workspace m_workspace;
};

//! Single server variable (struct) in a workspace.
//! Creating variable on GUI side, duplicating in the domain workspace. Starting listening,
//! making sure that correct signals are flying and initial values coincide.
TEST_F(WorkspaceSynchronizerPVAccessTest, ServerVariableSimpleStart)
{
  const std::string kChannelName(kTestPrefix + "STRUCT1");
  const std::string var_name("var");
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  // creating PVServerVariableItem in the model
  auto variable_item =
      m_model.InsertItem<PvAccessServerVariableItem>(m_workspace_item, mvvm::TagIndex::Append());
  variable_item->SetChannel(kChannelName);
  variable_item->SetName(var_name);
  SetAnyValue(initial_value, *variable_item);

  EXPECT_FALSE(variable_item->IsAvailable());

  // creating synchronizer
  auto synchronizer = CreateSynchronizer();
  m_workspace.Setup();

  // Creating listeners and setting callback expectations.
  mock_listener_t model_listener(&m_model);

  // empty_queue_predicate will cause  DataChangedEvent for IsAvailable status
  auto is_available_property = GetIsAvailableItem(*variable_item);
  auto expected_event1 = mvvm::DataChangedEvent{is_available_property, mvvm::DataRole::kData};
  EXPECT_CALL(model_listener, OnDataChanged(expected_event1)).Times(1);

  auto empty_queue_predicate = [&synchronizer]() { return synchronizer->IsEmptyQueue(); };
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));

  EXPECT_TRUE(m_workspace.WaitForVariable(var_name, 1.0));

  // checking the value on domain side and GUI
  EXPECT_EQ(GetAnyValue(var_name, m_workspace), initial_value);
  EXPECT_TRUE(variable_item->IsAvailable());
  EXPECT_EQ(GetAnyValue(*variable_item), initial_value);
}

//! Single server variable (struct) in a workspace.
//! The data is set on the GUI side, validating updates on the domain side.

TEST_F(WorkspaceSynchronizerPVAccessTest, SetDataFromGUI)
{
  const std::string kChannelName(kTestPrefix + "STRUCT2");
  const std::string var_name("var");
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  // creating PVServerVariableItem in the model
  auto variable_item =
      m_model.InsertItem<PvAccessServerVariableItem>(m_workspace_item, mvvm::TagIndex::Append());
  variable_item->SetChannel(kChannelName);
  variable_item->SetName(var_name);
  SetAnyValue(initial_value, *variable_item);

  EXPECT_FALSE(variable_item->IsAvailable());

  // creating synchronizer
  auto synchronizer = CreateSynchronizer();
  m_workspace.Setup();

  // Creating domain and setting callback expectations.
  test::MockDomainWorkspaceListener domain_listener(m_workspace);
  auto anyvalue_item = variable_item->GetAnyValueItem();
  const sup::dto::AnyValue expected_value({{"value", {sup::dto::SignedInteger32Type, 42}}});
  EXPECT_CALL(domain_listener, OnEvent(var_name, expected_value, true)).Times(1);

  // creating model listener and setting expectations
  mvvm::test::MockModelListener model_listener(&m_model);
  auto scalar_field = anyvalue_item->GetChildren().at(0);
  auto expected_event = mvvm::DataChangedEvent{scalar_field, mvvm::DataRole::kData};
  EXPECT_CALL(model_listener, OnDataChanged(expected_event)).Times(1);

  // setting the data from GUI (will trigger expectation)
  scalar_field->SetData(42);

  // validating the data in the domain
  EXPECT_EQ(GetAnyValue(var_name, m_workspace), expected_value);
}

//! Single server variable (struct) in a workspace.
//! The data is set on the domain side, validating updates on the GUI side.
TEST_F(WorkspaceSynchronizerPVAccessTest, SetDataFromDomain)
{
  const std::string kChannelName(kTestPrefix + "STRUCT3");
  const std::string var_name("var");
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  // creating PVServerVariableItem in the model
  auto variable_item =
      m_model.InsertItem<PvAccessServerVariableItem>(m_workspace_item, mvvm::TagIndex::Append());
  variable_item->SetChannel(kChannelName);
  variable_item->SetName(var_name);
  SetAnyValue(initial_value, *variable_item);

  EXPECT_FALSE(variable_item->IsAvailable());

  // creating synchronizer
  auto synchronizer = CreateSynchronizer();
  m_workspace.Setup();

  // creating model listener and setting expectations
  mvvm::test::MockModelListener model_listener(&m_model);

  // Start method below will cause  DataChangedEvent for IsAvailable status
  auto is_available_property = GetIsAvailableItem(*variable_item);
  auto expected_event1 = mvvm::DataChangedEvent{is_available_property, mvvm::DataRole::kData};
  EXPECT_CALL(model_listener, OnDataChanged(expected_event1)).Times(1);

  // expecting data changed event of the scalar field
  auto anyvalue_item = variable_item->GetAnyValueItem();
  auto scalar_field = anyvalue_item->GetChildren().at(0);
  auto expected_event = mvvm::DataChangedEvent{scalar_field, mvvm::DataRole::kData};
  EXPECT_CALL(model_listener, OnDataChanged(expected_event)).Times(1);

  // setting the data from the domain (will trigger expectation)
  const sup::dto::AnyValue expected_value({{"value", {sup::dto::SignedInteger32Type, 42}}});
  EXPECT_TRUE(m_workspace.SetValue(var_name, expected_value));

  auto empty_queue_predicate = [&synchronizer]() { return synchronizer->IsEmptyQueue(); };
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));

  // validating the data in the domain and the GUI
  EXPECT_EQ(GetAnyValue(var_name, m_workspace), expected_value);
  EXPECT_EQ(GetAnyValue(*variable_item), expected_value);
}

//! One server and one client variable (struct) in a workspace.
//! Creating variables on a GUI side, duplicating in the domain workspace. Starting listening,
//! making sure that correct signals are flying and initial values coincide.

TEST_F(WorkspaceSynchronizerPVAccessTest, ClientAndServerVariableConnection)
{
  const std::string kChannelName(kTestPrefix + "STRUCT4");
  const std::string server_var_name("server");
  const std::string client_var_name("client");
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  // creating PVServerVariableItem in the model
  auto server_item =
      m_model.InsertItem<PvAccessServerVariableItem>(m_workspace_item, mvvm::TagIndex::Append());
  server_item->SetChannel(kChannelName);
  server_item->SetName(server_var_name);
  SetAnyValue(initial_value, *server_item);

  // creating PVServerClientItem in the model
  auto client_item =
      m_model.InsertItem<PvAccessClientVariableItem>(m_workspace_item, mvvm::TagIndex::Append());
  client_item->SetChannel(kChannelName);
  client_item->SetName(client_var_name);
  SetAnyValue(initial_value, *client_item);

  EXPECT_FALSE(server_item->IsAvailable());
  EXPECT_FALSE(client_item->IsAvailable());

  // creating synchronizer
  auto synchronizer = CreateSynchronizer();

  mock_listener_t model_listener(&m_model);

  // expected events from client variable
  {
    auto expected_event1 = mvvm::DataChangedEvent{client_item->GetItem(itemconstants::kIsAvailable),
                                                  mvvm::DataRole::kData};
    EXPECT_CALL(model_listener, OnDataChanged(expected_event1)).Times(1);
  }

  // expected events from server variable
  {
    auto expected_event2 = mvvm::DataChangedEvent{server_item->GetItem(itemconstants::kIsAvailable),
                                                  mvvm::DataRole::kData};
    EXPECT_CALL(model_listener, OnDataChanged(expected_event2)).Times(1);
  }

  // Creating domain listener and setting callback expectations.
  test::MockDomainWorkspaceListener domain_listener(m_workspace);
  const sup::dto::AnyValue empty_value;
  {
    // This behavior coincides SequencerWorkspaceCornerCaseTest::PVAccessClientAndServerVariables
    // We check that WorkspaceSyncronizer doesn't change Workspace behavior.
    const ::testing::InSequence seq;
    const sup::dto::AnyValue empty_value;
    EXPECT_CALL(domain_listener, OnEvent(server_var_name, initial_value, true)).Times(1);
    EXPECT_CALL(domain_listener, OnEvent(client_var_name, empty_value, true)).Times(1);
    EXPECT_CALL(domain_listener, OnEvent(client_var_name, initial_value, true)).Times(1);
  }

  m_workspace.Setup();

  EXPECT_TRUE(m_workspace.WaitForVariable(server_var_name, 1.0));
  EXPECT_TRUE(m_workspace.WaitForVariable(client_var_name, 1.0));

  // queued connection toward the GUI requires special waiting with qWaitFor
  EXPECT_TRUE(QTest::qWaitFor([server_item]() { return server_item->IsAvailable(); }, 3000));
  EXPECT_TRUE(QTest::qWaitFor([client_item]() { return client_item->IsAvailable(); }, 3000));

  ASSERT_TRUE(client_item->GetAnyValueItem());  // client got new AnyValueItem
  EXPECT_TRUE(test::IsEqual(*client_item, initial_value));
  EXPECT_TRUE(test::IsEqual(*server_item, initial_value));
}

//! One server and one client variable in a workspace. The difference with the previous test is that
//! client doesn't have AnyValueItem defined. It is expected that it will pick up right value from
//! the server on the first connection.
TEST_F(WorkspaceSynchronizerPVAccessTest, ClientWithoutAnyValueAndServerVariableConnection)
{
  const std::string kChannelName(kTestPrefix + "STRUCT5");
  const std::string server_var_name("server");
  const std::string client_var_name("client");
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  // creating PVServerVariableItem in the model
  auto server_item =
      m_model.InsertItem<PvAccessServerVariableItem>(m_workspace_item, mvvm::TagIndex::Append());
  server_item->SetChannel(kChannelName);
  server_item->SetName(server_var_name);
  SetAnyValue(initial_value, *server_item);

  // creating PVServerClientItem in the model
  auto client_item =
      m_model.InsertItem<PvAccessClientVariableItem>(m_workspace_item, mvvm::TagIndex::Append());
  client_item->SetChannel(kChannelName);
  client_item->SetName(client_var_name);
  // we do not set AnyValue to client

  EXPECT_FALSE(server_item->IsAvailable());
  EXPECT_FALSE(client_item->IsAvailable());
  EXPECT_TRUE(test::IsEqual(*server_item, initial_value));

  // creating synchronizer (and underlying domain  workspace)
  auto synchronizer = CreateSynchronizer();

  mvvm::test::MockModelListener model_listener(&m_model);

  // expected events from client variable
  {
    auto expected_event1 = mvvm::DataChangedEvent{client_item->GetItem(itemconstants::kIsAvailable),
                                                  mvvm::DataRole::kData};
    EXPECT_CALL(model_listener, OnDataChanged(expected_event1)).Times(1);
    EXPECT_CALL(model_listener, OnAboutToInsertItem(::testing::_)).Times(1);
    EXPECT_CALL(model_listener, OnItemInserted(::testing::_)).Times(1);
  }

  // expected events from server variable
  {
    auto expected_event2 = mvvm::DataChangedEvent{server_item->GetItem(itemconstants::kIsAvailable),
                                                  mvvm::DataRole::kData};
    EXPECT_CALL(model_listener, OnDataChanged(expected_event2)).Times(1);
  }

  // Creating domain listener and setting callback expectations.
  test::MockDomainWorkspaceListener domain_listener(m_workspace);
  {
    // This behavior coincides SequencerWorkspaceCornerCaseTest::PVAccessClientAndServerVariables
    // We check that WorkspaceSyncronizer doesn't change Workspace behavior.
    const ::testing::InSequence seq;
    const sup::dto::AnyValue empty_value;
    EXPECT_CALL(domain_listener, OnEvent(server_var_name, initial_value, true)).Times(1);
    EXPECT_CALL(domain_listener, OnEvent(client_var_name, empty_value, true)).Times(1);
    EXPECT_CALL(domain_listener, OnEvent(client_var_name, initial_value, true)).Times(1);
  }

  m_workspace.Setup();

  EXPECT_TRUE(m_workspace.WaitForVariable(server_var_name, 1.0));
  EXPECT_TRUE(m_workspace.WaitForVariable(client_var_name, 1.0));

  // queued connection toward the GUI requires special waiting with qWaitFor
  EXPECT_TRUE(QTest::qWaitFor([server_item]() { return server_item->IsAvailable(); }, 3000));
  EXPECT_TRUE(QTest::qWaitFor([client_item]() { return client_item->IsAvailable(); }, 3000));

  ASSERT_TRUE(client_item->GetAnyValueItem());  // client got new AnyValueItem
  EXPECT_TRUE(test::IsEqual(*client_item, initial_value));
  EXPECT_TRUE(test::IsEqual(*server_item, initial_value));
}

}  // namespace oac_tree_gui::test
