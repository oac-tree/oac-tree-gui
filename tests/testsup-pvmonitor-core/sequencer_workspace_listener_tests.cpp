/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
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
#include <sequencergui/model/domain_utils.h>
#include <sup/dto/anyvalue.h>
#include <sup/sequencer/workspace.h>
#include <suppvmonitor/workspace_event.h>

#include <QSignalSpy>

using namespace suppvmonitor;

//! Tests for InstructionViewItemMap class.

class SequencerWorkspaceListenerTests : public ::testing::Test
{
public:

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

TEST_F(SequencerWorkspaceListenerTests, LocalVariableInTheWorkspace)
{
  SequencerWorkspaceListener listener;
  QSignalSpy spy_upate(&listener, &SequencerWorkspaceListener::VariabledUpdated);

  sup::sequencer::Workspace workspace;
  EXPECT_NO_THROW(listener.StartListening(&workspace));

  EXPECT_EQ(spy_upate.count(), 0);

  // creating local variable
  const std::string expected_name("abc");
  const std::string expected_type(R"RAW({"type":"int32"})RAW");
  sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 42};
  auto local_variable = sequencergui::DomainUtils::CreateDomainVariable(
      sequencergui::domainconstants::kLocalVariableType);
  local_variable->SetName(expected_name);
  local_variable->AddAttribute("type", expected_type);
  local_variable->Setup();

  EXPECT_TRUE(local_variable->SetValue(expected_anyvalue));
  auto local_variable_ptr = local_variable.get();

  // adding it to the workspace, expecting no signals
  workspace.AddVariable("abcdef", local_variable.release());
  EXPECT_EQ(spy_upate.count(), 0);
  EXPECT_EQ(listener.GetEventCount(), 0);

  // setting workspace, expecting no signals
  workspace.Setup();
  EXPECT_EQ(spy_upate.count(), 0);
  EXPECT_EQ(listener.GetEventCount(), 0);

  // changing variable
  //  local_variable_ptr->SetValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  EXPECT_TRUE(workspace.SetValue("abcdef", sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43}));
  EXPECT_EQ(spy_upate.count(), 1);
  EXPECT_EQ(listener.GetEventCount(), 1);
}
