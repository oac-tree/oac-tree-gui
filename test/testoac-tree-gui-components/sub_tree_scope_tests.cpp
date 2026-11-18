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

#include "oac_tree_gui/nodeeditor/objects/sub_tree_scope.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

namespace oac_tree_gui::test
{

//! Tests for SubTreeScope class.
class SubTreeScopeTest : public ::testing::Test
{
public:
  SubTreeScopeTest()
  {
    m_container = m_model.InsertItem<ProcedureItem>()->GetInstructionContainer();
  }

  //! Inserts a top-level Sequence.
  InstructionItem* InsertSequence()
  {
    return InsertInstruction(domainconstants::kSequenceInstructionType, m_container);
  }

  //! Returns identifiers of the given instructions.
  static std::set<std::string> GetIdentifiers(const std::vector<const InstructionItem*>& items)
  {
    std::set<std::string> result;
    for (const auto* item : items)
    {
      (void)result.insert(item->GetIdentifier());
    }
    return result;
  }

  mvvm::ApplicationModel m_model;
  InstructionContainerItem* m_container{nullptr};
};

TEST_F(SubTreeScopeTest, InitialState)
{
  SubTreeScope scope;
  EXPECT_EQ(scope.GetScope(), nullptr);
  EXPECT_TRUE(scope.GetHiddenIdentifiers().empty());

  auto sequence = InsertSequence();
  EXPECT_FALSE(scope.SetScope(sequence));
  EXPECT_EQ(scope.GetScope(), nullptr);
}

//! Reset shows the given SubTree and hides the others.
TEST_F(SubTreeScopeTest, Reset)
{
  auto sequence0 = InsertSequence();
  auto sequence1 = InsertSequence();
  auto sequence2 = InsertSequence();

  SubTreeScope scope;
  const QSignalSpy spy_changed(&scope, &SubTreeScope::scopeChanged);

  scope.Reset(m_container, sequence1);
  EXPECT_EQ(scope.GetScope(), sequence1);
  EXPECT_EQ(scope.GetHiddenIdentifiers(), GetIdentifiers({sequence0, sequence2}));
  EXPECT_EQ(spy_changed.count(), 1);
}

//! Switching shows another SubTree. The same SubTree again changes nothing.
TEST_F(SubTreeScopeTest, SetScope)
{
  auto sequence0 = InsertSequence();
  auto sequence1 = InsertSequence();

  SubTreeScope scope;
  scope.Reset(m_container, sequence0);
  const QSignalSpy spy_changed(&scope, &SubTreeScope::scopeChanged);

  EXPECT_TRUE(scope.SetScope(sequence1));
  EXPECT_EQ(scope.GetScope(), sequence1);
  EXPECT_EQ(scope.GetHiddenIdentifiers(), GetIdentifiers({sequence0}));
  EXPECT_EQ(spy_changed.count(), 1);

  EXPECT_FALSE(scope.SetScope(sequence1));
  EXPECT_EQ(spy_changed.count(), 1);

  EXPECT_TRUE(scope.SetScope(sequence0));
  EXPECT_EQ(scope.GetHiddenIdentifiers(), GetIdentifiers({sequence1}));
  EXPECT_EQ(spy_changed.count(), 2);

  EXPECT_TRUE(scope.SetScope(nullptr));
  EXPECT_EQ(scope.GetScope(), nullptr);
  EXPECT_EQ(scope.GetHiddenIdentifiers(), GetIdentifiers({sequence0, sequence1}));
  EXPECT_EQ(spy_changed.count(), 3);
}

//! A loose instruction is hidden when leaving its SubTree, and shown again on return. Reset
//! forgets it.
TEST_F(SubTreeScopeTest, LooseInstruction)
{
  auto sequence0 = InsertSequence();
  auto sequence1 = InsertSequence();

  SubTreeScope scope;
  scope.Reset(m_container, sequence0);

  EXPECT_EQ(scope.GetScope(), sequence0);
  EXPECT_EQ(scope.GetHiddenIdentifiers(), GetIdentifiers({sequence1}));

  auto loose = InsertInstruction(domainconstants::kWaitInstructionType, m_container);

  // last added "loose" instruction is not in the list of hidden identifiers, so it is visible
  EXPECT_EQ(scope.GetHiddenIdentifiers(), GetIdentifiers({sequence1}));

  (void)scope.SetScope(sequence1);
  EXPECT_EQ(scope.GetHiddenIdentifiers(), GetIdentifiers({sequence0, loose}));

  (void)scope.SetScope(sequence0);
  EXPECT_EQ(scope.GetHiddenIdentifiers(), GetIdentifiers({sequence1}));

  scope.Reset(m_container, sequence0);
  EXPECT_EQ(scope.GetHiddenIdentifiers(), GetIdentifiers({sequence1, loose}));
}

//! In an empty container, an inserted instruction is not hidden.
TEST_F(SubTreeScopeTest, EmptyContainer)
{
  SubTreeScope scope;
  const QSignalSpy spy_changed(&scope, &SubTreeScope::scopeChanged);
  scope.Reset(m_container, nullptr);
  EXPECT_EQ(scope.GetScope(), nullptr);
  EXPECT_TRUE(scope.GetHiddenIdentifiers().empty());

  (void)InsertSequence();
  EXPECT_TRUE(scope.GetHiddenIdentifiers().empty());
  EXPECT_EQ(scope.GetScope(), nullptr);
  EXPECT_EQ(spy_changed.count(), 0);
}

//! Removing the current SubTree leaves no scope. Undo restores it.
TEST_F(SubTreeScopeTest, RemoveScope)
{
  auto sequence0 = InsertSequence();
  auto sequence1 = InsertSequence();
  const auto identifier = sequence1->GetIdentifier();
  m_model.SetUndoEnabled(true);

  SubTreeScope scope;
  scope.Reset(m_container, sequence1);
  const QSignalSpy spy_changed(&scope, &SubTreeScope::scopeChanged);

  m_model.RemoveItem(sequence1);
  EXPECT_EQ(scope.GetScope(), nullptr);
  EXPECT_EQ(spy_changed.count(), 1);

  m_model.GetCommandStack()->Undo();
  ASSERT_NE(scope.GetScope(), nullptr);
  EXPECT_EQ(scope.GetScope()->GetIdentifier(), identifier);
  EXPECT_EQ(spy_changed.count(), 2);

  // removing a hidden instruction changes nothing
  m_model.RemoveItem(sequence0);
  EXPECT_EQ(scope.GetScope()->GetIdentifier(), identifier);
  EXPECT_EQ(spy_changed.count(), 2);
}

//! When the current SubTree is connected under a loose instruction, the scope follows it. Loose
//! instructions are recorded for the new top-level.
TEST_F(SubTreeScopeTest, MoveScopeUnderLooseInstruction)
{
  auto sequence0 = InsertSequence();
  auto sequence1 = InsertSequence();

  SubTreeScope scope;
  scope.Reset(m_container, sequence0);
  const QSignalSpy spy_changed(&scope, &SubTreeScope::scopeChanged);

  auto loose_sequence = InsertSequence();
  m_model.MoveItem(sequence0, loose_sequence,
                   mvvm::TagIndex::Append(itemconstants::kChildInstructions));

  EXPECT_EQ(scope.GetScope(), loose_sequence);
  EXPECT_EQ(spy_changed.count(), 2);  // no scope after the take, then the new top-level

  auto wait = InsertInstruction(domainconstants::kWaitInstructionType, m_container);

  (void)scope.SetScope(sequence1);
  EXPECT_EQ(scope.GetHiddenIdentifiers(), GetIdentifiers({loose_sequence, wait}));

  (void)scope.SetScope(loose_sequence);
  EXPECT_EQ(scope.GetHiddenIdentifiers(), GetIdentifiers({sequence1}));
}

//! When the current SubTree is moved into a hidden tree, there is no scope.
TEST_F(SubTreeScopeTest, MoveScopeIntoHiddenTree)
{
  (void)InsertSequence();
  auto sequence1 = InsertSequence();
  auto sequence2 = InsertSequence();

  SubTreeScope scope;
  scope.Reset(m_container, sequence1);
  const QSignalSpy spy_changed(&scope, &SubTreeScope::scopeChanged);

  m_model.MoveItem(sequence1, sequence2, mvvm::TagIndex::Append(itemconstants::kChildInstructions));
  EXPECT_EQ(scope.GetScope(), nullptr);
  EXPECT_EQ(spy_changed.count(), 1);

  EXPECT_TRUE(scope.SetScope(sequence2));
  EXPECT_EQ(scope.GetScope(), sequence2);
}

//! Clear removes the container and stops listening to the model.
TEST_F(SubTreeScopeTest, Clear)
{
  auto sequence0 = InsertSequence();
  auto sequence1 = InsertSequence();

  SubTreeScope scope;
  scope.Reset(m_container, sequence0);
  const QSignalSpy spy_changed(&scope, &SubTreeScope::scopeChanged);

  scope.Clear();
  EXPECT_EQ(scope.GetScope(), nullptr);
  EXPECT_TRUE(scope.GetHiddenIdentifiers().empty());
  EXPECT_EQ(spy_changed.count(), 1);

  EXPECT_FALSE(scope.SetScope(sequence1));
  m_model.RemoveItem(sequence0);
  EXPECT_EQ(spy_changed.count(), 1);
}

}  // namespace oac_tree_gui::test
