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
  using mock_listener_t = ::testing::StrictMock<mvvm::test::MockModelListenerV2>;

  WorkspaceSynchronizerTest() { m_workspace_item = m_model.InsertItem<WorkspaceItem>(); }

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
    PopulateDomainWorkspace(*m_workspace_item, m_workspace);
    return std::make_unique<WorkspaceSynchronizer>(m_workspace_item, &m_workspace);
  }

  MonitorModel m_model;
  WorkspaceItem* m_workspace_item{nullptr};
  sup::sequencer::Workspace m_workspace;
};

//! Creating WorkspaceItem with one LocalVariableItem. Validating initial values after
//! synchronization was started.
TEST_F(WorkspaceSynchronizerTest, InitialState)
{
  const std::string var_name("var");
  const sup::dto::AnyValue initial_value(sup::dto::SignedInteger32Type, 42);

  auto variable_item = CreateLocalVariableItem(var_name, initial_value);
  auto variable_item_ptr = variable_item.get();
  m_model.InsertItem(std::move(variable_item), m_workspace_item, mvvm::TagIndex::Append());

  mock_listener_t model_listener(&m_model);

  auto synchronizer = CreateSynchronizer();
  m_workspace.Setup();

  auto empty_queue_predicate = [&synchronizer]() { return synchronizer->IsEmptyQueue(); };
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));

  EXPECT_EQ(GetAnyValue(*variable_item_ptr), initial_value);
  EXPECT_EQ(GetAnyValue(var_name, m_workspace), initial_value);

  // no signals from mock model listener expected here, since initial values of AnyValue coincide
  // for both workspaces
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Changing domain variable and checking that WorkspaceItem was properly updated.
TEST_F(WorkspaceSynchronizerTest, OnDomainVariableUpdated)
{
  const std::string var_name("var");
  const sup::dto::AnyValue initial_value(sup::dto::SignedInteger32Type, 42);

  auto variable_item = CreateLocalVariableItem(var_name, initial_value);
  auto variable_item_ptr = variable_item.get();
  m_model.InsertItem(std::move(variable_item), m_workspace_item, mvvm::TagIndex::Append());

  mock_listener_t model_listener(&m_model);

  auto synchronizer = CreateSynchronizer();
  m_workspace.Setup();

  auto empty_queue_predicate = [&synchronizer]() { return synchronizer->IsEmptyQueue(); };
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));

  EXPECT_EQ(GetAnyValue(*variable_item_ptr), initial_value);
  EXPECT_EQ(GetAnyValue(var_name, m_workspace), initial_value);

  auto prev_anyvalue_item = variable_item_ptr->GetAnyValueItem();

  // preparing expectation for signals from the model
  auto expected_event =
      mvvm::DataChangedEvent{variable_item_ptr->GetAnyValueItem(), mvvm::DataRole::kData};
  EXPECT_CALL(model_listener, OnDataChanged(expected_event)).Times(1);

  // changing the value via domain workspace
  const sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 43);
  EXPECT_TRUE(m_workspace.SetValue(var_name, new_value));

  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));

  EXPECT_EQ(variable_item_ptr->GetAnyValueItem(), prev_anyvalue_item);
  EXPECT_EQ(GetAnyValue(*variable_item_ptr), new_value);
}

//! Creating WorkspaceItem with one LocalVariableItem.
//! Changing GUI variable and checking that domain was properly updated.
TEST_F(WorkspaceSynchronizerTest, OnModelVariableUpdate)
{
  const std::string var_name("var");
  const sup::dto::AnyValue initial_value(sup::dto::SignedInteger32Type, 42);

  auto variable_item = CreateLocalVariableItem(var_name, initial_value);
  auto variable_item_ptr = variable_item.get();
  m_model.InsertItem(std::move(variable_item), m_workspace_item, mvvm::TagIndex::Append());

  auto synchronizer = CreateSynchronizer();
  m_workspace.Setup();

  auto empty_queue_predicate = [&synchronizer]() { return synchronizer->IsEmptyQueue(); };
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));

  mock_listener_t model_listener(&m_model);
  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);

  // changing the value via the model
  const sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 43);

  // callback expectations
  EXPECT_CALL(domain_listener, OnEvent(var_name, new_value, true)).Times(1);
  EXPECT_CALL(model_listener, OnAboutToRemoveItem(_)).Times(1);
  EXPECT_CALL(model_listener, OnItemRemoved(_)).Times(1);
  EXPECT_CALL(model_listener, OnAboutToInsertItem(_)).Times(1);
  EXPECT_CALL(model_listener, OnItemInserted(_)).Times(1);

  // no need to wait, domain is notified via direct connections
  SetAnyValue(new_value, *variable_item_ptr); // will replace AnyValueItem as a whole

  EXPECT_EQ(GetAnyValue(*variable_item_ptr), new_value);
  EXPECT_EQ(GetAnyValue(var_name, m_workspace), new_value);
}

//! Creating WorkspaceItem with one LocalVariableItem. Validating initial values and editable
//! properties after start and after shutdown.

// FIXME restore text. NameItem has been removed.

// TEST_F(WorkspaceSynchronizerTest, DISABLED_StartAndShutdown)
// {
//   const sup::dto::AnyValue value0(sup::dto::SignedInteger32Type, 42);

//   auto var_item = m_model.GetWorkspaceItem()->InsertItem(CreateLocalVariableItem("abc", value0),
//                                                          mvvm::TagIndex::Append());

//   auto synchronizer = CreateSynchronizer();

//   EXPECT_TRUE(var_item->GetItem(domainconstants::kNameAttribute)->IsEditable());

//   synchronizer->Start();
//   EXPECT_TRUE(synchronizer->HasStarted());

//   EXPECT_FALSE(var_item->GetItem(domainconstants::kNameAttribute)->IsEditable());

//   ASSERT_TRUE(synchronizer->GetWorkspace() != nullptr);
//   EXPECT_EQ(synchronizer->GetWorkspace()->GetVariables().size(), 1);
//   ASSERT_TRUE(synchronizer->GetWorkspace()->HasVariable("abc"));

//   auto domain_variable0 = synchronizer->GetWorkspace()->GetVariable("abc");
//   EXPECT_EQ(domain_variable0->GetName(), "abc");

//   sup::dto::AnyValue domain_value;
//   EXPECT_TRUE(domain_variable0->GetValue(domain_value));
//   EXPECT_EQ(domain_value, value0);

//   synchronizer->Shutdown();
//   EXPECT_TRUE(var_item->GetItem(domainconstants::kNameAttribute)->IsEditable());
// }
