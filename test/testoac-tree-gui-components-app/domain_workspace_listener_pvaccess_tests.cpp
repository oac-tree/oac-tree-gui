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

#include "oac-tree-gui/pvmonitor/domain_workspace_listener.h"

#include <oac-tree-gui/core/exceptions.h>
#include <oac-tree-gui/domain/domain_helper.h>
#include <oac-tree-gui/jobsystem/domain_events.h>

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/epics-test/unit_test_helper.h>
#include <sup/oac-tree/workspace.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

#include <QTest>

namespace
{
const std::string kTestPrefix("DomainWorkspaceListenerPVAccessTest:");
const std::string kScalarChannelName(kTestPrefix + "scalar");
const std::string kStructChannelName(kTestPrefix + "STRUCT");
}  // namespace

using testing::_;

namespace sequencergui::test
{

//! Tests for DomainWorkspaceListener class.
class DomainWorkspaceListenerPVAccessTest : public ::testing::Test
{
public:
  using mock_client_t = testing::MockFunction<void(const VariableUpdatedEvent &)>;

  //! Disables all tests in the fixture if PVAccess is not available
  void SetUp() override
  {
    if (!IsSequencerPluginEpicsAvailable())
    {
      GTEST_SKIP();
    }
  }

  sup::oac_tree::Workspace m_workspace;
};

//! Single scalar variable in a workspace.
TEST_F(DomainWorkspaceListenerPVAccessTest, WorkspaceWithSingleServerScalarVariable)
{
  mock_client_t mock_client;
  const std::string var_name("abc");

  // creating LocalVariable in domain workspace
  const sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  auto variable = test::CreatePVAccessServerVariable(var_name, value, kScalarChannelName);
  m_workspace.AddVariable(var_name, std::move(variable));

  DomainWorkspaceListener listener(&m_workspace, mock_client.AsStdFunction());
  EXPECT_EQ(listener.GetEventCount(), 0);

  m_workspace.Setup();

  // This is an event from domain on first variable update. It is not processed yet by the
  // workspace listener, since connection is queued.
  EXPECT_EQ(listener.GetEventCount(), 1);

  // preparing expectations
  const VariableUpdatedEvent expected_event1{0, value, true};
  EXPECT_CALL(mock_client, Call(expected_event1)).Times(1);

  // let event loop do its job and trigger expectations
  auto empty_queue_predicate = [&listener]() { return listener.GetEventCount() == 0; };
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));
  EXPECT_EQ(listener.GetEventCount(), 0);

  // preparing new expectations
  const sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 43);
  const VariableUpdatedEvent expected_event2{0, new_value, true};
  EXPECT_CALL(mock_client, Call(expected_event2)).Times(1);

  EXPECT_TRUE(m_workspace.SetValue(var_name, new_value));
  EXPECT_EQ(listener.GetEventCount(), 1);

  // let event loop do its job
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));
  EXPECT_EQ(listener.GetEventCount(), 0);
}

//! Single struct variable in a workspace. The same as before, except that a scalar is wrapped
//! inside struct.
TEST_F(DomainWorkspaceListenerPVAccessTest, WorkspaceWithSingleServerStructVariable)
{
  mock_client_t mock_client;
  const std::string var_name("abc");

  // creating LocalVariable in domain workspace
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 42}}});
  auto variable = test::CreatePVAccessServerVariable(var_name, initial_value, kStructChannelName);
  m_workspace.AddVariable(var_name, std::move(variable));

  DomainWorkspaceListener listener(&m_workspace, mock_client.AsStdFunction());
  EXPECT_EQ(listener.GetEventCount(), 0);

  m_workspace.Setup();

  // This is an event from domain on first variable update. It is not processed yet by the
  // workspace listener, since connection is queued.
  EXPECT_EQ(listener.GetEventCount(), 1);

  // preparing expectations
  const VariableUpdatedEvent expected_event1{0, initial_value, true};
  EXPECT_CALL(mock_client, Call(expected_event1)).Times(1);

  // let event loop do its job and trigger expectations
  auto empty_queue_predicate = [&listener]() { return listener.GetEventCount() == 0; };
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));
  EXPECT_EQ(listener.GetEventCount(), 0);

  // preparing new expectations
  const sup::dto::AnyValue new_value({{"value", {sup::dto::SignedInteger32Type, 43}}});
  const VariableUpdatedEvent expected_event2{0, new_value, true};
  EXPECT_CALL(mock_client, Call(expected_event2)).Times(1);

  EXPECT_TRUE(m_workspace.SetValue(var_name, new_value));
  EXPECT_EQ(listener.GetEventCount(), 1);

  // let event loop do its job
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));
  EXPECT_EQ(listener.GetEventCount(), 0);
}

}  // namespace sequencergui::test
