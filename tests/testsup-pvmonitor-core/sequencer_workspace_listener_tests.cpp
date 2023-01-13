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
#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_utils.h>
#include <sup/dto/anyvalue.h>
#include <sup/gui/dto/anyvalue_utils.h>
#include <sup/sequencer/workspace.h>
#include <suppvmonitor/workspace_event.h>

#include <QSignalSpy>
#include <iostream>

using namespace suppvmonitor;

//! Tests for SequencerWorkspaceListener class.

class SequencerWorkspaceListenerTests : public ::testing::Test
{
public:
  std::unique_ptr<variable_t> CreateLocalVariable(const std::string& name,
                                                  const sup::dto::AnyValue& initial_value)
  {
    auto local_variable = sequencergui::DomainUtils::CreateDomainVariable(
        sequencergui::domainconstants::kLocalVariableType);
    local_variable->SetName(name);
    local_variable->AddAttribute("type", sup::gui::GetAnyTypeToJSONString(&initial_value));
    local_variable->Setup();
    if (!local_variable->SetValue(initial_value))
    {
      throw std::runtime_error("Can't create variable");
    }
    return local_variable;
  }
};

//! Initial state.

TEST_F(SequencerWorkspaceListenerTests, InitialState)
{
  SequencerWorkspaceListener listener;

  EXPECT_EQ(listener.GetEventCount(), 0);
  auto event = listener.PopEvent();
  EXPECT_TRUE(event.m_variable_name.empty());
  EXPECT_TRUE(sup::dto::IsEmptyValue(event.m_value));
}

//! Checking exceptions during start/stop listening.

TEST_F(SequencerWorkspaceListenerTests, StartListeningStopListening)
{
  SequencerWorkspaceListener listener;

  EXPECT_THROW(listener.StartListening(nullptr), sequencergui::RuntimeException);
  EXPECT_THROW(listener.StopListening(), sequencergui::RuntimeException);

  sup::sequencer::Workspace workspace;

  // it is possible to start listening to the given workspace only once
  EXPECT_NO_THROW(listener.StartListening(&workspace));
  EXPECT_THROW(listener.StartListening(nullptr), sequencergui::RuntimeException);

  // it is possible to stop listening given workspace only once
  EXPECT_NO_THROW(listener.StopListening());
  EXPECT_THROW(listener.StopListening(), sequencergui::RuntimeException);
}

//! Single local variable is created in the workspace.
//! We change it's value several times, check signaling, and validate workspace events.

TEST_F(SequencerWorkspaceListenerTests, LocalVariableInTheWorkspace)
{
  SequencerWorkspaceListener listener;
  QSignalSpy spy_upate(&listener, &SequencerWorkspaceListener::VariabledUpdated);

  sup::sequencer::Workspace workspace;
  EXPECT_NO_THROW(listener.StartListening(&workspace));

  EXPECT_EQ(spy_upate.count(), 0);

  // creating local variable
  sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  auto local_variable = CreateLocalVariable("abc", value0);
  auto local_variable_ptr = local_variable.get();

  // adding it to the workspace, expecting no signals
  workspace.AddVariable("abcdef", local_variable.release());
  EXPECT_EQ(spy_upate.count(), 0);
  EXPECT_EQ(listener.GetEventCount(), 0);

  // setting workspace, expecting no signals
  workspace.Setup();
  EXPECT_EQ(spy_upate.count(), 0);
  EXPECT_EQ(listener.GetEventCount(), 0);

  // changing variable via workspace
  sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  EXPECT_TRUE(workspace.SetValue("abcdef", value1));
  EXPECT_EQ(spy_upate.count(), 1);
  EXPECT_EQ(listener.GetEventCount(), 1);

  // changing variable via variable pointer
  sup::dto::AnyValue value2(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 44});
  local_variable_ptr->SetValue(value2);
  EXPECT_EQ(spy_upate.count(), 2);
  EXPECT_EQ(listener.GetEventCount(), 2);

  // getting back first value
  auto workspace_event = listener.PopEvent();
  EXPECT_EQ(workspace_event.m_variable_name, std::string("abcdef"));
  EXPECT_EQ(workspace_event.m_value, value1);
  EXPECT_EQ(listener.GetEventCount(), 1);

  // getting back second value
  workspace_event = listener.PopEvent();
  EXPECT_EQ(workspace_event.m_variable_name, std::string("abcdef"));
  EXPECT_EQ(workspace_event.m_value, value2);
  EXPECT_EQ(listener.GetEventCount(), 0);

  // there are no events left
  auto empty_event = listener.PopEvent();
  EXPECT_TRUE(empty_event.m_variable_name.empty());
  EXPECT_TRUE(sup::dto::IsEmptyValue(empty_event.m_value));
}

//! Single local variable is created in the workspace.
//! After first update we stop listening and check that no signals have been issued after.

TEST_F(SequencerWorkspaceListenerTests, StopListeningWorkspace)
{
  SequencerWorkspaceListener listener;
  QSignalSpy spy_upate(&listener, &SequencerWorkspaceListener::VariabledUpdated);

  // creating workspace with single variable
  sup::sequencer::Workspace workspace;
  sup::dto::AnyValue value0(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  auto local_variable = CreateLocalVariable("abc", value0);
  workspace.AddVariable("abcdef", local_variable.release());

  EXPECT_NO_THROW(listener.StartListening(&workspace));

  sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  EXPECT_TRUE(workspace.SetValue("abcdef", value1));

  EXPECT_EQ(spy_upate.count(), 1);
  EXPECT_EQ(listener.GetEventCount(), 1);

  // stop listening
  listener.StopListening();

  sup::dto::AnyValue value2(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 44});
  EXPECT_TRUE(workspace.SetValue("abcdef", value2));

  // no other signals
  EXPECT_EQ(spy_upate.count(), 1);
  EXPECT_EQ(listener.GetEventCount(), 1);
}
