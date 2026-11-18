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

}  // namespace oac_tree_gui::test
