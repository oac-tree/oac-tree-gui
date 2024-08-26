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

#include "sequencergui/pvmonitor/workspace_synchronizer.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/monitor_model.h>
#include <sequencergui/pvmonitor/workspace_monitor_helper.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/test/mock_model_listener.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <testutils/mock_domain_workspace_listener.h>

#include <QTest>

using namespace sequencergui;
using ::testing::_;

//! Tests for WorkspaceSynchronizer class.

class WorkspaceSynchronizerTest : public ::testing::Test
{
public:
  WorkspaceSynchronizerTest() { m_model.InsertItem<WorkspaceItem>(); }

  //! Helper function to create LocalVariableItem with given name and initial AnyValue.
  static std::unique_ptr<LocalVariableItem> CreateLocalVariableItem(
      const std::string& name, const sup::dto::AnyValue& initial_value)
  {
    auto result = std::make_unique<LocalVariableItem>();
    result->SetName(name);
    SetAnyValue(initial_value, *result);
    return result;
  }

  //! Creates synchronizer for testing.
  std::unique_ptr<WorkspaceSynchronizer> CreateSynchronizer()
  {
    // populate sequencer workspace so it match WorkspaceItem
    PopulateDomainWorkspace(*m_model.GetWorkspaceItem(), m_workspace);
    return std::make_unique<WorkspaceSynchronizer>(m_model.GetWorkspaceItem(), &m_workspace);
  }

  //! Creates synchronizer for testing. This mimics the way syncronizer is set in a JobHandler
  std::unique_ptr<WorkspaceSynchronizer> CreateSynchronizerJobHandlerCase()
  {
    // populate sequencer workspace so it match WorkspaceItem
    PopulateDomainWorkspace(*m_model.GetWorkspaceItem(), m_workspace);
    auto result = std::make_unique<WorkspaceSynchronizer>(&m_workspace);
    m_workspace.Setup();
    result->SetWorkspaceItem(m_model.GetWorkspaceItem());
    return result;
  }

  MonitorModel m_model;
  sup::sequencer::Workspace m_workspace;
};

TEST_F(WorkspaceSynchronizerTest, InitialState)
{
  auto synchronizer = CreateSynchronizer();
  EXPECT_EQ(synchronizer->GetWorkspace(), &m_workspace);
  EXPECT_EQ(synchronizer->GetWorkspaceItem(), m_model.GetWorkspaceItem());
  EXPECT_FALSE(synchronizer->HasStarted());
}

