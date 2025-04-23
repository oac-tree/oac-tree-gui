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

#include "oac_tree_gui/views/composer/procedure_list_actions.h"

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Tests of ProcedureListActions class.

class ProcedureListActionsTest : public ::testing::Test
{
};

TEST_F(ProcedureListActionsTest, GetActions)
{
  using ActionKey = ProcedureListActions::ActionKey;
  ProcedureListActions procedure_actions;

  EXPECT_TRUE(procedure_actions.GetActions({}).isEmpty());

  auto actions1 = procedure_actions.GetActions({ActionKey::kCreateNew});
  ASSERT_EQ(actions1.size(), 1);

  auto actions2 = procedure_actions.GetActions({ActionKey::kRemoveSelected});
  ASSERT_EQ(actions2.size(), 1);

  EXPECT_NE(actions1.at(0), actions2.at(0));

  auto actions3 = procedure_actions.GetActions({ActionKey::kRemoveSelected, ActionKey::kCreateNew});
  ASSERT_EQ(actions3.size(), 2);

  EXPECT_EQ(actions3.at(0), actions2.at(0));
  EXPECT_EQ(actions3.at(1), actions1.at(0));
}

TEST_F(ProcedureListActionsTest, NoDuplicates)
{
  std::vector<ProcedureListActions::ActionKey> all_action_keys;
  // iteration over enum class
  for (int i = 0; i < static_cast<int>(ProcedureListActions::ActionKey::kTotalCount); ++i)
  {
    all_action_keys.push_back(static_cast<ProcedureListActions::ActionKey>(i));
  }

  ProcedureListActions procedure_actions;
  auto actions = procedure_actions.GetActions(all_action_keys);
  EXPECT_EQ(actions.size(), static_cast<std::size_t>(ProcedureListActions::ActionKey::kTotalCount));

  // checking duplicates
  std::set<QAction*> action_set(actions.begin(), actions.end());
  EXPECT_EQ(action_set.size(), actions.size());
}

}  // namespace oac_tree_gui::test
