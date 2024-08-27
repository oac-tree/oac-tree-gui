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

#include "sequencergui/pvmonitor/sequencer_workspace_listener.h"
#include "sequencergui/pvmonitor/sequencer_workspace_listener_v2.h"

#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/jobsystem/domain_events.h>
#include <sequencergui/model/sequencer_item_helper.h>
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
#include <sup/dto/anyvalue_helper.h>
#include <sup/epics-test/softioc_runner.h>
#include <sup/epics-test/unit_test_helper.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

#include <QSignalSpy>
#include <QTest>

using namespace sequencergui;
using testing::_;

namespace
{
const std::string kChannelName("CA-TESTS:INT");
}

//! Tests for WorkspaceController class.
//! Sequencer workspace is populated with ChannelAccessVariable.

class SequencerWorkspaceListenerSoftIocTest : public ::testing::Test
{
public:
  using mock_listener_t = ::testing::StrictMock<mvvm::test::MockModelListenerV2>;

  SequencerWorkspaceListenerSoftIocTest()
  {
    m_workspace_item = m_model.InsertItem<WorkspaceItem>();
  }

  //! Disables all tests in the fixture if ChannelAccess is not available
  void SetUp() override
  {
    if (!IsSequencerPluginEpicsAvailable())
    {
      GTEST_SKIP();
    }
  }

  static void SetUpTestSuite()
  {
    if (IsSequencerPluginEpicsAvailable())
    {
      m_softioc_service.Start(testutils::GetEpicsDBContentString());
    }
  }

  static void TearDownTestSuite()
  {
    if (IsSequencerPluginEpicsAvailable())
    {
      m_softioc_service.Stop();
    }
  }

  static sup::epics::test::SoftIocRunner m_softioc_service;

  sup::sequencer::Workspace m_workspace;
  WorkspaceItem *m_workspace_item{nullptr};
  mvvm::ApplicationModel m_model;
};

sup::epics::test::SoftIocRunner SequencerWorkspaceListenerSoftIocTest::m_softioc_service{};

//! The sequencer Workspace contains a single channel access variable.
//! Validating that SequencerWorkspaceListener gets notifications.

// TEST_F(SequencerWorkspaceListenerSoftIocTest, ListeningWorkspaceWithSingleCAVariable)
// {
//   // creating ChannelAccessVariable
//   auto variable = CreateDomainVariable(domainconstants::kChannelAccessVariableType);
//   variable->AddAttribute("channel", kChannelName);
//   variable->AddAttribute("type", R"RAW({"type":"uint32"})RAW");

//   // adding it to the workspace and validating values
//   sup::sequencer::Workspace workspace;

//   EXPECT_TRUE(workspace.AddVariable("var", std::move(variable)));
//   EXPECT_NO_THROW(workspace.Setup());
//   EXPECT_TRUE(workspace.WaitForVariable("var", 5.0));
//   sup::dto::AnyValue value;
//   ASSERT_TRUE(workspace.GetValue("var", value));
//   EXPECT_EQ(value.GetType(), sup::dto::UnsignedInteger32Type);

//   SequencerWorkspaceListener listener(&workspace);
//   QSignalSpy spy_upate(&listener, &SequencerWorkspaceListener::VariabledUpdated);
//   listener.StartListening();

//   // attaching listener to the workspace
//   EXPECT_EQ(spy_upate.count(), 0);

//   // changing the value and waiting for update
//   value = 42;
//   EXPECT_TRUE(workspace.SetValue("var", value));
//   auto worker = [&workspace]()
//   {
//     sup::dto::AnyValue tmp;
//     return workspace.GetValue("var", tmp) && tmp.As<sup::dto::uint32>() == 42;
//   };
//   EXPECT_TRUE(sup::epics::test::BusyWaitFor(2.0, worker));

//   EXPECT_EQ(spy_upate.count(), 1);

//   // checking accumulated event
//   sup::dto::AnyValue expected_value(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 42});
//   EXPECT_EQ(listener.GetEventCount(), 1);
//   auto event = listener.PopEvent();
//   EXPECT_EQ(event.variable_name, std::string("var"));
//   EXPECT_EQ(event.value, expected_value);
//   EXPECT_TRUE(event.connected);
// }

TEST_F(SequencerWorkspaceListenerSoftIocTest, ListeningWorkspaceWithSingleCAVariableV2)
{
  const std::string var_name("abc");

  // creating ChannelAccessVariable
  auto variable_item = m_model.InsertItem<ChannelAccessVariableItem>(m_workspace_item);
  variable_item->SetName(var_name);
  variable_item->SetChannel(kChannelName);

  const sup::dto::AnyValue initial_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  SetAnyValue(initial_value, *variable_item);
  PopulateDomainWorkspace(*m_workspace_item, m_workspace);

  const sup::dto::AnyValue softioc_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 1});

  SequencerWorkspaceListenerV2 listener(m_workspace_item, &m_workspace);
  EXPECT_EQ(listener.GetEventCount(), 0);

  listener.StartListening();

  EXPECT_FALSE(variable_item->IsAvailable());

  // setting up expectations for the model
  mock_listener_t model_listener(&m_model);

  // Connection to SoftIoc generates two callbacks. The first callback contains the value=0, and
  // connection=1 and it is triggered by the connection status change. The second callback contains
  // value=1 and connection=1 and it is triggered by initial value in epics.db. This creates two
  // events in SequencerWorkspaceListenerV2 queue.

  // Propagation of these two events to the model will generate 3 model events: one related to
  // connection status change, and two related to values change.

  // expecting DataChangedEvent for item representing connected attribute
  auto expected_event1 =
      mvvm::DataChangedEvent{GetIsAvailableItem(*variable_item), mvvm::DataRole::kData};
  EXPECT_CALL(model_listener, OnDataChanged(expected_event1)).Times(1);

  // expecting two events corresponding to value=0, and value=1
  auto expected_event2 =
      mvvm::DataChangedEvent{variable_item->GetAnyValueItem(), mvvm::DataRole::kData};
  EXPECT_CALL(model_listener, OnDataChanged(expected_event2)).Times(2);

  // triggering all expectations
  m_workspace.Setup();

  // checking current server variable
  EXPECT_TRUE(m_workspace.WaitForVariable(var_name, 5.0));

  EXPECT_EQ(GetAnyValue(var_name, m_workspace), softioc_value);

  EXPECT_EQ(listener.GetEventCount(), 2);

  // let event loop do its job
  auto empty_queue_predicate = [&listener]() { return listener.GetEventCount() == 0; };
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));

  EXPECT_EQ(listener.GetEventCount(), 0);

  // setting expectation for second value change
  EXPECT_CALL(model_listener, OnDataChanged(expected_event2)).Times(1);

  // changing the value and waiting for update
  const sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});

  EXPECT_TRUE(m_workspace.SetValue(var_name, new_value));
  auto worker = [this, var_name]()
  {
    sup::dto::AnyValue tmp;
    return m_workspace.GetValue(var_name, tmp) && tmp.As<sup::dto::int32>() == 43;
  };
  EXPECT_TRUE(sup::epics::test::BusyWaitFor(2.0, worker));

  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));
  EXPECT_EQ(GetAnyValue(*variable_item), new_value);
}
