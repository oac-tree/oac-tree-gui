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

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/domain_events.h>

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/oac-tree/workspace.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

#include <QTest>

#include "oac_tree_gui/pvmonitor/domain_workspace_listener.h"

using testing::_;

namespace oac_tree_gui::test
{

//! Tests for DomainWorkspaceListener class.
class DomainWorkspaceListenerTest : public ::testing::Test
{
public:
  using mock_client_t = testing::MockFunction<void(const VariableUpdatedEvent &)>;

  sup::oac_tree::Workspace m_workspace;
};

//! Initial state.
TEST_F(DomainWorkspaceListenerTest, InitialState)
{
  mock_client_t mock_client;

  EXPECT_THROW(DomainWorkspaceListener(nullptr, {}), RuntimeException);
  EXPECT_THROW(DomainWorkspaceListener(&m_workspace, {}), RuntimeException);

  const DomainWorkspaceListener listener(&m_workspace, mock_client.AsStdFunction());
  EXPECT_EQ(listener.GetEventCount(), 0);

  // listener shall be constructed before workspace setup
  m_workspace.Setup();
  EXPECT_THROW(DomainWorkspaceListener(&m_workspace, mock_client.AsStdFunction()),
               RuntimeException);
}

//! Single local variable is created in the workspace.
//! We change it's value several times, check signaling, and validate workspace events.
TEST_F(DomainWorkspaceListenerTest, LocalVariableInTheWorkspace)
{
  mock_client_t mock_client;
  const std::string var_name("abc");

  // creating LocalVariable in domain workspace
  const sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  auto local_variable = test::CreateLocalVariable(var_name, value);
  auto local_variable_ptr = local_variable.get();
  m_workspace.AddVariable(var_name, std::move(local_variable));

  DomainWorkspaceListener listener(&m_workspace, mock_client.AsStdFunction());
  EXPECT_EQ(listener.GetEventCount(), 0);

  m_workspace.Setup();

  // This is an event from domain that variable has changed. It is not processed yet by the
  // workspace listener, since connection is queued.
  EXPECT_EQ(listener.GetEventCount(), 1);

  VariableUpdatedEvent expected_event{0, value, true};
  EXPECT_CALL(mock_client, Call(expected_event)).Times(1);

  // let event loop do its job
  auto empty_queue_predicate = [&listener]() { return listener.GetEventCount() == 0; };
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));
  EXPECT_EQ(listener.GetEventCount(), 0);

  // preparing new expectations
  const sup::dto::AnyValue value1(sup::dto::SignedInteger32Type, 43);
  const sup::dto::AnyValue value2(sup::dto::SignedInteger32Type, 44);

  {
    const ::testing::InSequence seq;

    const VariableUpdatedEvent expected_event1{0, value1, true};
    EXPECT_CALL(mock_client, Call(expected_event1)).Times(1);
    const VariableUpdatedEvent expected_event2{0, value2, true};
    EXPECT_CALL(mock_client, Call(expected_event2)).Times(1);
  }

  // changing variable via workspace
  EXPECT_TRUE(m_workspace.SetValue(var_name, value1));
  // changing variable via variable pointer
  local_variable_ptr->SetValue(value2);

  EXPECT_EQ(listener.GetEventCount(), 2);

  // let event loop do its job
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));
  EXPECT_EQ(listener.GetEventCount(), 0);
}

TEST_F(DomainWorkspaceListenerTest, StopListeningWorkspace)
{
  mock_client_t mock_client;
  const std::string var_name("abc");

  // creating LocalVariable in domain workspace
  const sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  auto local_variable = test::CreateLocalVariable(var_name, value);
  auto local_variable_ptr = local_variable.get();
  m_workspace.AddVariable(var_name, std::move(local_variable));

  auto listener =
      std::make_unique<DomainWorkspaceListener>(&m_workspace, mock_client.AsStdFunction());
  EXPECT_EQ(listener->GetEventCount(), 0);

  EXPECT_CALL(mock_client, Call(_)).Times(1);
  m_workspace.Setup();

  EXPECT_EQ(listener->GetEventCount(), 1);

  // let event loop do its job
  auto empty_queue_predicate = [&listener]() { return listener->GetEventCount() == 0; };
  EXPECT_TRUE(QTest::qWaitFor(empty_queue_predicate, 50));
  EXPECT_EQ(listener->GetEventCount(), 0);

  // resetting listener, expecting no events from the model
  listener.reset();

  EXPECT_CALL(mock_client, Call(_)).Times(0);

  // changing domain variable
  const sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 43);
  EXPECT_TRUE(m_workspace.SetValue(var_name, new_value));

  // expecting no calls
}

}  // namespace oac_tree_gui::test
