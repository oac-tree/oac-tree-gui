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

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/domain_events.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

#include <QSignalSpy>

using namespace sequencergui;

//! Tests for SequencerWorkspaceListener class.
class SequencerWorkspaceListenerTest : public ::testing::Test
{
};

//! Initial state.
TEST_F(SequencerWorkspaceListenerTest, InitialState)
{
  {
    EXPECT_THROW(SequencerWorkspaceListener{nullptr}, RuntimeException);
  }

  sup::sequencer::Workspace workspace;
  SequencerWorkspaceListener listener(&workspace);

  EXPECT_EQ(listener.GetEventCount(), 0);
  auto event = listener.PopEvent();
  EXPECT_TRUE(event.variable_name.empty());
  EXPECT_TRUE(sup::dto::IsEmptyValue(event.value));
}

//! Single local variable is created in the workspace.
//! We change it's value several times, check signaling, and validate workspace events.
TEST_F(SequencerWorkspaceListenerTest, LocalVariableInTheWorkspace)
{
  sup::sequencer::Workspace workspace;

  const std::string var_name("abc");

  SequencerWorkspaceListener listener(&workspace);
  const QSignalSpy spy_upate(&listener, &SequencerWorkspaceListener::VariabledUpdated);
  EXPECT_NO_THROW(listener.StartListening());

  EXPECT_EQ(spy_upate.count(), 0);

  // creating local variable
  const sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  auto local_variable = testutils::CreateLocalVariable(var_name, value0);
  auto local_variable_ptr = local_variable.get();

  // adding it to the workspace, expecting no signals
  workspace.AddVariable(var_name, std::move(local_variable));
  EXPECT_EQ(spy_upate.count(), 0);
  EXPECT_EQ(listener.GetEventCount(), 0);

  // setting workspace
  workspace.Setup();
  // single signal notifies initial value
  EXPECT_EQ(spy_upate.count(), 1);
  EXPECT_EQ(listener.GetEventCount(), 1);

  // changing variable via workspace
  const sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  EXPECT_TRUE(workspace.SetValue(var_name, value1));
  EXPECT_EQ(spy_upate.count(), 2);
  EXPECT_EQ(listener.GetEventCount(), 2);

  // changing variable via variable pointer
  const sup::dto::AnyValue value2(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 44});
  local_variable_ptr->SetValue(value2);
  EXPECT_EQ(spy_upate.count(), 3);
  EXPECT_EQ(listener.GetEventCount(), 3);

  // getting back initial value
  auto workspace_event = listener.PopEvent();
  EXPECT_EQ(workspace_event.variable_name, var_name);
  EXPECT_EQ(workspace_event.value, value0);
  EXPECT_EQ(listener.GetEventCount(), 2);

  // getting back first update
  workspace_event = listener.PopEvent();
  EXPECT_EQ(workspace_event.variable_name, var_name);
  EXPECT_EQ(workspace_event.value, value1);
  EXPECT_EQ(listener.GetEventCount(), 1);

  // getting back second update
  workspace_event = listener.PopEvent();
  EXPECT_EQ(workspace_event.variable_name, var_name);
  EXPECT_EQ(workspace_event.value, value2);
  EXPECT_EQ(listener.GetEventCount(), 0);

  // there are no events left
  auto empty_event = listener.PopEvent();
  EXPECT_TRUE(empty_event.variable_name.empty());
  EXPECT_TRUE(sup::dto::IsEmptyValue(empty_event.value));
}

//! Single local variable is created in the workspace.
//! After first update we stop listening and check that no signals have been issued after.

TEST_F(SequencerWorkspaceListenerTest, StopListeningWorkspace)
{
  sup::sequencer::Workspace workspace;
  const std::string var_name("abc");

  auto listener = std::make_unique<SequencerWorkspaceListener>(&workspace);
  QSignalSpy spy_upate(listener.get(), &SequencerWorkspaceListener::VariabledUpdated);

  // creating workspace with single variable
  const sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  auto local_variable = testutils::CreateLocalVariable(var_name, value0);
  workspace.AddVariable(var_name, std::move(local_variable));

  // setting workspace
  workspace.Setup();

  EXPECT_NO_THROW(listener->StartListening());

  const sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  EXPECT_TRUE(workspace.SetValue(var_name, value1));

  EXPECT_EQ(spy_upate.count(), 1);
  EXPECT_EQ(listener->GetEventCount(), 1);

  // stop listening
  listener.reset();

  const sup::dto::AnyValue value2(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 44});
  EXPECT_TRUE(workspace.SetValue(var_name, value2));

  // no other signals
  EXPECT_EQ(spy_upate.count(), 1);
}
