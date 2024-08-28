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

#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/workspace_monitor_helper.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/workspace.h>

#include <testutils/mock_domain_workspace_listener.h>

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
  const sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(value, *variable_item);
  PopulateDomainWorkspace(*m_workspace_item, m_workspace);

  WorkspaceItemListener listener(m_workspace_item, &m_workspace);

  m_workspace.Setup();

  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);

  EXPECT_EQ(GetAnyValue(var_name, m_workspace), value);

  const sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 43);
  EXPECT_CALL(domain_listener, OnEvent(var_name, new_value, true)).Times(1);

  variable_item->GetAnyValueItem()->SetData(43);

  EXPECT_EQ(GetAnyValue(var_name, m_workspace), new_value);
}

TEST_F(WorkspaceItemListenerTest, ProcessEventFromDomain)
{
  const std::string var_name("abc");

  // creating VariableItem and populating domain workspace
  auto variable_item = m_model.InsertItem<LocalVariableItem>(m_workspace_item);
  variable_item->SetName(var_name);
  const sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(value, *variable_item);
  PopulateDomainWorkspace(*m_workspace_item, m_workspace);

  WorkspaceItemListener listener(m_workspace_item, &m_workspace);

  m_workspace.Setup();

  EXPECT_EQ(GetAnyValue(var_name, m_workspace), value);

  // pretending we processing an event from the domain
  const sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 43);
  listener.ProcessEventFromDomain(VariableUpdatedEvent{0, new_value, true});

  // domain workspace still has old value thanks to the blocking flag
  EXPECT_EQ(GetAnyValue(var_name, m_workspace), value);

  // GUI model was updated
  EXPECT_EQ(variable_item->GetAnyValueItem()->Data<int>(), 43);
}

//! Setting up the WorkspaceItem with single variable.
//! Replacing AnyValueItem through the model using SetAnyValue and expecting callback toward the
//! domain.
TEST_F(WorkspaceItemListenerTest, ModifyAnyValueFromModelViaInsert)
{
  const std::string var_name("abc");

  const sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  auto variable_item = m_workspace_item->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  variable_item->SetName("abc");
  SetAnyValue(value, *variable_item);

  PopulateDomainWorkspace(*m_workspace_item, m_workspace);

  WorkspaceItemListener listener(m_workspace_item, &m_workspace);
  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);

  m_workspace.Setup();

  // preparing callback expectations
  const sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 43);
  EXPECT_CALL(domain_listener, OnEvent(var_name, new_value, true)).Times(1);

  // modifying value from the model
  SetAnyValue(new_value, *variable_item);  // replaces AnyValueItem

  EXPECT_EQ(GetAnyValue(var_name, m_workspace), new_value);
  EXPECT_EQ(variable_item->GetAnyValueItem()->Data<int>(), 43);
}

//! Setting up the workspace with two variables. Replacing variables one after another and checking
//! the order of callbacks.

TEST_F(WorkspaceItemListenerTest, ModifyTwoVariablesViaInserts)
{
  const std::string var_name0("var0");
  const std::string var_name1("var1");
  sup::dto::AnyValue value0(sup::dto::SignedInteger32Type, 42);
  sup::dto::AnyValue value1(sup::dto::SignedInteger32Type, 43);
  sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 44);

  auto variable_item0 = m_workspace_item->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  SetAnyValue(value0, *variable_item0);
  variable_item0->SetName(var_name0);

  auto variable_item1 = m_workspace_item->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  SetAnyValue(value1, *variable_item1);
  variable_item1->SetName(var_name1);

  PopulateDomainWorkspace(*m_workspace_item, m_workspace);

  WorkspaceItemListener listener(m_workspace_item, &m_workspace);
  testutils::MockDomainWorkspaceListener domain_listener(m_workspace);

  m_workspace.Setup();

  {
    ::testing::InSequence seq;
    EXPECT_CALL(domain_listener, OnEvent(var_name0, new_value, true)).Times(1);
    EXPECT_CALL(domain_listener, OnEvent(var_name1, new_value, true)).Times(1);
  }

  // modifying value from the model
  SetAnyValue(new_value, *variable_item0);
  SetAnyValue(new_value, *variable_item1);

  EXPECT_EQ(GetAnyValue(var_name0, m_workspace), new_value);
  EXPECT_EQ(GetAnyValue(var_name1, m_workspace), new_value);
}
