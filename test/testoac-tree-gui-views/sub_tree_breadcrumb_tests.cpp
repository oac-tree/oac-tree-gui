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

#include "oac_tree_gui/views/nodeeditor/sub_tree_breadcrumb.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/nodeeditor/objects/node_editor_scene_controller.h>
#include <oac_tree_gui/nodeeditor/objects/node_graphics_scene.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

#include <QLabel>
#include <QMenu>
#include <QToolButton>

namespace oac_tree_gui::test
{

//! Tests for SubTreeBreadcrumb class.
class SubTreeBreadcrumbTest : public ::testing::Test
{
public:
  SubTreeBreadcrumbTest()
      : m_controller(NodeEditorMode::kNodeEditor, &m_scene, [](const auto&) {})
      , m_breadcrumb(&m_controller)
  {
  }

  //! Inserts a named instruction.
  static InstructionItem* InsertNamed(const std::string& domain_type, const std::string& name,
                                      mvvm::SessionItem* parent)
  {
    auto result = InsertInstruction(domain_type, parent);
    result->SetName(name);
    return result;
  }

  //! Inserts a named Include with the given path.
  static InstructionItem* InsertInclude(const std::string& name, const std::string& path,
                                        mvvm::SessionItem* parent)
  {
    auto result = InsertNamed(domainconstants::kIncludeInstructionType, name, parent);
    result->SetAttribute(domainconstants::kPathAttribute, path);
    return result;
  }

  NodeGraphicsScene m_scene;
  mvvm::ApplicationModel m_model;
  NodeEditorSceneController m_controller;
  SubTreeBreadcrumb m_breadcrumb;
};

TEST_F(SubTreeBreadcrumbTest, InitialState)
{
  EXPECT_TRUE(m_breadcrumb.GetText().isEmpty());
}

//! Include hops extend the trail. A selector switch starts a new one. A click goes back.
TEST_F(SubTreeBreadcrumbTest, Trail)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto main = InsertNamed(domainconstants::kSequenceInstructionType, "Main", container);
  main->SetIsRootFlag(true);
  auto include_dispatch = InsertInclude("dispatch", "Dispatch", main);
  auto dispatch = InsertNamed(domainconstants::kSequenceInstructionType, "Dispatch", container);
  auto include_init = InsertInclude("init", "DoInit", dispatch);
  auto do_init = InsertNamed(domainconstants::kSequenceInstructionType, "DoInit", container);

  m_controller.SetProcedure(procedure);
  EXPECT_EQ(m_breadcrumb.GetText(), QString("<b>Main</b>"));

  m_controller.OpenSubTree(*include_dispatch);
  m_controller.OpenSubTree(*include_init);
  EXPECT_EQ(m_breadcrumb.GetText(), QString("<a href=\"0\">Main</a> › dispatch → <a "
                                            "href=\"1\">Dispatch</a> › init → <b>DoInit</b>"));

  auto label = m_breadcrumb.findChild<QLabel*>();
  emit label->linkActivated("1");
  EXPECT_EQ(m_controller.GetScope(), dispatch);
  EXPECT_EQ(m_breadcrumb.GetText(), QString("<a href=\"0\">Main</a> › dispatch → <b>Dispatch</b>"));

  m_controller.SetScope(do_init);
  EXPECT_EQ(m_breadcrumb.GetText(), QString("<b>DoInit</b>"));
}

//! Deleted instructions are shown as "(deleted)", without a link. The text is updated on the next
//! scope or history change.
TEST_F(SubTreeBreadcrumbTest, DeletedInstructions)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto main = InsertNamed(domainconstants::kSequenceInstructionType, "Main", container);
  main->SetIsRootFlag(true);
  auto include_sub = InsertInclude("sub", "Sub", main);
  auto sub = InsertNamed(domainconstants::kSequenceInstructionType, "Sub", container);
  auto include_other = InsertInclude("other", "Other", sub);
  (void)InsertNamed(domainconstants::kSequenceInstructionType, "Other", container);

  m_controller.SetProcedure(procedure);
  m_controller.OpenSubTree(*include_sub);
  m_model.RemoveItem(main);
  m_controller.OpenSubTree(*include_other);
  EXPECT_EQ(m_breadcrumb.GetText(), QString("(deleted) › (deleted) → <a href=\"1\">Sub</a> › "
                                            "other → <b>Other</b>"));
}

//! "Included by" lists Includes pointing at the current SubTree, grouped by top-level instruction.
//! A click shows and selects the Include.
TEST_F(SubTreeBreadcrumbTest, IncludedBy)
{
  m_controller.SetActive(true);  // a provider, to select on the scene
  auto button = m_breadcrumb.findChild<QToolButton*>();
  ASSERT_NE(button, nullptr);
  EXPECT_TRUE(button->isHidden());

  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto main = InsertNamed(domainconstants::kSequenceInstructionType, "Main", container);
  main->SetIsRootFlag(true);
  auto include_a = InsertInclude("a", "Sub", main);
  auto other = InsertNamed(domainconstants::kSequenceInstructionType, "Other", container);
  (void)InsertInclude("b", "Sub.Inner", other);
  auto sub = InsertNamed(domainconstants::kSequenceInstructionType, "Sub", container);
  (void)InsertNamed(domainconstants::kWaitInstructionType, "Inner", sub);
  (void)InsertInclude("c", "Sub", main);

  m_controller.SetProcedure(procedure);
  EXPECT_FALSE(button->isHidden());

  auto menu = button->menu();
  emit menu->aboutToShow();
  ASSERT_EQ(menu->actions().size(), 1);
  EXPECT_EQ(menu->actions().at(0)->text(), QString("No Includes point here"));
  EXPECT_FALSE(menu->actions().at(0)->isEnabled());

  m_controller.SetScope(sub);
  emit menu->aboutToShow();
  auto actions = menu->actions();
  ASSERT_EQ(actions.size(), 6);
  EXPECT_EQ(actions.at(0)->text(), QString("Main"));
  EXPECT_FALSE(actions.at(0)->isEnabled());
  EXPECT_TRUE(actions.at(0)->font().bold());
  EXPECT_EQ(actions.at(1)->text(), QString("a (Sub)"));
  EXPECT_EQ(actions.at(2)->text(), QString("c (Sub)"));
  EXPECT_TRUE(actions.at(3)->isSeparator());
  EXPECT_EQ(actions.at(4)->text(), QString("Other"));
  EXPECT_EQ(actions.at(5)->text(), QString("b (Sub.Inner)"));

  actions.at(1)->trigger();
  EXPECT_EQ(m_controller.GetScope(), main);
  EXPECT_EQ(m_controller.GetSelectedInstructions(), std::vector<InstructionItem*>({include_a}));
}

}  // namespace oac_tree_gui::test
