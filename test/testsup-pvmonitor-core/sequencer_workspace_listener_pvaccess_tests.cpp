/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "suppvmonitor/sequencer_workspace_listener.h"

#include <gtest/gtest.h>
#include <sequencergui/domain/domain_utils.h>
#include <suppvmonitor/workspace_event.h>

#include <sup/dto/anyvalue.h>
#include <sup/epics-test/unit_test_helper.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/sequencer/workspace.h>

#include <QSignalSpy>
#include <QTest>

namespace
{
const std::string kTestPrefix("SUP-GUI-CORE-PVTESTS:");
const std::string kStructChannelName(kTestPrefix + "STRUCT");
}  // namespace

using namespace suppvmonitor;

//! Tests for WorkspaceController class.
//! Sequencer workspace is populated with PVAccessServer and PVAccessChannel variables.

class SequencerWorkspaceListenerPVAccessTests : public ::testing::Test
{
public:
  static bool IsPVAccessAvailable()
  {
    return sequencergui::IsPVAccessServerAvailable() && sequencergui::IsPVAccessClientAvailable();
  }

  //! Disables all tests in the fixture if PVAccess is not available
  void SetUp() override
  {
    if (!IsPVAccessAvailable())
    {
      GTEST_SKIP();
    }
  }

  //! Creates PvAccessServerVariable for given channel_name and
  std::unique_ptr<sup::sequencer::Variable> CreateServerVariable(const std::string& channel_name,
                                                                 const sup::dto::AnyValue& anyvalue)
  {
    auto result =
        sequencergui::CreateDomainVariable(sequencergui::domainconstants::kPVServerVariableType);
    result->AddAttribute("channel", channel_name);
    result->AddAttribute("type", sup::gui::GetAnyTypeToJSONString(anyvalue));
    return result;
  }
};

//! Single server variable (struct) in a workspace

TEST_F(SequencerWorkspaceListenerPVAccessTests, WorkspaceWithSingleServerVariable)
{
  // creating server variable
  const std::string var_name("var");
  sup::dto::AnyValue initial_value({{"value", {sup::dto::SignedInteger32Type, 0}}});
  auto variable = CreateServerVariable(kStructChannelName, initial_value);

  // initialising workspace
  sup::sequencer::Workspace workspace;
  EXPECT_TRUE(workspace.AddVariable(var_name, variable.release()));
  EXPECT_NO_THROW(workspace.Setup());

  // checking current server variable
  EXPECT_TRUE(workspace.WaitForVariable(var_name, 5.0));
  sup::dto::AnyValue value;
  ASSERT_TRUE(workspace.GetValue("var", value));
  EXPECT_EQ(value, initial_value);

  // creating listener and attaching it to the workspace
  SequencerWorkspaceListener listener;
  QSignalSpy spy_upate(&listener, &SequencerWorkspaceListener::VariabledUpdated);
  EXPECT_NO_THROW(listener.StartListening(&workspace));

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
  sup::dto::AnyValue expected_value({{"value", {sup::dto::SignedInteger32Type, 42}}});
  EXPECT_EQ(listener.GetEventCount(), 1);
  auto event = listener.PopEvent();
  EXPECT_EQ(event.variable_name, var_name);
  EXPECT_EQ(event.value, expected_value);
  EXPECT_TRUE(event.connected);
}
