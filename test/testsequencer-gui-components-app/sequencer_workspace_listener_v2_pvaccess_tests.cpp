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

#include "sequencergui/pvmonitor/sequencer_workspace_listener_v2.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/workspace_monitor_helper.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>
#include <mvvm/test/mock_model_listener.h>

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/epics-test/unit_test_helper.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

#include <QTest>

namespace
{
const std::string kTestPrefix("SequencerWorkspaceListenerV2PVAccessTest:");
const std::string kScalarChannelName(kTestPrefix + "scalar");
const std::string kStructChannelName(kTestPrefix + "STRUCT");
}  // namespace

using namespace sequencergui;
using testing::_;

//! Tests for SequencerWorkspaceListenerV2 class.
class SequencerWorkspaceListenerV2PVAccessTest : public ::testing::Test
{
public:
  using mock_listener_t = ::testing::StrictMock<mvvm::test::MockModelListenerV2>;

  SequencerWorkspaceListenerV2PVAccessTest()
  {
    m_workspace_item = m_model.InsertItem<WorkspaceItem>();
  }

  //! Disables all tests in the fixture if PVAccess is not available
  void SetUp() override
  {
    if (!IsSequencerPluginEpicsAvailable())
    {
      GTEST_SKIP();
    }
  }

  sup::sequencer::Workspace m_workspace;
  WorkspaceItem *m_workspace_item{nullptr};
  mvvm::ApplicationModel m_model;
};

//! Single scalar variable in a workspace.
TEST_F(SequencerWorkspaceListenerV2PVAccessTest, WorkspaceWithSingleServerScalarVariable)
{
  const std::string var_name("abc");

  // creating VariableItem and populating domain workspace
  auto variable_item = m_model.InsertItem<PvAccessServerVariableItem>(m_workspace_item);
  variable_item->SetName(var_name);
  variable_item->SetChannel(kScalarChannelName);
  sup::dto::AnyValue initial_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  SetAnyValue(initial_value, *variable_item);
  PopulateDomainWorkspace(*m_workspace_item, m_workspace);

  // sup::dto::PrintAnyValue(event.value);

  SequencerWorkspaceListenerV2 listener(m_workspace_item, &m_workspace);
  EXPECT_EQ(listener.GetEventCount(), 0);

  listener.StartListening();

  EXPECT_FALSE(variable_item->IsAvailable());

  // setting up expectations for the model
  mock_listener_t model_listener(&m_model);

  // expecting DataChangedEvent for item representing connected attribute
  auto expected_event =
      mvvm::DataChangedEvent{GetIsAvailableItem(*variable_item), mvvm::DataRole::kData};
  EXPECT_CALL(model_listener, OnDataChanged(expected_event)).Times(1);

  // Due to the way we store a scalar inside struct, original AnyValueItem will be overwritten,
  // with new AnyValueItem
  // expecting 4 signals because of regeneration of AnyValueItem
  EXPECT_CALL(model_listener, OnAboutToRemoveItem(_)).Times(1);
  EXPECT_CALL(model_listener, OnItemRemoved(_)).Times(1);
  EXPECT_CALL(model_listener, OnAboutToInsertItem(_)).Times(1);
  EXPECT_CALL(model_listener, OnItemInserted(_)).Times(1);

  // triggering all expectations
  m_workspace.Setup();

  // This is an event from domain that variable has changed. It is not processed yet by the
  // workspace listener, since connection is queued.
  EXPECT_EQ(listener.GetEventCount(), 1);

  // let event loop do its job
  auto empty_queue_predicate = [&listener]() { return listener.GetEventCount() == 0; };
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));

  EXPECT_EQ(listener.GetEventCount(), 0);

  // checking current server variable
  EXPECT_TRUE(m_workspace.WaitForVariable(var_name, 5.0));
  sup::dto::AnyValue value;
  ASSERT_TRUE(m_workspace.GetValue(var_name, value));
  EXPECT_TRUE(m_workspace.GetVariable(var_name)->IsAvailable());
  EXPECT_EQ(value, initial_value);

  // // checking current value of AnyValueItem (a struct instead of scalar
  // const sup::dto::AnyValue expected_initial_value({{"value", {sup::dto::SignedInteger32Type,
  // 42}}}); EXPECT_EQ(GetAnyValue(*variable_item), expected_initial_value);

  // // changing the value and waiting for update
  // value = 43;
  // EXPECT_TRUE(m_workspace.SetValue(var_name, value));
  // auto worker = [this, var_name]()
  // {
  //   sup::dto::AnyValue tmp;
  //   return m_workspace.GetValue(var_name, tmp) && tmp.As<sup::dto::int32>() == 43;
  // };
  // EXPECT_TRUE(sup::epics::test::BusyWaitFor(2.0, worker));

  // const sup::dto::AnyValue new_anyvalue({{"value", {sup::dto::SignedInteger32Type, 43}}});
  // EXPECT_EQ(GetAnyValue(*variable_item), expected_initial_value);

  // EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));
}

