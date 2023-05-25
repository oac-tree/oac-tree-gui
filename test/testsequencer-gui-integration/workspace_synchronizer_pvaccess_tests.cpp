/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/pvmonitor/workspace_synchronizer.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/monitor_model.h>
#include <sequencergui/pvmonitor/workspace_monitor_helper.h>
#include <sequencergui/transform/transform_helpers.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/core/exceptions.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <testutils/mock_domain_workspace_listener.h>
#include <testutils/mock_model_listener.h>
#include <testutils/test_utils.h>

using namespace sequencergui;
using ::testing::_;

#include <QTest>

namespace
{
const std::string kTestPrefix("WorkspaceSynchronizerPVAccessTests:");
}  // namespace

//! Tests for WorkspaceSynchronizer class.

class WorkspaceSynchronizerPVAccessTests : public ::testing::Test
{
public:
  WorkspaceSynchronizerPVAccessTests() { m_model.InsertItem<WorkspaceItem>(); }

  //! Creates synchronizer for testing.
  std::unique_ptr<WorkspaceSynchronizer> CreateSynchronizer()
  {
    // populate sequencer workspace so it match WorkspaceItem
    PopulateDomainWorkspace(*m_model.GetWorkspaceItem(), m_workspace);
    return std::make_unique<WorkspaceSynchronizer>(m_model.GetWorkspaceItem(), &m_workspace);
  }

  //! Disables all tests in the fixture if PVAccess is not available
  void SetUp() override
  {
    if (!IsSequencerPluginEpicsAvailable())
    {
      GTEST_SKIP();
    }
  }

  MonitorModel m_model;
  sup::sequencer::Workspace m_workspace;
};

//! Single server variable (struct) in a workspace.
//! Creating variable on GUI side, duplicating in the domain workspace. Starting listening,
//! making sure that correct signals are flying and initial values coincide.

TEST_F(WorkspaceSynchronizerPVAccessTests, ServerVariableSimpleStart)
{
  const std::string kChannelName(kTestPrefix + "STRUCT1");
  const std::string var_name("var");
  sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  // creating PVServerVariableItem in the model
  auto variable_item =
      m_model.GetWorkspaceItem()->InsertItem<PvAccessServerVariableItem>(mvvm::TagIndex::Append());
  variable_item->SetChannel(kChannelName);
  variable_item->SetName(var_name);
  SetAnyValue(initial_value, *variable_item);

  EXPECT_FALSE(variable_item->IsAvailable());

  // creating synchronizer (and underlying domain  workspace)
  auto synchronizer = CreateSynchronizer();

  // Creating listeners and setting callback expectations.
  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);
  testutils::MockModelListener model_listener(&m_model);

  // After domain workspace was set-up, the only callback will be DataChangedEvent for IsAvailable
  // status, caused by Start method.

  auto is_available_property = variable_item->GetItem(itemconstants::kIsAvailable);
  auto expected_event =
      mvvm::event_variant_t(mvvm::DataChangedEvent{is_available_property, mvvm::DataRole::kData});

  EXPECT_CALL(model_listener, OnEvent(expected_event)).Times(1);
  EXPECT_CALL(domain_listener, OnEvent(_, _, _)).Times(0);

  synchronizer->Start();

  QTest::qWait(100);  // queued signals need special waiting

  EXPECT_TRUE(m_workspace.WaitForVariable(var_name, 1.0));

  // checking the value on domain side
  sup::dto::AnyValue domain_value;
  m_workspace.GetValue(var_name, domain_value);
  EXPECT_EQ(domain_value, initial_value);

  // checking the value on GUI side
  EXPECT_TRUE(variable_item->IsAvailable());
  auto gui_value = sup::gui::CreateAnyValue(*variable_item->GetAnyValueItem());
  EXPECT_EQ(gui_value, initial_value);
}

//! Single server variable (struct) in a workspace.
//! The data is set on the GUI side, validating updates on the domain side.

TEST_F(WorkspaceSynchronizerPVAccessTests, SetDataFromGUI)
{
  const std::string kChannelName(kTestPrefix + "STRUCT2");
  const std::string var_name("var");
  sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  // creating PVServerVariableItem in the model
  auto variable_item =
      m_model.GetWorkspaceItem()->InsertItem<PvAccessServerVariableItem>(mvvm::TagIndex::Append());
  variable_item->SetChannel(kChannelName);
  variable_item->SetName(var_name);
  SetAnyValue(initial_value, *variable_item);

  EXPECT_FALSE(variable_item->IsAvailable());

  // creating synchronizer (and underlying domain  workspace)
  auto synchronizer = CreateSynchronizer();
  synchronizer->Start();

  // Creating domain and setting callback expectations.
  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);
  auto anyvalue_item = variable_item->GetAnyValueItem();
  sup::dto::AnyValue expected_value({{"value", {sup::dto::SignedInteger32Type, 42}}});
  EXPECT_CALL(domain_listener, OnEvent(var_name, expected_value, true)).Times(1);

  // creating model listener and setting expectations
  testutils::MockModelListener model_listener(&m_model);
  auto scalar_field = anyvalue_item->GetChildren().at(0);
  auto expected_event =
      mvvm::event_variant_t(mvvm::DataChangedEvent{scalar_field, mvvm::DataRole::kData});
  EXPECT_CALL(model_listener, OnEvent(expected_event)).Times(1);

  // setting the data from GUI (will trigger expectation)
  scalar_field->SetData(42);

  // validating the data in the domain
  sup::dto::AnyValue domain_value;
  m_workspace.GetValue(var_name, domain_value);
  EXPECT_EQ(domain_value, expected_value);
}

