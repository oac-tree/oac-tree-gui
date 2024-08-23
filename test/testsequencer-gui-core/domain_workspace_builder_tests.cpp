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

#include "sequencergui/transform/domain_workspace_builder.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_helper.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/transform_helpers.h>

#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Testing DomainWorkspaceBuilder class.

class DomainWorkspaceBuilderTest : public ::testing::Test
{
};

//! Populating from empty WorkspaceItem.

TEST_F(DomainWorkspaceBuilderTest, EmptyWorkspace)
{
  sup::sequencer::Workspace workspace;
  WorkspaceItem workspace_item;

  DomainWorkspaceBuilder builder;
  builder.PopulateDomainWorkspace(&workspace_item, &workspace);

  EXPECT_TRUE(workspace.VariableNames().empty());
  EXPECT_EQ(builder.GetVariableItemFromDomainVariableName("abc"), nullptr);
}

//! Attempt to populate non-empty workspace

TEST_F(DomainWorkspaceBuilderTest, AttemptToPopulateNonEmptyWorkspace)
{
  sup::sequencer::Workspace workspace;
  workspace.AddVariable("abc", CreateDomainVariable(domainconstants::kLocalVariableType));

  WorkspaceItem workspace_item;

  DomainWorkspaceBuilder builder;
  EXPECT_THROW(builder.PopulateDomainWorkspace(&workspace_item, &workspace), LogicErrorException);
}

//! Populating Workspace from WorkspaceItem with two variables.

TEST_F(DomainWorkspaceBuilderTest, WorkspaceWithVariable)
{
  sup::sequencer::Workspace workspace;
  WorkspaceItem workspace_item;

  auto var_item0 = workspace_item.InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var_item0->SetName("var0");

  sup::dto::AnyValue value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});

  SetAnyValue(value, *var_item0);
  auto var_item1 = workspace_item.InsertItem<FileVariableItem>(mvvm::TagIndex::Append());
  var_item1->SetName("var1");
  var_item1->SetFileName("aaa.txt");

  DomainWorkspaceBuilder builder;
  builder.PopulateDomainWorkspace(&workspace_item, &workspace);

  EXPECT_EQ(workspace.VariableNames(), std::vector<std::string>({"var0", "var1"}));
  EXPECT_EQ(builder.GetVariableItemFromDomainVariableName("var0"), var_item0);
  EXPECT_EQ(builder.GetVariableItemFromDomainVariableName("var1"), var_item1);

  auto domain_var0 = workspace.GetVariable("var0");
  auto domain_var1 = workspace.GetVariable("var1");
  EXPECT_EQ(domain_var0->GetName(), var_item0->GetName());
  EXPECT_EQ(domain_var1->GetName(), var_item1->GetName());

  // checking values
  workspace.Setup();
  sup::dto::AnyValue domain_value;
  domain_var0->GetValue(domain_value);
  EXPECT_EQ(domain_value, value);
}
