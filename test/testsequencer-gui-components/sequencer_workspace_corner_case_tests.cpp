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

#include <sequencergui/domain/domain_utils.h>
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

//! Single variable in workspace. Test validates initial value notificartions on workspace->Setup.
TEST_F(SequencerWorkspaceCornerCaseTest, LocalVariable)
{
  const std::string var_name("var0");

  // creating local variable
  const sup::dto::AnyValue initial_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
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
  const sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 44});
  EXPECT_CALL(listener, OnEvent(var_name, new_value, true)).Times(1);

  variable_ptr->SetValue(new_value);

  sup::dto::AnyValue current_value2;
  EXPECT_TRUE(variable_ptr->GetValue(current_value2));
  EXPECT_EQ(current_value2, new_value);
}

TEST_F(SequencerWorkspaceCornerCaseTest, PVAccessServerVariable)
{
  const std::string var_name("var0");
  const std::string channel_name("var0");

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  // creating local variable
  const sup::dto::AnyValue initial_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  auto variable = testutils::CreatePVAccessServerVariable(var_name, channel_name, initial_value);
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
  const sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 44});
  EXPECT_CALL(listener, OnEvent(var_name, new_value, true)).Times(1);

  EXPECT_TRUE(m_workspace.SetValue(var_name, new_value));

  auto worker = [this, &var_name]()
  {
    sup::dto::AnyValue tmp;
    return m_workspace.GetValue(var_name, tmp) && tmp.As<sup::dto::uint32>() == 44;
  };
  EXPECT_TRUE(sup::epics::test::BusyWaitFor(2.0, worker));
}
