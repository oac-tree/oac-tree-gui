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

#include "sequencergui/monitor/workspace_synchronizer.h"

#include <gtest/gtest.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/variable_transform_helper.h>
#include <sequencergui/monitor/monitor_model.h>
#include <sequencergui/monitor/workspace_monitor_helper.h>
#include <testutils/gui_domain_utils.h>
#include <testutils/mock_domain_workspace_listener.h>
#include <testutils/mock_model_listener.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/core/exceptions.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/sequencer/workspace.h>

#include <QTest>

using namespace sequencergui;
using ::testing::_;

//! Tests for WorkspaceSyncronizer class.

class WorkspaceSynchronizerTests : public ::testing::Test
{
public:
  WorkspaceSynchronizerTests() { m_model.InsertItem<WorkspaceItem>(); }

  //! Helper function to create LocalVariableItem with given name and initial AnyValue.
  static std::unique_ptr<LocalVariableItem> CreateLocalVariable(
      const std::string& name, const sup::dto::AnyValue& initial_value)
  {
    auto result = std::make_unique<LocalVariableItem>();
    testutils::SetupVariable(name, initial_value, *result.get());
    return result;
  }

  //! Creates syncronizer for testing.
  std::unique_ptr<WorkspaceSynchronizer> CreateSynchronizer()
  {
    // populate sequencer workspace so it match WorkspaceItem
    PopulateDomainWorkspace(*m_model.GetWorkspaceItem(), m_workspace);
    m_workspace.Setup();  // current convention: has to be setup before starting listening
    return std::make_unique<WorkspaceSynchronizer>(m_model.GetWorkspaceItem(), &m_workspace);
  }

  MonitorModel m_model;
  sup::sequencer::Workspace m_workspace;
};

TEST_F(WorkspaceSynchronizerTests, InitialState)
{
  auto synchronizer = CreateSynchronizer();
  EXPECT_EQ(synchronizer->GetWorkspace(), &m_workspace);
}

