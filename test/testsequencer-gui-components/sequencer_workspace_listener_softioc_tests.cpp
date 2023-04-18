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

#include "sequencergui/pvmonitor/sequencer_workspace_listener.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/monitor_model.h>
#include <sequencergui/pvmonitor/workspace_event.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/epics-test/softioc_runner.h>
#include <sup/epics-test/unit_test_helper.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <testutils/epics_test_utils.h>

#include <QSignalSpy>
#include <QTest>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace sequencergui;

//! Tests for WorkspaceController class.
//! Sequencer workspace is populated with ChannelAccessVariable.

class SequencerWorkspaceListenerSoftIocTests : public ::testing::Test
{
public:
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
};

sup::epics::test::SoftIocRunner SequencerWorkspaceListenerSoftIocTests::m_softioc_service{};

//! The sequencer Workspace contains a single channel access variable.
//! Validating that SequencerWorkspaceListener gets notifications.

TEST_F(SequencerWorkspaceListenerSoftIocTests, ListeningWorkspaceWithSingleCAVariable)
{
  // creating ChannelAccessVariable
  auto variable = CreateDomainVariable(domainconstants::kChannelAccessVariableType);
  variable->AddAttribute("channel", "CA-TESTS:INT");
  variable->AddAttribute("type", R"RAW({"type":"uint32"})RAW");

  // adding it to the workspace and validating values
  sup::sequencer::Workspace workspace;

  EXPECT_TRUE(workspace.AddVariable("var", variable.release()));
  EXPECT_NO_THROW(workspace.Setup());
  EXPECT_TRUE(workspace.WaitForVariable("var", 5.0));
  sup::dto::AnyValue value;
  ASSERT_TRUE(workspace.GetValue("var", value));
  EXPECT_EQ(value.GetType(), sup::dto::UnsignedInteger32Type);

  SequencerWorkspaceListener listener;
  QSignalSpy spy_upate(&listener, &SequencerWorkspaceListener::VariabledUpdated);
  EXPECT_NO_THROW(listener.StartListening(&workspace));

  // attaching listener to the workspace
  EXPECT_EQ(spy_upate.count(), 0);

  // changing the value and waiting for update
  value = 42;
  EXPECT_TRUE(workspace.SetValue("var", value));
  auto worker = [&workspace]()
  {
    sup::dto::AnyValue tmp;
    return workspace.GetValue("var", tmp) && tmp.As<sup::dto::uint32>() == 42;
  };
  EXPECT_TRUE(sup::epics::test::BusyWaitFor(2.0, worker));

  EXPECT_EQ(spy_upate.count(), 1);

  // checking accumulated event
  sup::dto::AnyValue expected_value(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 42});
  EXPECT_EQ(listener.GetEventCount(), 1);
  auto event = listener.PopEvent();
  EXPECT_EQ(event.variable_name, std::string("var"));
  EXPECT_EQ(event.value, expected_value);
  EXPECT_TRUE(event.connected);
}