//! Single server variable (struct) in a workspace.
//! The data is set on the domain side, validating updates on the GUI side.

TEST_F(WorkspaceSynchronizerPVAccessTests, SetDataFromDomain)
{
  const std::string kChannelName(kTestPrefix + "STRUCT3");
  const std::string var_name("var");
  sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  // creating PVServerVariableItem in the model
  auto variable_item =
      m_model.GetWorkspaceItem()->InsertItem<PvAccessServerVariableItem>(mvvm::TagIndex::Append());
  variable_item->SetChannel(kChannelName);
  variable_item->SetName(var_name);
  SetAnyValue(initial_value, *variable_item);

  EXPECT_FALSE(variable_item->IsAvailable());

  // creating synchronizer (and underlying domain  workspace)
  auto synchronizer = CreateSynchronizer();
  synchronizer->Start();

  //  // Creating domain and setting callback expectations.
  //  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);
  auto anyvalue_item = variable_item->GetAnyValueItem();
  sup::dto::AnyValue expected_value({{"value", {sup::dto::SignedInteger32Type, 42}}});
  //  EXPECT_CALL(domain_listener, OnEvent(var_name, expected_value, true)).Times(1);

  // creating model listener and setting expectations
  testutils::MockModelListener model_listener(&m_model);
  auto scalar_field = anyvalue_item->GetChildren().at(0);
  auto expected_event =
      mvvm::event_variant_t(mvvm::DataChangedEvent{scalar_field, mvvm::DataRole::kData});
  EXPECT_CALL(model_listener, OnEvent(expected_event)).Times(1);

  // setting the data from the domain (will trigger expectation)
  EXPECT_TRUE(m_workspace.SetValue(var_name, expected_value));

  QTest::qWait(50);  // queued signals toward the GUI needs special waiting

  // validating the data in the domain
  sup::dto::AnyValue domain_value;
  m_workspace.GetValue(var_name, domain_value);
  EXPECT_EQ(domain_value, expected_value);

  // validating the data in the GUI
  auto gui_anyvalue = sup::gui::CreateAnyValue(*anyvalue_item);
  EXPECT_EQ(gui_anyvalue, expected_value);
}

//! One server and one client variable (struct) in a workspace.
//! Creating variables on a GUI side, duplicating in the domain workspace. Starting listening,
//! making sure that correct signals are flying and initial values coincide.

TEST_F(WorkspaceSynchronizerPVAccessTests, ClientAndServerVariableConnection)
{
  const std::string kChannelName(kTestPrefix + "STRUCT4");
  const std::string server_var_name("server");
  const std::string client_var_name("client");
  sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  // creating PVServerVariableItem in the model
  auto server_item =
      m_model.GetWorkspaceItem()->InsertItem<PvAccessServerVariableItem>(mvvm::TagIndex::Append());
  server_item->SetChannel(kChannelName);
  server_item->SetName(server_var_name);
  SetAnyValue(initial_value, *server_item);

  // creating PVServerClientItem in the model
  auto client_item =
      m_model.GetWorkspaceItem()->InsertItem<PvAccessClientVariableItem>(mvvm::TagIndex::Append());
  client_item->SetChannel(kChannelName);
  client_item->SetName(client_var_name);
  SetAnyValue(initial_value, *client_item);

  EXPECT_FALSE(server_item->IsAvailable());
  EXPECT_FALSE(client_item->IsAvailable());
  EXPECT_EQ(sup::gui::CreateAnyValue(*client_item->GetAnyValueItem()), initial_value);
  EXPECT_EQ(sup::gui::CreateAnyValue(*server_item->GetAnyValueItem()), initial_value);

  // creating synchronizer (and underlying domain  workspace)
  auto synchronizer = CreateSynchronizer();

  testutils::MockModelListener model_listener(&m_model);

  {
    ::testing::InSequence seq;
    auto expected_event1 =
        mvvm::DataChangedEvent{client_item->GetItem(itemconstants::kIsAvailable), mvvm::DataRole::kData};
    EXPECT_CALL(model_listener, OnEvent(mvvm::event_variant_t(expected_event1))).Times(1);

    auto expected_event2 =
        mvvm::DataChangedEvent{server_item->GetItem(itemconstants::kIsAvailable), mvvm::DataRole::kData};
    EXPECT_CALL(model_listener, OnEvent(mvvm::event_variant_t(expected_event2))).Times(1);
  }

  // Creating domain listener and setting callback expectations.
  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);
  {
    ::testing::InSequence seq;
    sup::dto::AnyValue empty_value;
    EXPECT_CALL(domain_listener, OnEvent(client_var_name, empty_value, true)).Times(1);
    EXPECT_CALL(domain_listener, OnEvent(client_var_name, initial_value, true)).Times(1);
  }

  synchronizer->Start();

  EXPECT_TRUE(m_workspace.WaitForVariable(server_var_name, 1.0));
  EXPECT_TRUE(m_workspace.WaitForVariable(client_var_name, 1.0));

  // queued connection toward the GUI requires special waiting with qWaitFor
  EXPECT_TRUE(QTest::qWaitFor([server_item]() { return server_item->IsAvailable(); }, 3000));
  EXPECT_TRUE(QTest::qWaitFor([client_item]() { return client_item->IsAvailable(); }, 3000));

  EXPECT_EQ(sup::gui::CreateAnyValue(*client_item->GetAnyValueItem()), initial_value);
  EXPECT_EQ(sup::gui::CreateAnyValue(*server_item->GetAnyValueItem()), initial_value);
}
