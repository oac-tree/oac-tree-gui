/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "oac_tree_gui/pvmonitor/workspace_monitor_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>

#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

namespace oac_tree_gui::test
{

//! Tests for methods from workspace_monitor_helper.h.

class WorkspaceMonitorHelperTest : public ::testing::Test
{
};

TEST_F(WorkspaceMonitorHelperTest, PopulateDomainWorkspaceEmptyWorkspaces)
{
  const WorkspaceItem workspace_item;
  sup::oac_tree::Workspace workspace;

  EXPECT_NO_THROW(PopulateDomainWorkspace(workspace_item, workspace));
  EXPECT_TRUE(workspace.VariableNames().empty());
}

TEST_F(WorkspaceMonitorHelperTest, PopulateDomainWorkspaceAttemptToPopulateNonEmptyWorkspace)
{
  const WorkspaceItem workspace_item;
  sup::oac_tree::Workspace workspace;
  workspace.AddVariable("abc", CreateDomainVariable(domainconstants::kLocalVariableType));

  EXPECT_THROW(PopulateDomainWorkspace(workspace_item, workspace), LogicErrorException);
}

TEST_F(WorkspaceMonitorHelperTest, PopulateDomainWorkspaceSingleVariables)
{
  WorkspaceItem workspace_item;
  sup::oac_tree::Workspace workspace;

  auto var_item0 = InsertVariable(domainconstants::kLocalVariableType, &workspace_item);
  var_item0->SetName("var0");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, *var_item0);

  auto var_item1 = InsertVariable(domainconstants::kFileVariableType, &workspace_item);
  var_item1->SetName("var1");
  var_item1->SetAttribute(domainconstants::kFileNameAttribute, std::string("filename"));

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
  auto var_item0 = InsertVariable(domainconstants::kLocalVariableType, &workspace_item);
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, *var_item0);

  EXPECT_EQ(mvvm::utils::FindItemUp<WorkspaceItem>(nullptr), nullptr);
  EXPECT_EQ(mvvm::utils::FindItemUp<WorkspaceItem>(var_item0), &workspace_item);
  EXPECT_EQ(mvvm::utils::FindItemUp<VariableItem>(var_item0->GetAnyValueItem()), var_item0);
  EXPECT_EQ(mvvm::utils::FindItemUp<WorkspaceItem>(var_item0->GetAnyValueItem()), &workspace_item);
}

}  // namespace oac_tree_gui::test
