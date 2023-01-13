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
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sup/dto/anyvalue.h>
#include <sup/gui/dto/anyvalue_utils.h>
#include <sup/gui/dto/conversion_utils.h>
#include <sup/sequencer/workspace.h>
#include <suppvmonitor/monitor_model.h>
#include <testutils/softioc_runner.h>
#include <testutils/softioc_utils.h>

#include <QSignalSpy>
#include <QTest>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <thread>

namespace
{
// temporary from sup-sequencer-plugin-epics

bool BusyWaitFor(double timeout_sec, std::function<bool()> predicate)
{
  long timeout_ns = std::lround(timeout_sec * 1e9);
  auto time_end = std::chrono::system_clock::now() + std::chrono::nanoseconds(timeout_ns);
  while (!predicate() && std::chrono::system_clock::now() < time_end)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
  return predicate();
}
}  // namespace

using namespace suppvmonitor;

//! Tests for WorkspaceController class.

class SequencerWorkspaceListenerSoftIocTests : public ::testing::Test
{
public:
  static void ValidateChannelAccessPresence()
  {
    if (!sequencergui::DomainUtils::IsChannelAccessClientAvailable())
    {
      // Will disable all tests in the Fixture.
      GTEST_SKIP();
    }
  }

  void SetUp() override { ValidateChannelAccessPresence(); }

  static void SetUpTestSuite()
  {
    ValidateChannelAccessPresence();
    m_softioc_service.Start(testutils::GetEpicsDBContentString());
  }

  static void TearDownTestSuite()
  {
    ValidateChannelAccessPresence();
    m_softioc_service.Stop();
  }

  static testutils::SoftIocRunner m_softioc_service;
};

testutils::SoftIocRunner SequencerWorkspaceListenerSoftIocTests::m_softioc_service{};

//! The sequencer Workspace contains a single channel access variable.
//! Validating that SequencerWorkspaceListener gets notifications.

TEST_F(SequencerWorkspaceListenerSoftIocTests, ListeningWorkspaceWithSingleCAVariable)
{
  // creating ChannelAccessVariable
  auto variable = sequencergui::DomainUtils::CreateDomainVariable(
      sequencergui::domainconstants::kChannelAccessVariableType);
  variable->AddAttribute("channel", "CA-TESTS:INT");
  variable->AddAttribute("type", R"RAW({"type":"uint32"})RAW");
  auto variable_ptr = variable.get();

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
  EXPECT_TRUE(BusyWaitFor(2.0, worker));

  EXPECT_EQ(spy_upate.count(), 1);
}