TEST_F(WorkspaceSynchronizerTests, AttemptToSyncronizeNonMatchingWorkspaces)
{
  {  // empty workspaces
    sup::sequencer::Workspace workspace;

    mvvm::ApplicationModel model;
    auto workspace_item = model.InsertItem<WorkspaceItem>();
    WorkspaceSynchronizer syncronizer(workspace_item, &workspace);
    EXPECT_THROW(syncronizer.Start(), sup::gui::LogicErrorException);
  }

  {  // variables do not match
    const sup::dto::AnyValue value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
    auto variable_item =
        m_model.InsertItem<LocalVariableItem>(m_model.GetWorkspaceItem());
    variable_item->SetName("var0");

    auto synchronizer = CreateSynchronizer();

    // changing the name to make mismatch in workspaces
    variable_item->SetName("abc");

    EXPECT_THROW(synchronizer->Start(), sup::gui::LogicErrorException);
  }
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Validating initial values after syncronization was started.

TEST_F(WorkspaceSynchronizerTests, Start)
{
  const sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item0 = m_model.GetWorkspaceItem()->InsertItem(CreateLocalVariable("abc", value0),
                                                               mvvm::TagIndex::Append());

  auto synchronizer = CreateSynchronizer();
  synchronizer->Start();

  ASSERT_TRUE(synchronizer->GetWorkspace() != nullptr);
  EXPECT_EQ(synchronizer->GetWorkspace()->GetVariables().size(), 1);
  ASSERT_TRUE(synchronizer->GetWorkspace()->HasVariable("abc"));

  auto domain_variable0 = synchronizer->GetWorkspace()->GetVariable("abc");
  EXPECT_EQ(domain_variable0->GetName(), "abc");

  sup::dto::AnyValue domain_value;
  EXPECT_TRUE(domain_variable0->GetValue(domain_value));
  EXPECT_EQ(domain_value, value0);
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Changing domain variable and checking that WorkspaceItem was properly updated.

TEST_F(WorkspaceSynchronizerTests, OnDomainVariableUpdated)
{
  const sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item0 = m_model.GetWorkspaceItem()->InsertItem<LocalVariableItem>(
      mvvm::TagIndex::Append());
  testutils::SetupVariable("abc", value0, *variable_item0);
  EXPECT_EQ(variable_item0->GetAnyValueItem(), nullptr);

  auto synchronizer = CreateSynchronizer();
  synchronizer->Start();

  // AnyValueItem on board should represent proper AnyValue
  ASSERT_TRUE(variable_item0->GetAnyValueItem() != nullptr);
  auto stored_anyvalue0 = sup::gui::CreateAnyValue(*variable_item0->GetAnyValueItem());
  EXPECT_EQ(value0, stored_anyvalue0);

  auto prev_anyvalue_item = variable_item0->GetAnyValueItem();

  // changing the value via domain workspace
  const sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  EXPECT_TRUE(synchronizer->GetWorkspace()->SetValue("abc", value1));

  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_EQ(variable_item0->GetAnyValueItem(), prev_anyvalue_item);
  auto stored_anyvalue1 = sup::gui::CreateAnyValue(*variable_item0->GetAnyValueItem());
  EXPECT_EQ(value1, stored_anyvalue1);
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Changing GUI variable and checking that domain was properly updated.

TEST_F(WorkspaceSynchronizerTests, OnModelVariableUpdate)
{
  const std::string var_name("abc");

  const sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item = m_model.GetWorkspaceItem()->InsertItem<LocalVariableItem>(
      mvvm::TagIndex::Append());
  testutils::SetupVariable(var_name, value0, *variable_item);
  EXPECT_EQ(variable_item->GetAnyValueItem(), nullptr);
  SetAnyValue(value0, *variable_item);

  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);
  testutils::MockModelListener model_listener(&m_model);

  auto synchronizer = CreateSynchronizer();
  synchronizer->Start();

  // changing the value via the model
  const sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});

  // callback expectations
  EXPECT_CALL(domain_listener, OnEvent(var_name, new_value, true)).Times(1);
  // method SetAnyValue below will remove previous AnyValue and insert new item, 4 calls
  // correspond to AboutToRemoveEvent, ItemRemovedEvent, AboutToInsertEvent, ItemInsertedEvent
  EXPECT_CALL(model_listener, OnEvent(_)).Times(4);

  // no need to wait, domain is notified via direct connections
  SetAnyValue(new_value, *variable_item);

  auto domain_variable0 = synchronizer->GetWorkspace()->GetVariable(var_name);
  sup::dto::AnyValue domain_value;
  EXPECT_TRUE(domain_variable0->GetValue(domain_value));
  EXPECT_EQ(domain_value, new_value);
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Update domain variable and check signaling.

TEST_F(WorkspaceSynchronizerTests, UpdateDomainAndCheckSignals)
{
  const std::string var_name("abc");

  sup::dto::AnyValue value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  auto variable_item = m_model.GetWorkspaceItem()->InsertItem<LocalVariableItem>(
      mvvm::TagIndex::Append());
  variable_item->SetName(var_name);
  SetAnyValue(value, *variable_item);

  auto synchronizer = CreateSynchronizer();
  synchronizer->Start();

  testutils::MockModelListener model_listener(&m_model);
  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);

  auto expected_event = mvvm::event_variant_t(
      mvvm::DataChangedEvent{variable_item->GetAnyValueItem(), mvvm::DataRole::kData});

  const sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});

  EXPECT_CALL(model_listener, OnEvent(expected_event)).Times(1);
  EXPECT_CALL(domain_listener, OnEvent(var_name, new_value, true)).Times(1);

  // changing the value via domain workspace
  EXPECT_TRUE(synchronizer->GetWorkspace()->SetValue(var_name, new_value));

  // We are testing here queued signals, need special waiting
  QTest::qWait(100);

  EXPECT_EQ(variable_item->GetAnyValueItem()->Data<int>(), 43);
}
