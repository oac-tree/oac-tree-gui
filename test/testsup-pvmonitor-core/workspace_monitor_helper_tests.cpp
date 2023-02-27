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

#include "suppvmonitor/workspace_monitor_helper.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>

#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <testutils/gui_domain_utils.h>

using namespace suppvmonitor;

//! Tests for methods from workspace_monitor_helper.h.

class WorkspaceMonitorHelperTests : public ::testing::Test
{
};

TEST_F(WorkspaceMonitorHelperTests, PopulateDomainWorkspaceEmptyWorkspaces)
{
  const sequencergui::WorkspaceItem workspace_item;
  sup::sequencer::Workspace workspace;

  EXPECT_NO_THROW(PopulateDomainWorkspace(workspace_item, workspace));
  EXPECT_TRUE(workspace.VariableNames().empty());
}

TEST_F(WorkspaceMonitorHelperTests, PopulateDomainWorkspaceAttemptToPopulateNonEmptyWorkspace)
{
  const sequencergui::WorkspaceItem workspace_item;
  sup::sequencer::Workspace workspace;
  workspace.AddVariable("abc", sequencergui::DomainUtils::CreateDomainVariable(
                                   sequencergui::domainconstants::kLocalVariableType)
                                   .release());

  EXPECT_THROW(PopulateDomainWorkspace(workspace_item, workspace), sequencergui::LogicErrorException);
}

TEST_F(WorkspaceMonitorHelperTests, PopulateDomainWorkspaceSingleVariables)
{
  sequencergui::WorkspaceItem workspace_item;
  sup::sequencer::Workspace workspace;

  auto var_item0 =
      workspace_item.InsertItem<sequencergui::LocalVariableItem>(mvvm::TagIndex::Append());
  var_item0->SetName("var0");
  auto var_item1 =
      workspace_item.InsertItem<sequencergui::FileVariableItem>(mvvm::TagIndex::Append());
  var_item1->SetName("var1");
  var_item1->SetFileName("filename");

  PopulateDomainWorkspace(workspace_item, workspace);
  EXPECT_EQ(workspace.VariableNames(), std::vector<std::string>({"var0", "var1"}));

  auto domain_var0 = workspace.GetVariable("var0");
  auto domain_var1 = workspace.GetVariable("var1");
  EXPECT_EQ(domain_var0->GetName(), var_item0->GetName());
  EXPECT_EQ(domain_var1->GetName(), var_item1->GetName());
}
