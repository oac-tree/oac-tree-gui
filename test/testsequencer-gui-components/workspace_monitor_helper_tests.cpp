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

#include "sequencergui/pvmonitor/workspace_monitor_helper.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>

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

TEST_F(WorkspaceMonitorHelperTest, FindAncestor)
{
  WorkspaceItem workspace_item;
  auto var_item0 = workspace_item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, *var_item0);

  EXPECT_EQ(FindAncestor<WorkspaceItem>(nullptr), nullptr);
  EXPECT_EQ(FindAncestor<WorkspaceItem>(var_item0), &workspace_item);
  EXPECT_EQ(FindAncestor<LocalVariableItem>(var_item0->GetAnyValueItem()), var_item0);
  EXPECT_EQ(FindAncestor<WorkspaceItem>(var_item0->GetAnyValueItem()), &workspace_item);
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

  EXPECT_TRUE(var_item0->GetItem(domainconstants::kNameAttribute)->IsEditable());
  EXPECT_TRUE(var_item1->GetItem(domainconstants::kNameAttribute)->IsEditable());
  EXPECT_TRUE(var_item1->GetItem(domainconstants::kChannelAttribute)->IsEditable());

  UpdateVariableEditableProperty(/*is_running*/ true, workspace_item);
  EXPECT_FALSE(var_item0->GetItem(domainconstants::kNameAttribute)->IsEditable());
  EXPECT_FALSE(var_item1->GetItem(domainconstants::kNameAttribute)->IsEditable());
  EXPECT_FALSE(var_item1->GetItem(domainconstants::kChannelAttribute)->IsEditable());

  UpdateVariableEditableProperty(/*is_running*/ false, workspace_item);
  EXPECT_TRUE(var_item0->GetItem(domainconstants::kNameAttribute)->IsEditable());
  EXPECT_TRUE(var_item1->GetItem(domainconstants::kNameAttribute)->IsEditable());
  EXPECT_TRUE(var_item1->GetItem(domainconstants::kChannelAttribute)->IsEditable());
}

//! Validating helper method SetupNewVariable.

TEST_F(WorkspaceMonitorHelperTest, SetupNewVariable)
{
  WorkspaceItem workspace_item;
  auto var_item0 = workspace_item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  SetupNewVariable(var_item0);

  auto var_item1 = workspace_item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  SetupNewVariable(var_item1);

  EXPECT_EQ(var_item0->GetName(), "var0");
  EXPECT_EQ(var_item1->GetName(), "var1");

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
  SetupNewVariable(var_item2);

  // PvAccessClient variable is different, it shouldn't have any AnyValue from the start
  EXPECT_EQ(var_item2->GetName(), "var2");
  ASSERT_FALSE(var_item2->GetAnyValueItem());
}