TEST_F(WorkspaceSynchronizerTest, AttemptToSynchronizeNonMatchingWorkspaces)
{
  {  // empty workspaces
    sup::sequencer::Workspace workspace;

    mvvm::ApplicationModel model;
    auto workspace_item = model.InsertItem<WorkspaceItem>();
    WorkspaceSynchronizer synchronizer(workspace_item, &workspace);
    EXPECT_THROW(synchronizer.Start(), LogicErrorException);
  }

  {  // variables do not match
    const sup::dto::AnyValue value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
    auto variable_item = m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
    variable_item->SetName("var0");
    SetAnyValue(value, *variable_item);

    auto synchronizer = CreateSynchronizer();

    // changing the name to make mismatch in workspaces
    variable_item->SetName("abc");

    EXPECT_THROW(synchronizer->Start(), LogicErrorException);
  }
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Validating initial values after synchronization was started.

TEST_F(WorkspaceSynchronizerTest, Start)
{
  const sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  m_model.GetWorkspaceItem()->InsertItem(CreateLocalVariableItem("abc", value0),
                                         mvvm::TagIndex::Append());

  auto synchronizer = CreateSynchronizer();

  synchronizer->Start();
  EXPECT_TRUE(synchronizer->HasStarted());

  ASSERT_TRUE(synchronizer->GetWorkspace() != nullptr);
  EXPECT_EQ(synchronizer->GetWorkspace()->GetVariables().size(), 1);
  ASSERT_TRUE(synchronizer->GetWorkspace()->HasVariable("abc"));

  auto domain_variable0 = synchronizer->GetWorkspace()->GetVariable("abc");
  EXPECT_EQ(domain_variable0->GetName(), "abc");

  sup::dto::AnyValue domain_value;
  EXPECT_TRUE(domain_variable0->GetValue(domain_value));
  EXPECT_EQ(domain_value, value0);

  // attempt to start twice
  EXPECT_THROW(synchronizer->Start(), sequencergui::LogicErrorException);
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Changing domain variable and checking that WorkspaceItem was properly updated.

TEST_F(WorkspaceSynchronizerTest, OnDomainVariableUpdated)
{
  const sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item0 =
      m_model.GetWorkspaceItem()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  variable_item0->SetName("abc");
  SetAnyValue(value0, *variable_item0);

  auto synchronizer = CreateSynchronizer();
  synchronizer->Start();

  // AnyValueItem on board should represent proper AnyValue
  ASSERT_TRUE(variable_item0->GetAnyValueItem() != nullptr);
  EXPECT_EQ(value0, GetAnyValue(*variable_item0));

  auto prev_anyvalue_item = variable_item0->GetAnyValueItem();

  // changing the value via domain workspace
  const sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  EXPECT_TRUE(synchronizer->GetWorkspace()->SetValue("abc", value1));

  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_EQ(variable_item0->GetAnyValueItem(), prev_anyvalue_item);
  EXPECT_EQ(value1, GetAnyValue(*variable_item0));
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Changing domain variable and checking that WorkspaceItem was properly updated.
//! The test repeats the test above, except that synchronizer is constructed differently.

TEST_F(WorkspaceSynchronizerTest, OnDomainVariableUpdatedHandlerCase)
{
  const sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item0 =
      m_model.GetWorkspaceItem()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  variable_item0->SetName("abc");
  SetAnyValue(value0, *variable_item0);

  auto synchronizer = CreateSynchronizerJobHandlerCase();  // the difference with previous test
  synchronizer->Start();

  // AnyValueItem on board should represent proper AnyValue
  ASSERT_TRUE(variable_item0->GetAnyValueItem() != nullptr);
  EXPECT_EQ(value0, GetAnyValue(*variable_item0));

  auto prev_anyvalue_item = variable_item0->GetAnyValueItem();

  // changing the value via domain workspace
  const sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  EXPECT_TRUE(synchronizer->GetWorkspace()->SetValue("abc", value1));

  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_EQ(variable_item0->GetAnyValueItem(), prev_anyvalue_item);
  EXPECT_EQ(value1, GetAnyValue(*variable_item0));
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Changing GUI variable and checking that domain was properly updated.

TEST_F(WorkspaceSynchronizerTest, OnModelVariableUpdate)
{
  const std::string var_name("abc");

  const sup::dto::AnyValue initial_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item =
      m_model.GetWorkspaceItem()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  variable_item->SetName(var_name);
  EXPECT_EQ(variable_item->GetAnyValueItem(), nullptr);

  SetAnyValue(initial_value, *variable_item);

  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);
  mvvm::test::MockModelListenerV2 model_listener(&m_model);

  auto synchronizer = CreateSynchronizer();

  // expecting to get IsAvailable notification update after the start
  EXPECT_CALL(domain_listener, OnEvent(var_name, initial_value, true)).Times(1);

  synchronizer->Start();

  // changing the value via the model
  const sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});

  // callback expectations
  EXPECT_CALL(domain_listener, OnEvent(var_name, new_value, true)).Times(1);
  // method SetAnyValue below will remove previous AnyValue and insert new item
  EXPECT_CALL(model_listener, OnAboutToRemoveItem(_)).Times(1);
  EXPECT_CALL(model_listener, OnItemRemoved(_)).Times(1);
  EXPECT_CALL(model_listener, OnAboutToInsertItem(_)).Times(1);
  EXPECT_CALL(model_listener, OnItemInserted(_)).Times(1);

  // no need to wait, domain is notified via direct connections
  SetAnyValue(new_value, *variable_item);

  auto domain_variable0 = synchronizer->GetWorkspace()->GetVariable(var_name);
  sup::dto::AnyValue domain_value;
  EXPECT_TRUE(domain_variable0->GetValue(domain_value));
  EXPECT_EQ(domain_value, new_value);
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Changing GUI variable and checking that domain was properly updated.
//! The test repeats the test above, except that synchronizer is constructed differently.

TEST_F(WorkspaceSynchronizerTest, OnModelVariableUpdateHandlerCase)
{
  const std::string var_name("abc");

  const sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item =
      m_model.GetWorkspaceItem()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  variable_item->SetName(var_name);
  EXPECT_EQ(variable_item->GetAnyValueItem(), nullptr);

  SetAnyValue(value0, *variable_item);

  auto synchronizer = CreateSynchronizerJobHandlerCase();
  synchronizer->Start();

  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);
  mvvm::test::MockModelListenerV2 model_listener(&m_model);

  // changing the value via the model
  const sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});

  // callback expectations
  EXPECT_CALL(domain_listener, OnEvent(var_name, new_value, true)).Times(1);
  // method SetAnyValue below will remove previous AnyValue and insert new item
  EXPECT_CALL(model_listener, OnAboutToRemoveItem(_)).Times(1);
  EXPECT_CALL(model_listener, OnItemRemoved(_)).Times(1);
  EXPECT_CALL(model_listener, OnAboutToInsertItem(_)).Times(1);
  EXPECT_CALL(model_listener, OnItemInserted(_)).Times(1);

  // no need to wait, domain is notified via direct connections
  SetAnyValue(new_value, *variable_item);

  auto domain_variable0 = synchronizer->GetWorkspace()->GetVariable(var_name);
  sup::dto::AnyValue domain_value;
  EXPECT_TRUE(domain_variable0->GetValue(domain_value));
  EXPECT_EQ(domain_value, new_value);
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Update domain variable and check signaling.

