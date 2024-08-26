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

#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/pvmonitor/workspace_event.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/epics-test/unit_test_helper.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

#include <QSignalSpy>
#include <QTest>

namespace
{
const std::string kTestPrefix("SequencerWorkspaceListenerPVAccessTest:");
const std::string kScalarChannelName(kTestPrefix + "scalar");
const std::string kStructChannelName(kTestPrefix + "STRUCT");
}  // namespace

using namespace sequencergui;

//! Tests for SequencerWorkspaceListener class.
//! Sequencer workspace is populated with PVAccessServer and PVAccessChannel variables.

class SequencerWorkspaceListenerPVAccessTest : public ::testing::Test
{
public:
  //! Disables all tests in the fixture if PVAccess is not available
  void SetUp() override
  {
    if (!IsSequencerPluginEpicsAvailable())
    {
      GTEST_SKIP();
    }
  }
};

//! Single scalar variable in a workspace.
//! We start listening before the setup. Test confirms, that after the setup no callback
//! with the initial value follows.

TEST_F(SequencerWorkspaceListenerPVAccessTest, WorkspaceWithSingleServerScalarVariable)
{
  // creating server variable
  const std::string var_name("var");
  const sup::dto::AnyValue initial_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  auto variable =
      testutils::CreatePVAccessServerVariable(var_name, initial_value, kScalarChannelName);

  // initialising workspace
  sup::sequencer::Workspace workspace;
  EXPECT_TRUE(workspace.AddVariable(var_name, std::move(variable)));

  // creating listener and attaching it to the workspace
  SequencerWorkspaceListener listener(&workspace);
  const QSignalSpy spy_upate(&listener, &SequencerWorkspaceListener::VariabledUpdated);
  listener.StartListening();

  QTest::qWait(10);

  // no signals after object initialisation
  EXPECT_EQ(listener.GetEventCount(), 0);
  EXPECT_EQ(spy_upate.count(), 0);

  EXPECT_NO_THROW(workspace.Setup());

  QTest::qWait(10);

  // after workspace setup
  EXPECT_EQ(listener.GetEventCount(), 1);
  EXPECT_EQ(spy_upate.count(), 1);

  // The event stored in the listener should contain initial value after workspace Setup.
  // However, due to the way we store a scalar inside struct, expected initial value will be
  // wrapped in a struct.
  const sup::dto::AnyValue expected_initial_value({{"value", {sup::dto::SignedInteger32Type, 43}}});
  auto event = listener.PopEvent();
  EXPECT_EQ(event.variable_name, var_name);
  EXPECT_EQ(event.value, expected_initial_value);

  // checking current server variable
  EXPECT_TRUE(workspace.WaitForVariable(var_name, 5.0));
  sup::dto::AnyValue value;
  ASSERT_TRUE(workspace.GetValue("var", value));
  EXPECT_TRUE(workspace.GetVariable("var")->IsAvailable());
  EXPECT_EQ(value, initial_value);

  // changing the value and waiting for update
  value = 42;
  EXPECT_TRUE(workspace.SetValue("var", value));
  auto worker = [&workspace]()
  {
    sup::dto::AnyValue tmp;
    return workspace.GetValue("var", tmp) && tmp.As<sup::dto::uint32>() == 42;
  };
  EXPECT_TRUE(sup::epics::test::BusyWaitFor(2.0, worker));

  EXPECT_EQ(spy_upate.count(), 2);

  // checking accumulated event
  const sup::dto::AnyValue expected_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  EXPECT_EQ(listener.GetEventCount(), 1);
  event = listener.PopEvent();
  EXPECT_EQ(event.variable_name, var_name);
  EXPECT_EQ(event.value, expected_value);
  EXPECT_TRUE(event.connected);
}

//! Single server variable (struct) in a workspace

TEST_F(SequencerWorkspaceListenerPVAccessTest, WorkspaceWithSingleServerStructVariable)
{
  // creating server variable
  const std::string var_name("var");
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});
  auto variable =
      testutils::CreatePVAccessServerVariable(var_name, initial_value, kStructChannelName);

  // initialising workspace
  sup::sequencer::Workspace workspace;
  EXPECT_TRUE(workspace.AddVariable(var_name, std::move(variable)));
  EXPECT_NO_THROW(workspace.Setup());

  // checking current server variable
  EXPECT_TRUE(workspace.WaitForVariable(var_name, 5.0));
  sup::dto::AnyValue value;
  ASSERT_TRUE(workspace.GetValue("var", value));
  EXPECT_EQ(value, initial_value);

  // creating listener and attaching it to the workspace
  SequencerWorkspaceListener listener(&workspace);
  const QSignalSpy spy_upate(&listener, &SequencerWorkspaceListener::VariabledUpdated);
  listener.StartListening();

  EXPECT_EQ(listener.GetEventCount(), 0);
  EXPECT_EQ(spy_upate.count(), 0);

  // changing the value and waiting for update
  value["value"] = 42;
  EXPECT_TRUE(workspace.SetValue("var", value));
  auto worker = [&workspace]()
  {
    sup::dto::AnyValue tmp;
    return workspace.GetValue("var", tmp) && tmp["value"].As<sup::dto::uint32>() == 42;
  };
  EXPECT_TRUE(sup::epics::test::BusyWaitFor(2.0, worker));

  EXPECT_EQ(spy_upate.count(), 1);

  // checking accumulated event
  const sup::dto::AnyValue expected_value({{"value", {sup::dto::SignedInteger32Type, 42}}});
  EXPECT_EQ(listener.GetEventCount(), 1);
  auto event = listener.PopEvent();
  EXPECT_EQ(event.variable_name, var_name);
  EXPECT_EQ(event.value, expected_value);
  EXPECT_TRUE(event.connected);
}
