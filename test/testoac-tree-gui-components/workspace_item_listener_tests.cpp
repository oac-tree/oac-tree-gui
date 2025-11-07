/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/pvmonitor/workspace_item_listener.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/pvmonitor/workspace_monitor_helper.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>

#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>
#include <testutils/mock_domain_workspace_listener.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for WorkspaceItemListener class.
 */
class WorkspaceItemListenerTest : public ::testing::Test
{
public:
  WorkspaceItemListenerTest() { m_workspace_item = m_model.InsertItem<WorkspaceItem>(); }

  sup::oac_tree::Workspace m_workspace;
  WorkspaceItem* m_workspace_item{nullptr};
  mvvm::ApplicationModel m_model;
};

TEST_F(WorkspaceItemListenerTest, AttemptToInitialize)
{
  EXPECT_THROW(WorkspaceItemListener(nullptr, nullptr), RuntimeException);
  EXPECT_THROW(WorkspaceItemListener(m_workspace_item, nullptr), RuntimeException);
  EXPECT_NO_THROW(WorkspaceItemListener(m_workspace_item, &m_workspace));

  // number of variables doesn't match
  auto variable_item = m_model.InsertItem<LocalVariableItem>(m_workspace_item);
  EXPECT_THROW(WorkspaceItemListener(m_workspace_item, &m_workspace), RuntimeException);
}

//! Setting up the WorkspaceItem with single scalar variable. Setting the value through the model
//! and expecting update in the domain.
TEST_F(WorkspaceItemListenerTest, SetScalarData)
{
  const std::string var_name("abc");

  // creating VariableItem and populating domain workspace
  auto variable_item = m_model.InsertItem<LocalVariableItem>(m_workspace_item);
  variable_item->SetName(var_name);
  const sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(value, *variable_item);
  PopulateDomainWorkspace(*m_workspace_item, m_workspace);

  const WorkspaceItemListener listener(m_workspace_item, &m_workspace);

  m_workspace.Setup();

  MockDomainWorkspaceListener domain_listener(m_workspace);

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

  MockDomainWorkspaceListener domain_listener(m_workspace);

  // expectations triggered by the setup
  EXPECT_CALL(domain_listener, OnEvent(var_name, value, true)).Times(1);
  m_workspace.Setup();

  // expectations triggered by value change
  const sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 43);
  EXPECT_CALL(domain_listener, OnEvent(var_name, new_value, true)).Times(1);
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
  const sup::dto::AnyValue value0(sup::dto::SignedInteger32Type, 42);
  const sup::dto::AnyValue value1(sup::dto::SignedInteger32Type, 43);
  const sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type, 44);

  auto variable_item0 = m_workspace_item->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  SetAnyValue(value0, *variable_item0);
  variable_item0->SetName(var_name0);

  auto variable_item1 = m_workspace_item->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  SetAnyValue(value1, *variable_item1);
  variable_item1->SetName(var_name1);

  PopulateDomainWorkspace(*m_workspace_item, m_workspace);

  const WorkspaceItemListener listener(m_workspace_item, &m_workspace);
  MockDomainWorkspaceListener domain_listener(m_workspace);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(domain_listener, OnEvent(var_name0, value0, true)).Times(1);
    EXPECT_CALL(domain_listener, OnEvent(var_name1, value1, true)).Times(1);
  }
  m_workspace.Setup();

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(domain_listener, OnEvent(var_name0, new_value, true)).Times(1);
    EXPECT_CALL(domain_listener, OnEvent(var_name1, new_value, true)).Times(1);
  }

  // modifying value from the model
  SetAnyValue(new_value, *variable_item0);
  SetAnyValue(new_value, *variable_item1);

  EXPECT_EQ(GetAnyValue(var_name0, m_workspace), new_value);
  EXPECT_EQ(GetAnyValue(var_name1, m_workspace), new_value);
}

//! Setting up two workspaces and two listeners. First workspace contains a single variable, the
//! second workspace is empty. Adding AnyValue to the variable. Making sure that events in one
//! workspace doesn't affect another.
TEST_F(WorkspaceItemListenerTest, TwoWorkspaceScenario)
{
  sup::oac_tree::Workspace domain_workspace0;
  sup::oac_tree::Workspace domain_workspace1;

  auto workspace_item0 = m_model.InsertItem<WorkspaceItem>();
  auto workspace_item1 = m_model.InsertItem<WorkspaceItem>();

  auto variable_item0 = workspace_item0->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());

  PopulateDomainWorkspace(*workspace_item0, domain_workspace0);
  PopulateDomainWorkspace(*workspace_item1, domain_workspace1);

  const WorkspaceItemListener listener0(workspace_item0, &domain_workspace0);
  const WorkspaceItemListener listener1(workspace_item1, &domain_workspace1);

  domain_workspace0.Setup();
  domain_workspace1.Setup();

  const sup::dto::AnyValue value0(sup::dto::SignedInteger32Type, 42);
  SetAnyValue(value0, *variable_item0); // FAILING HERE because second listener also reacts
}

}  // namespace oac_tree_gui::test
