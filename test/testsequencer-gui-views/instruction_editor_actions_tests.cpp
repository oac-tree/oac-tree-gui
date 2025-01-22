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

#include "sequencergui/views/composer/instruction_editor_actions.h"

#include <gtest/gtest.h>

namespace sequencergui::test
{

//! Tests of InstructionEditorActionsTest class.

class InstructionEditorActionsTest : public ::testing::Test
{
};

TEST_F(InstructionEditorActionsTest, GetActions)
{
  using ActionKey = InstructionEditorActions::ActionKey;
  const InstructionEditorActions editor_actions(/*handler*/ nullptr);

  EXPECT_TRUE(editor_actions.GetActions({}).isEmpty());

  auto actions = editor_actions.GetActions({ActionKey::kInsertAfter, ActionKey::kInsertInto});
  ASSERT_EQ(actions.size(), 2);
}

TEST_F(InstructionEditorActionsTest, NoDuplicates)
{
  std::vector<InstructionEditorActions::ActionKey> all_action_keys;
  // iteration over enum class
  for (int i = 0; i < static_cast<int>(InstructionEditorActions::ActionKey::kTotalCount); ++i)
  {
    all_action_keys.push_back(static_cast<InstructionEditorActions::ActionKey>(i));
  }

  const InstructionEditorActions editor_actions(/*handler*/ nullptr);
  auto actions = editor_actions.GetActions(all_action_keys);
  EXPECT_EQ(actions.size(),
            static_cast<std::size_t>(InstructionEditorActions::ActionKey::kTotalCount));

  // checking duplicates
  const std::set<QAction*> action_set(actions.begin(), actions.end());
  EXPECT_EQ(action_set.size(), actions.size());
}

}  // namespace sequencergui::test
