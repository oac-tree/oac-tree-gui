/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/domain/sequencer_types_fwd.h>

#include <sup/dto/anyvalue.h>
#include <sup/epics-test/unit_test_helper.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <testutils/mock_domain_workspace_listener.h>
#include <testutils/sequencer_test_utils.h>
#include <testutils/test_utils.h>

using ::testing::_;
using namespace sequencergui;

namespace
{
const std::string kTestPrefix("SequencerWorkspaceCornerCaseTests:");
}  // namespace

//! Testing domain sequencer workspace for notifications accompanying Setup, and other corner cases.

class SequencerWorkspaceCornerCaseTest : public ::testing::Test
{
public:
  sup::sequencer::Workspace m_workspace;
};

//! Single local variable in workspace. Test validates initial value notifications on
//! workspace->Setup.
TEST_F(SequencerWorkspaceCornerCaseTest, LocalVariable)
{
  const std::string var_name("var0");

  // creating local variable
  const sup::dto::AnyValue initial_value(sup::dto::SignedInteger32Type, 42);
  auto variable = testutils::CreateLocalVariable(var_name, initial_value);
  auto variable_ptr = variable.get();

  // listener is subscribed to the workspace on the contstruction already
  testutils::MockDomainWorkspaceListener listener(m_workspace);

  // adding variable doesn't cause notifications
  EXPECT_CALL(listener, OnEvent(_, _, _)).Times(0);
  m_workspace.AddVariable(var_name, std::move(variable));

  EXPECT_EQ(variable_ptr, m_workspace.GetVariable(var_name));
  EXPECT_FALSE(variable_ptr->IsAvailable());

  // workspace setup causes a single notification for the initial value
  EXPECT_CALL(listener, OnEvent(_, _, _)).Times(1);
  EXPECT_NO_THROW(m_workspace.Setup());

  // variable is available and has correct value
  EXPECT_TRUE(variable_ptr->IsAvailable());
  sup::dto::AnyValue current_value;
  EXPECT_TRUE(variable_ptr->GetValue(current_value));
  EXPECT_EQ(current_value, initial_value);

  // expecting notification on new value set
  const sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 44);
  EXPECT_CALL(listener, OnEvent(var_name, new_value, true)).Times(1);

  variable_ptr->SetValue(new_value);

  sup::dto::AnyValue current_value2;
  EXPECT_TRUE(variable_ptr->GetValue(current_value2));
  EXPECT_EQ(current_value2, new_value);
}

//! Single PVAccessServer variable in workspace. Test validates initial value notifications on
//! workspace->Setup.

TEST_F(SequencerWorkspaceCornerCaseTest, PVAccessServerVariable)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const std::string var_name("var0");
  const std::string kChannelName(kTestPrefix + "PVAccessServerVariable");

  // creating local variable
  const sup::dto::AnyValue initial_value(sup::dto::SignedInteger32Type, 42);
  auto variable = testutils::CreatePVAccessServerVariable(var_name, initial_value, kChannelName);
  auto variable_ptr = variable.get();

  // listener is subscribed to the workspace on the contstruction already
  testutils::MockDomainWorkspaceListener listener(m_workspace);

  // adding variable doesn't cause notifications
  EXPECT_CALL(listener, OnEvent(_, _, _)).Times(0);
  m_workspace.AddVariable(var_name, std::move(variable));

  EXPECT_EQ(variable_ptr, m_workspace.GetVariable(var_name));
  EXPECT_FALSE(variable_ptr->IsAvailable());

  // workspace setup causes a single notification for the initial value
  EXPECT_CALL(listener, OnEvent(_, _, _)).Times(1);
  EXPECT_NO_THROW(m_workspace.Setup());

  // giving variable time to become available
  EXPECT_TRUE(m_workspace.WaitForVariable(var_name, 5.0));

  // variable is available and has correct value
  EXPECT_TRUE(variable_ptr->IsAvailable());
  sup::dto::AnyValue current_value;
  EXPECT_TRUE(variable_ptr->GetValue(current_value));
  EXPECT_EQ(current_value, initial_value);

  // expecting notification on new value set
  const sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 44);
  EXPECT_CALL(listener, OnEvent(var_name, new_value, true)).Times(1);

  EXPECT_TRUE(m_workspace.SetValue(var_name, new_value));

  auto worker = [this, &var_name]()
  {
    sup::dto::AnyValue tmp;
    return m_workspace.GetValue(var_name, tmp) && tmp.As<sup::dto::uint32>() == 44;
  };
  EXPECT_TRUE(sup::epics::test::BusyWaitFor(2.0, worker));
}

//! Two variables, PVAccessServer and PVAccessClient in workspace. Test validates initial values
//! notifications on workspace->Setup.

TEST_F(SequencerWorkspaceCornerCaseTest, PVAccessClientAndServerVariables)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  const sup::dto::AnyValue empty_value;

  const std::string server_var_name("server");
  const std::string client_var_name("client");
  const std::string kChannelName(kTestPrefix + "PVAccessClientAndServerVariables");
  const sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});

  // adding two variables in a workspace
  auto server_variable =
      testutils::CreatePVAccessServerVariable(server_var_name, initial_value, kChannelName);
  auto server_variable_ptr = server_variable.get();

  // adding two variables in a workspace
  auto client_variable =
      testutils::CreatePVAccessClientVariable(client_var_name, initial_value, kChannelName);
  auto client_variable_ptr = client_variable.get();

  m_workspace.AddVariable(server_var_name, std::move(server_variable));
  m_workspace.AddVariable(client_var_name, std::move(client_variable));

  // setting expectations
  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);
  {
    const ::testing::InSequence seq;
    // server variable reports its availability once
    EXPECT_CALL(domain_listener, OnEvent(server_var_name, initial_value, true)).Times(1);
    // client variable reports twice: availability, and then the value
    EXPECT_CALL(domain_listener, OnEvent(client_var_name, empty_value, true)).Times(1);
    EXPECT_CALL(domain_listener, OnEvent(client_var_name, initial_value, true)).Times(1);
  }

  // triggering expectations
  m_workspace.Setup();

  // server variable reports immediately, client variable needs special waiting
  m_workspace.WaitForVariable(client_var_name, 2.0, true);
}