//! Single struct variable in a workspace.
TEST_F(SequencerWorkspaceListenerV2PVAccessTest, WorkspaceWithSingleServerStructVariable)
{
  const std::string var_name("abc");

  // creating VariableItem and populating domain workspace
  auto variable_item = m_model.InsertItem<PvAccessServerVariableItem>(m_workspace_item);
  variable_item->SetName(var_name);
  variable_item->SetChannel(kStructChannelName);
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});
  SetAnyValue(initial_value, *variable_item);
  PopulateDomainWorkspace(*m_workspace_item, m_workspace);

  // sup::dto::PrintAnyValue(event.value);

  SequencerWorkspaceListenerV2 listener(m_workspace_item, &m_workspace);
  EXPECT_EQ(listener.GetEventCount(), 0);

  listener.StartListening();

  EXPECT_FALSE(variable_item->IsAvailable());

  // setting up expectations for the model
  mock_listener_t model_listener(&m_model);

  // expecting DataChangedEvent for item representing connected attribute
  auto expected_event =
      mvvm::DataChangedEvent{GetIsAvailableItem(*variable_item), mvvm::DataRole::kData};
  EXPECT_CALL(model_listener, OnDataChanged(expected_event)).Times(1);

  // triggering all expectations
  m_workspace.Setup();

  // This is an event from domain that variable has changed. It is not processed yet by the
  // workspace listener, since connection is queued.
  EXPECT_EQ(listener.GetEventCount(), 1);

  // let event loop do its job
  auto empty_queue_predicate = [&listener]() { return listener.GetEventCount() == 0; };
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));

  EXPECT_EQ(listener.GetEventCount(), 0);

  // checking current server variable
  EXPECT_TRUE(m_workspace.WaitForVariable(var_name, 5.0));
  sup::dto::AnyValue value;
  ASSERT_TRUE(m_workspace.GetValue(var_name, value));
  EXPECT_TRUE(m_workspace.GetVariable(var_name)->IsAvailable());
  EXPECT_EQ(value, initial_value);

  // setting up expectation for the data change below
  auto scalar_item =
      variable_item->GetAnyValueItem()->GetChildren().at(0);  // first child inside struct
  auto expected_event2 = mvvm::DataChangedEvent{scalar_item, mvvm::DataRole::kData};
  EXPECT_CALL(model_listener, OnDataChanged(expected_event2)).Times(1);

  // changing the value and waiting for update
  value["value"] = 42;
  EXPECT_TRUE(m_workspace.SetValue(var_name, value));
  auto worker = [this, var_name]()
  {
    sup::dto::AnyValue tmp;
    return m_workspace.GetValue(var_name, tmp) && tmp["value"].As<sup::dto::int32>() == 42;
  };
  EXPECT_TRUE(sup::epics::test::BusyWaitFor(2.0, worker));

  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));

  const sup::dto::AnyValue new_anyvalue({{"value", {sup::dto::SignedInteger32Type, 42}}});
  EXPECT_EQ(GetAnyValue(*variable_item), new_anyvalue);
}
