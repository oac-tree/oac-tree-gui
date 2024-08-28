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

#include "sequencergui/pvmonitor/workspace_item_listener.h"

#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/workspace_monitor_helper.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/workspace.h>

using namespace sequencergui;

#include <gtest/gtest.h>

//! Tests for WorkspaceItemListener class.

class WorkspaceItemListenerTest : public ::testing::Test
{
public:
  WorkspaceItemListenerTest() { m_workspace_item = m_model.InsertItem<WorkspaceItem>(); }

  sup::sequencer::Workspace m_workspace;
  WorkspaceItem *m_workspace_item{nullptr};
  mvvm::ApplicationModel m_model;
};

//! Setting up the WorkspaceItem with single scalar variable. Setting the value through the model
//! and expecting upadte in the domain.
TEST_F(WorkspaceItemListenerTest, SetScalarData)
{
  const std::string var_name("abc");

  // creating VariableItem and populating domain workspace
  auto variable_item = m_model.InsertItem<LocalVariableItem>(m_workspace_item);
  variable_item->SetName(var_name);
  sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(value, *variable_item);
  PopulateDomainWorkspace(*m_workspace_item, m_workspace);

  WorkspaceItemListener listener(m_workspace_item, &m_workspace);

  m_workspace.Setup();

  EXPECT_EQ(GetAnyValue(var_name, m_workspace), value);

  variable_item->GetAnyValueItem()->SetData(43);

  sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 43);
  EXPECT_EQ(GetAnyValue(var_name, m_workspace), new_value);
}

TEST_F(WorkspaceItemListenerTest, ProcessEventFromDomain)
{
  const std::string var_name("abc");

  // creating VariableItem and populating domain workspace
  auto variable_item = m_model.InsertItem<LocalVariableItem>(m_workspace_item);
  variable_item->SetName(var_name);
  sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(value, *variable_item);
  PopulateDomainWorkspace(*m_workspace_item, m_workspace);

  WorkspaceItemListener listener(m_workspace_item, &m_workspace);

  m_workspace.Setup();

  EXPECT_EQ(GetAnyValue(var_name, m_workspace), value);

  // pretending we processing an event from the domain
  sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 43);
  listener.ProcessEventFromDomain(VariableUpdatedEvent{0, new_value, true});

  // domain workspace still has old value thanks to the blocking flag
  EXPECT_EQ(GetAnyValue(var_name, m_workspace), value);

  // GUI model was updated
  EXPECT_EQ(variable_item->GetAnyValueItem()->Data<int>(), 43);
}
