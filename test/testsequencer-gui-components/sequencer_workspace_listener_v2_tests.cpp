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

#include "sequencergui/pvmonitor/sequencer_workspace_listener_v2.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/workspace_monitor_helper.h>
#include <sequencergui/transform/transform_helpers.h>

#include <mvvm/model/application_model.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>
#include <mvvm/test/mock_model_listener.h>

#include <QSignalSpy>

using namespace sequencergui;

//! Tests for SequencerWorkspaceListenerV2 class.
class SequencerWorkspaceListenerV2Test : public ::testing::Test
{
public:
  SequencerWorkspaceListenerV2Test() { m_workspace_item = m_model.InsertItem<WorkspaceItem>(); }

  sup::sequencer::Workspace m_workspace;
  WorkspaceItem *m_workspace_item{nullptr};
  mvvm::ApplicationModel m_model;
};

//! Initial state.
TEST_F(SequencerWorkspaceListenerV2Test, InitialState)
{
  EXPECT_THROW(SequencerWorkspaceListenerV2(nullptr, nullptr), RuntimeException);
  EXPECT_THROW(SequencerWorkspaceListenerV2(m_workspace_item, nullptr), RuntimeException);
  EXPECT_THROW(SequencerWorkspaceListenerV2(nullptr, &m_workspace), RuntimeException);

  const SequencerWorkspaceListenerV2 listener(m_workspace_item, &m_workspace);
  EXPECT_EQ(listener.GetEventCount(), 0);

  // listener shall be constructed before workspace setup
  m_workspace.Setup();
  EXPECT_THROW(SequencerWorkspaceListenerV2(m_workspace_item, &m_workspace), RuntimeException);
}

//! Single local variable is created in both workspaces. Initial values coincides. Changing the
//! value on domain side and checking event propagation.
TEST_F(SequencerWorkspaceListenerV2Test, LocalVariableInTheWorkspace)
{
  const std::string var_name("abc");

  // creating VariableItem
  auto variable_item = m_model.InsertItem<LocalVariableItem>(m_workspace_item);
  variable_item->SetName(var_name);
  sup::dto::AnyValue value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  SetAnyValue(value, *variable_item);

  // populating domain workspace
  PopulateDomainWorkspace(*m_workspace_item, m_workspace);

  SequencerWorkspaceListenerV2 listener(m_workspace_item, &m_workspace);
  EXPECT_EQ(listener.GetEventCount(), 0);

  mvvm::test::MockModelListenerV2 model_listener(&m_model);

  m_workspace.Setup();

  // changing variable via workspace
  const sup::dto::AnyValue value1(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43});
  EXPECT_TRUE(m_workspace.SetValue(var_name, value1));
  // EXPECT_EQ(listener.GetEventCount(), 2);

}