TEST_F(WorkspaceSynchronizerTest, UpdateDomainAndCheckSignals)
{
  const std::string var_name("abc");

  const sup::dto::AnyValue value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item =
      m_model.GetWorkspaceItem()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  variable_item->SetName(var_name);
  SetAnyValue(value, *variable_item);

  auto synchronizer = CreateSynchronizer();
  synchronizer->Start();

  mvvm::test::MockModelListenerV2 model_listener(&m_model);
  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);

  auto expected_event =
      mvvm::DataChangedEvent{variable_item->GetAnyValueItem(), mvvm::DataRole::kData};

  const sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});

  EXPECT_CALL(model_listener, OnDataChanged(expected_event)).Times(1);
  EXPECT_CALL(domain_listener, OnEvent(var_name, new_value, true)).Times(1);

  // changing the value via domain workspace
  EXPECT_TRUE(synchronizer->GetWorkspace()->SetValue(var_name, new_value));

  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_EQ(variable_item->GetAnyValueItem()->Data<int>(), 43);
}

//! Creating WorkspaceItem with one LocalVariableItem. Validating initial values and editable
//! properties after start and after shutdown.

// FIXME restore text. NameItem has been removed.

TEST_F(WorkspaceSynchronizerTest, DISABLED_StartAndShutdown)
{
  const sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto var_item = m_model.GetWorkspaceItem()->InsertItem(CreateLocalVariableItem("abc", value0),
                                                         mvvm::TagIndex::Append());

  auto synchronizer = CreateSynchronizer();

  EXPECT_TRUE(var_item->GetItem(domainconstants::kNameAttribute)->IsEditable());

  synchronizer->Start();
  EXPECT_TRUE(synchronizer->HasStarted());

  EXPECT_FALSE(var_item->GetItem(domainconstants::kNameAttribute)->IsEditable());

  ASSERT_TRUE(synchronizer->GetWorkspace() != nullptr);
  EXPECT_EQ(synchronizer->GetWorkspace()->GetVariables().size(), 1);
  ASSERT_TRUE(synchronizer->GetWorkspace()->HasVariable("abc"));

  auto domain_variable0 = synchronizer->GetWorkspace()->GetVariable("abc");
  EXPECT_EQ(domain_variable0->GetName(), "abc");

  sup::dto::AnyValue domain_value;
  EXPECT_TRUE(domain_variable0->GetValue(domain_value));
  EXPECT_EQ(domain_value, value0);

  synchronizer->Shutdown();
  EXPECT_TRUE(var_item->GetItem(domainconstants::kNameAttribute)->IsEditable());
}
