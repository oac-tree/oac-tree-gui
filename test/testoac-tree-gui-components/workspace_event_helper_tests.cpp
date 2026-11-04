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

#include "oac_tree_gui/jobsystem/workspace_event_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/jobsystem/domain_events.h>
#include <oac_tree_gui/model/variable_item.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/transform/variable_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

namespace oac_tree_gui::test
{

//! Tests for methods from workspace_event_helper.h.

class WorkspaceEventHelperTest : public ::testing::Test
{
};

TEST_F(WorkspaceEventHelperTest, AreMatchingWorkspaces)
{
  sup::oac_tree::Workspace workspace;
  WorkspaceItem workspace_item;

  EXPECT_TRUE(AreMatchingWorkspaces(workspace_item, workspace));

  auto item0 = InsertVariable(domainconstants::kLocalVariableType, &workspace_item);
  item0->SetDisplayName("abc");
  auto item1 = InsertVariable(domainconstants::kLocalVariableType, &workspace_item);
  item1->SetDisplayName("def");
  EXPECT_FALSE(AreMatchingWorkspaces(workspace_item, workspace));

  workspace.AddVariable("abc", CreateDomainVariable(domainconstants::kLocalVariableType));
  workspace.AddVariable("def", CreateDomainVariable(domainconstants::kLocalVariableType));
  EXPECT_TRUE(AreMatchingWorkspaces(workspace_item, workspace));

  item0->SetDisplayName("qwe");
  EXPECT_FALSE(AreMatchingWorkspaces(workspace_item, workspace));
}

TEST_F(WorkspaceEventHelperTest, UpdateVariableFromEvent)
{
  const sup::dto::AnyValue value(sup::dto::SignedInteger32Type, 42);

  auto variable_item = CreateVariableItem(domainconstants::kLocalVariableType);

  // initially VariableItem has an empty AnyValueItem
  EXPECT_EQ(variable_item->GetAnyValueItem()->GetType(),
            mvvm::GetTypeName<sup::gui::AnyValueEmptyItem>());

  const std::size_t unused_variable_index{0};
  UpdateVariableFromEvent(VariableUpdatedEvent{unused_variable_index, value, true}, *variable_item);

  EXPECT_TRUE(variable_item->IsAvailable());
  EXPECT_EQ(value, GetAnyValue(*variable_item));
}

}  // namespace oac_tree_gui::test
