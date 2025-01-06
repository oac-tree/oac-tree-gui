/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/pvmonitor/workspace_monitor_helper.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/jobsystem/domain_events.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/anyvalue_item_transform_helper.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>

#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for methods from workspace_monitor_helper.h.

class WorkspaceMonitorHelperTest : public ::testing::Test
{
};

TEST_F(WorkspaceMonitorHelperTest, PopulateDomainWorkspaceEmptyWorkspaces)
{
  const WorkspaceItem workspace_item;
  sup::sequencer::Workspace workspace;

  EXPECT_NO_THROW(PopulateDomainWorkspace(workspace_item, workspace));
  EXPECT_TRUE(workspace.VariableNames().empty());
}

TEST_F(WorkspaceMonitorHelperTest, PopulateDomainWorkspaceAttemptToPopulateNonEmptyWorkspace)
{
  const WorkspaceItem workspace_item;
  sup::sequencer::Workspace workspace;
  workspace.AddVariable("abc", CreateDomainVariable(domainconstants::kLocalVariableType));

  EXPECT_THROW(PopulateDomainWorkspace(workspace_item, workspace), LogicErrorException);
}

TEST_F(WorkspaceMonitorHelperTest, PopulateDomainWorkspaceSingleVariables)
{
  WorkspaceItem workspace_item;
  sup::sequencer::Workspace workspace;

  auto var_item0 = workspace_item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var_item0->SetName("var0");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, *var_item0);

  auto var_item1 = workspace_item.InsertItem<FileVariableItem>(mvvm::TagIndex::Append());
  var_item1->SetName("var1");
  var_item1->SetFileName("filename");

  PopulateDomainWorkspace(workspace_item, workspace);
  EXPECT_EQ(workspace.VariableNames(), std::vector<std::string>({"var0", "var1"}));

  auto domain_var0 = workspace.GetVariable("var0");
  auto domain_var1 = workspace.GetVariable("var1");
  EXPECT_EQ(domain_var0->GetName(), var_item0->GetName());
  EXPECT_EQ(domain_var1->GetName(), var_item1->GetName());
}

TEST_F(WorkspaceMonitorHelperTest, FindItemUp)
{
  WorkspaceItem workspace_item;
  auto var_item0 = workspace_item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, *var_item0);

  EXPECT_EQ(mvvm::utils::FindItemUp<WorkspaceItem>(nullptr), nullptr);
  EXPECT_EQ(mvvm::utils::FindItemUp<WorkspaceItem>(var_item0), &workspace_item);
  EXPECT_EQ(mvvm::utils::FindItemUp<LocalVariableItem>(var_item0->GetAnyValueItem()), var_item0);
  EXPECT_EQ(mvvm::utils::FindItemUp<WorkspaceItem>(var_item0->GetAnyValueItem()), &workspace_item);
}

TEST_F(WorkspaceMonitorHelperTest, UpdateVariableEditableProperty)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  WorkspaceItem workspace_item;
  sup::sequencer::Workspace workspace;

  auto var_item0 = workspace_item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  auto var_item1 = workspace_item.InsertItem<PvAccessServerVariableItem>(mvvm::TagIndex::Append());

  EXPECT_TRUE(var_item1->GetItem(domainconstants::kChannelAttribute)->IsEditable());

  UpdateVariableEditableProperty(/*is_running*/ true, workspace_item);
  EXPECT_FALSE(var_item1->GetItem(domainconstants::kChannelAttribute)->IsEditable());

  UpdateVariableEditableProperty(/*is_running*/ false, workspace_item);
  EXPECT_TRUE(var_item1->GetItem(domainconstants::kChannelAttribute)->IsEditable());
}

//! Validating helper method SetupNewVariable.
TEST_F(WorkspaceMonitorHelperTest, SetupNewVariable)
{
  WorkspaceItem workspace_item;
  auto var_item0 = workspace_item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  SetupNewVariable(var_item0, workspace_item.GetVariableCount());

  EXPECT_EQ(var_item0->GetAnyValueItem()->GetDisplayName(),
            itemconstants::kAnyValueDefaultDisplayName);

  auto var_item1 = workspace_item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  SetupNewVariable(var_item1, workspace_item.GetVariableCount());

  EXPECT_EQ(var_item0->GetName(), "var1");
  EXPECT_EQ(var_item1->GetName(), "var2");

  sup::dto::AnyValue expected_value(sup::dto::SignedInteger32Type, 0);

  ASSERT_TRUE(var_item0->GetAnyValueItem());
  EXPECT_EQ(sup::gui::CreateAnyValue(*var_item0->GetAnyValueItem()), expected_value);

  ASSERT_TRUE(var_item1->GetAnyValueItem());
  EXPECT_EQ(sup::gui::CreateAnyValue(*var_item1->GetAnyValueItem()), expected_value);

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  auto var_item2 = workspace_item.InsertItem<PvAccessClientVariableItem>(mvvm::TagIndex::Append());
  EXPECT_TRUE(GetIsAvailableItem(*var_item2)->IsVisible());
  SetupNewVariable(var_item2, workspace_item.GetVariableCount());

  // "connected" property should be invisible, when variable is constructed via editor
  EXPECT_FALSE(GetIsAvailableItem(*var_item2)->IsVisible());

  // PvAccessClient variable is different, it shouldn't have just empty AnyValue
  EXPECT_EQ(var_item2->GetName(), "var3");
  ASSERT_NE(var_item2->GetAnyValueItem(), nullptr);
  EXPECT_TRUE(sup::dto::IsEmptyValue(sup::gui::CreateAnyValue(*var_item2->GetAnyValueItem())));
  EXPECT_EQ(var_item2->GetAnyValueItem()->GetDisplayName(),
            itemconstants::kAnyValueDefaultDisplayName);
}

TEST_F(WorkspaceMonitorHelperTest, AreMatchingWorkspaces)
{
  sup::sequencer::Workspace workspace;
  WorkspaceItem workspace_item;

  EXPECT_TRUE(AreMatchingWorkspaces(workspace_item, workspace));

  auto item0 = workspace_item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  item0->SetDisplayName("abc");
  auto item1 = workspace_item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  item1->SetDisplayName("def");

  PopulateDomainWorkspace(workspace_item, workspace);
  EXPECT_TRUE(AreMatchingWorkspaces(workspace_item, workspace));

  item0->SetDisplayName("qwe");
  EXPECT_FALSE(AreMatchingWorkspaces(workspace_item, workspace));
}

TEST_F(WorkspaceMonitorHelperTest, UpdateVariableFromEvent)
{
  sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);

  LocalVariableItem variable_item;

  // initially VariableItem doesn't have AnyValueItem
  EXPECT_EQ(variable_item.GetAnyValueItem(), nullptr);

  size_t unused_variable_index{0};
  UpdateVariableFromEvent(VariableUpdatedEvent{unused_variable_index, value, true}, variable_item);

  EXPECT_TRUE(variable_item.IsAvailable());
  EXPECT_EQ(value, GetAnyValue(variable_item));
}
