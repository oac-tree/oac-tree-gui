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

#include "oac_tree_gui/views/nodeeditor/sub_tree_navigation_actions.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/nodeeditor/objects/node_editor_scene_controller.h>
#include <oac_tree_gui/nodeeditor/objects/node_graphics_scene.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

#include <QMenu>
#include <QToolButton>
#include <QWidgetAction>

namespace oac_tree_gui::test
{

//! Tests for SubTreeNavigationActions class.
class SubTreeNavigationActionsTest : public ::testing::Test
{
public:
  SubTreeNavigationActionsTest()
      : m_controller(NodeEditorMode::kNodeEditor, &m_scene, [](const auto&) {})
      , m_actions(&m_controller)
  {
  }

  //! Returns the selector button.
  QToolButton* GetButton() const
  {
    auto action = dynamic_cast<QWidgetAction*>(m_actions.GetActions().at(0));
    return dynamic_cast<QToolButton*>(action->defaultWidget());
  }

  //! Fills the selector menu, as on opening, and returns it.
  QMenu* ShowMenu() const
  {
    auto menu = GetButton()->menu();
    emit menu->aboutToShow();
    return menu;
  }

  //! Inserts a named top-level instruction.
  InstructionItem* InsertNamed(const std::string& name, mvvm::SessionItem* parent)
  {
    auto result = InsertInstruction(domainconstants::kSequenceInstructionType, parent);
    result->SetName(name);
    return result;
  }

  NodeGraphicsScene m_scene;
  mvvm::ApplicationModel m_model;
  NodeEditorSceneController m_controller;
  SubTreeNavigationActions m_actions;
};

TEST_F(SubTreeNavigationActionsTest, InitialState)
{
  EXPECT_EQ(m_actions.GetActions().size(), 1);
  ASSERT_NE(GetButton(), nullptr);
  EXPECT_EQ(GetButton()->text(), QString("(none)"));
  EXPECT_TRUE(ShowMenu()->actions().empty());
}

//! The root comes first, in bold, followed by a separator. The current SubTree is checked.
TEST_F(SubTreeNavigationActionsTest, MenuContent)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  (void)InsertNamed("first", container);
  auto root = InsertNamed("root", container);
  root->SetIsRootFlag(true);
  (void)InsertNamed("last", container);

  m_controller.SetProcedure(procedure);
  EXPECT_EQ(GetButton()->text(), QString("root"));

  auto actions = ShowMenu()->actions();
  ASSERT_EQ(actions.size(), 4);
  EXPECT_EQ(actions.at(0)->text(), QString("root"));
  EXPECT_TRUE(actions.at(0)->font().bold());
  EXPECT_TRUE(actions.at(0)->isChecked());
  EXPECT_TRUE(actions.at(1)->isSeparator());
  EXPECT_EQ(actions.at(2)->text(), QString("first"));
  EXPECT_FALSE(actions.at(2)->font().bold());
  EXPECT_FALSE(actions.at(2)->isChecked());
  EXPECT_EQ(actions.at(3)->text(), QString("last"));
}

//! Triggering a menu entry switches the scope and updates the button text.
TEST_F(SubTreeNavigationActionsTest, TriggerEntry)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto first = InsertNamed("first", container);
  auto second = InsertNamed("second", container);

  m_controller.SetProcedure(procedure);
  ASSERT_EQ(m_controller.GetScope(), first);

  // without isRoot, the first instruction is the root: first, separator, second
  ShowMenu()->actions().at(2)->trigger();
  EXPECT_EQ(m_controller.GetScope(), second);
  EXPECT_EQ(GetButton()->text(), QString("second"));

  auto actions = ShowMenu()->actions();
  EXPECT_FALSE(actions.at(0)->isChecked());
  EXPECT_TRUE(actions.at(2)->isChecked());
}

//! The button text follows the model.
TEST_F(SubTreeNavigationActionsTest, TextFollowsModel)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto first = InsertNamed("first", container);

  m_controller.SetProcedure(procedure);
  ASSERT_EQ(GetButton()->text(), QString("first"));

  m_model.RemoveItem(first);
  EXPECT_EQ(GetButton()->text(), QString("(none)"));
}

}  // namespace oac_tree_gui::test
