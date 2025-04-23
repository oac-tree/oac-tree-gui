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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/jobsystem/domain_events.h>

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/epics-test/unit_test_helper.h>
#include <sup/oac-tree/workspace.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

#include <QTest>

#include "oac_tree_gui/pvmonitor/domain_workspace_listener.h"

using testing::_;

namespace oac_tree_gui::test
{

namespace
{
// the name defined in sequencer_test_utils.cpp
const std::string kChannelName("SEQUENCERGUI-CA-TESTS:INT");
}  // namespace

//! Tests for DomainWorkspaceListener class.
//! oac-tree workspace is populated with ChannelAccessVariable.
class DomainWorkspaceListenerSoftIocTest : public ::testing::Test
{
public:
  using mock_client_t = testing::MockFunction<void(const VariableUpdatedEvent &)>;

  sup::oac_tree::Workspace m_workspace;
};

TEST_F(DomainWorkspaceListenerSoftIocTest, ListeningWorkspaceWithSingleCAVariable)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  mock_client_t mock_client;
  const std::string var_name("abc");

  // creating ChannelAccessVariable
  const sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  auto variable = test::CreateChannelAccessVariable(var_name, value, kChannelName);
  m_workspace.AddVariable(var_name, std::move(variable));

  DomainWorkspaceListener listener(&m_workspace, mock_client.AsStdFunction());
  EXPECT_EQ(listener.GetEventCount(), 0);

  const sup::dto::AnyValue softioc_value(sup::dto::SignedInteger32Type, 1);

  m_workspace.Setup();
  // checking current server variable
  EXPECT_TRUE(m_workspace.WaitForVariable(var_name, 20.0));

  // These are two events from domain that variable has changed. It is not processed yet by the
  // workspace listener, since connection is queued.
  EXPECT_EQ(listener.GetEventCount(), 2);

  // Connection to SoftIoc generates two callbacks. The first callback contains the value=0, and
  // connection=1 and it is triggered by the connection status change. The second callback contains
  // value=1 and connection=1 and it is triggered by initial value in epics.db file.

  // expecting DataChangedEvent for item representing connected attribute
  const sup::dto::AnyValue softioc_value_on_connection(sup::dto::SignedInteger32Type, 0);
  const VariableUpdatedEvent expected_event1{0, softioc_value_on_connection, true};

  const sup::dto::AnyValue softioc_value_in_db_file(sup::dto::SignedInteger32Type, 1);
  const VariableUpdatedEvent expected_event2{0, softioc_value_in_db_file, true};

  {
    const ::testing::InSequence seq;

    EXPECT_CALL(mock_client, Call(expected_event1)).Times(1);
    EXPECT_CALL(mock_client, Call(expected_event2)).Times(1);
  }

  // let event loop do its job
  auto empty_queue_predicate = [&listener]() { return listener.GetEventCount() == 0; };
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 100));

  // setting expectation for second value change
  const sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 43);
  const VariableUpdatedEvent expected_event3{0, new_value, true};
  EXPECT_CALL(mock_client, Call(expected_event3)).Times(1);

  EXPECT_TRUE(m_workspace.SetValue(var_name, new_value));
  auto worker = [this, var_name, &new_value]()
  {
    sup::dto::AnyValue tmp;
    auto result = m_workspace.GetValue(var_name, tmp);
    return result && tmp == new_value;
  };
  EXPECT_TRUE(sup::epics::test::BusyWaitFor(2.0, worker));

  EXPECT_EQ(listener.GetEventCount(), 1);

  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 1000));
}

}  // namespace oac_tree_gui::test
