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

#include "oac_tree_gui/views/nodeeditor/node_editor_widget.h"
#include "oac_tree_gui/views/nodeeditor/node_graphics_view.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/nodeeditor/sequencer_align_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/signals/model_listener.h>

#include <gtest/gtest.h>
#include <testutils/test_node_editor_helper.h>

#include <QApplication>
#include <QScrollBar>

namespace oac_tree_gui::test
{

//! Tests for NodeEditorWidget class, focused on the deferred initial alignment.
class NodeEditorWidgetTest : public ::testing::Test
{
public:
  //! Builds a procedure (with an instruction sub-tree at the origin) inside the model.
  ProcedureItem* CreateProcedureWithInstructions()
  {
    auto procedure = m_model.InsertItem<ProcedureItem>();
    auto container = procedure->GetInstructionContainer();
    auto sequence = InsertInstruction(domainconstants::kSequenceInstructionType, container);
    (void)InsertInstruction(domainconstants::kSequenceInstructionType, sequence);
    return procedure;
  }

  //! Returns instructions with a shape on the scene of the widget.
  static std::vector<InstructionItem*> GetSceneInstructions(const NodeEditorWidget& widget)
  {
    return FindSceneInstructions(*widget.findChild<NodeGraphicsView*>()->scene());
  }

  //! Inserts a top-level instruction away from the origin, so it needs no alignment.
  static InstructionItem* InsertAt(const std::string& domain_type, mvvm::SessionItem* parent,
                                   double x, double y)
  {
    auto result = InsertInstruction(domain_type, parent);
    result->SetX(x);
    result->SetY(y);
    return result;
  }

  mvvm::ApplicationModel m_model;
};

//! Initial alignment mutates instruction coordinates and is therefore deferred to the next
//! event-loop cycle: it does not run synchronously from SetProcedure().
TEST_F(NodeEditorWidgetTest, InitialAlignmentIsDeferred)
{
  auto procedure = CreateProcedureWithInstructions();
  auto container = procedure->GetInstructionContainer();
  m_model.SetUndoEnabled(true);

  NodeEditorWidget widget(NodeEditorMode::kNodeViewer);
  widget.show();
  QApplication::processEvents();  // let the widget become visible

  ASSERT_TRUE(algorithm::RequiresInitialAlignment(container->GetInstructions()));

  widget.SetProcedure(procedure);

  // alignment has not happened yet - it is queued
  EXPECT_TRUE(algorithm::RequiresInitialAlignment(container->GetInstructions()));

  QApplication::processEvents();  // run the queued alignment

  // now the instructions have been aligned
  EXPECT_FALSE(algorithm::RequiresInitialAlignment(container->GetInstructions()));
}

//! Regression test: when SetProcedure() is triggered from inside a running model command (as
//! happens when a JobItemController pushes a regenerated expanded procedure into the widget during
//! an InsertItem command), the widget must not start a coordinate-changing command synchronously.
//! Doing so previously threw "new command before current one has finished". The alignment is now
//! deferred, so the command completes cleanly and the alignment runs afterwards.
TEST_F(NodeEditorWidgetTest, SetProcedureFromInsideCommandDoesNotThrow)
{
  auto procedure = CreateProcedureWithInstructions();
  auto container = procedure->GetInstructionContainer();
  m_model.SetUndoEnabled(true);

  NodeEditorWidget widget(NodeEditorMode::kNodeViewer);
  widget.show();
  QApplication::processEvents();

  // mimic JobItemController: push the procedure into the widget from within a model notification
  mvvm::ModelListener listener(&m_model);
  listener.Connect<mvvm::ItemInsertedEvent>(
      [&widget, procedure](const mvvm::ItemInsertedEvent&) { widget.SetProcedure(procedure); });

  // this insertion is a command; the listener reacts to it from inside the command
  EXPECT_NO_THROW((void)InsertInstruction(domainconstants::kSequenceInstructionType, container));

  // the deferred alignment runs cleanly once the command has finished
  EXPECT_NO_THROW(QApplication::processEvents());
  EXPECT_FALSE(algorithm::RequiresInitialAlignment(container->GetInstructions()));
}

//! The read-only viewer shows all top-level instructions.
TEST_F(NodeEditorWidgetTest, ViewerShowsAllTopLevelInstructions)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);

  NodeEditorWidget widget(NodeEditorMode::kNodeViewer);
  widget.show();
  QApplication::processEvents();
  widget.SetProcedure(procedure);

  EXPECT_EQ(widget.GetScope(), nullptr);
  EXPECT_EQ(GetSceneInstructions(widget), std::vector<InstructionItem*>({sequence0, sequence1}));
}

//! The default scope is the root. Switching the scope shows another SubTree.
TEST_F(NodeEditorWidgetTest, DefaultScopeAndSwitch)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
  auto wait0 = InsertAt(domainconstants::kWaitInstructionType, sequence0, 10.0, 20.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);
  sequence1->SetIsRootFlag(true);

  NodeEditorWidget widget(NodeEditorMode::kNodeEditor);
  widget.show();
  QApplication::processEvents();
  widget.SetProcedure(procedure);

  EXPECT_EQ(widget.GetScope(), sequence1);
  EXPECT_EQ(GetSceneInstructions(widget), std::vector<InstructionItem*>({sequence1}));

  widget.SetScope(sequence0);
  EXPECT_EQ(widget.GetScope(), sequence0);
  EXPECT_EQ(GetSceneInstructions(widget), std::vector<InstructionItem*>({sequence0, wait0}));
}

//! Removing the current SubTree falls back to the root on the next event-loop cycle.
TEST_F(NodeEditorWidgetTest, RemoveScope)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);

  NodeEditorWidget widget(NodeEditorMode::kNodeEditor);
  widget.show();
  QApplication::processEvents();
  widget.SetProcedure(procedure);
  widget.SetScope(sequence1);

  m_model.RemoveItem(sequence1);
  EXPECT_TRUE(GetSceneInstructions(widget).empty());

  QApplication::processEvents();
  EXPECT_EQ(widget.GetScope(), sequence0);
  EXPECT_EQ(GetSceneInstructions(widget), std::vector<InstructionItem*>({sequence0}));

  m_model.RemoveItem(sequence0);
  QApplication::processEvents();
  EXPECT_EQ(widget.GetScope(), nullptr);
  EXPECT_TRUE(GetSceneInstructions(widget).empty());
}

//! When the current SubTree is moved into another tree, the scope follows its new top-level.
TEST_F(NodeEditorWidgetTest, MoveScopeIntoAnotherTree)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);
  auto sequence2 = InsertAt(domainconstants::kSequenceInstructionType, container, 30.0, 30.0);

  NodeEditorWidget widget(NodeEditorMode::kNodeEditor);
  widget.show();
  QApplication::processEvents();
  widget.SetProcedure(procedure);
  widget.SetScope(sequence1);

  m_model.MoveItem(sequence1, sequence2, mvvm::TagIndex::Append(itemconstants::kChildInstructions));
  QApplication::processEvents();

  EXPECT_EQ(widget.GetScope(), sequence2);
  EXPECT_EQ(GetSceneInstructions(widget), std::vector<InstructionItem*>({sequence2, sequence1}));
}

//! In an empty procedure, an inserted instruction is shown.
TEST_F(NodeEditorWidgetTest, EmptyProcedure)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();

  NodeEditorWidget widget(NodeEditorMode::kNodeEditor);
  widget.show();
  QApplication::processEvents();
  widget.SetProcedure(procedure);
  EXPECT_EQ(widget.GetScope(), nullptr);

  auto wait = InsertAt(domainconstants::kWaitInstructionType, container, 10.0, 10.0);
  EXPECT_EQ(GetSceneInstructions(widget), std::vector<InstructionItem*>({wait}));
}

//! Initial alignment runs per SubTree, on its first show. An aligned SubTree is not realigned.
TEST_F(NodeEditorWidgetTest, InitialAlignmentPerSubTree)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertInstruction(domainconstants::kSequenceInstructionType, container);
  (void)InsertInstruction(domainconstants::kWaitInstructionType, sequence0);
  auto sequence1 = InsertInstruction(domainconstants::kSequenceInstructionType, container);
  (void)InsertInstruction(domainconstants::kWaitInstructionType, sequence1);

  NodeEditorWidget widget(NodeEditorMode::kNodeEditor);
  widget.show();
  QApplication::processEvents();
  widget.SetProcedure(procedure);
  QApplication::processEvents();

  EXPECT_FALSE(algorithm::RequiresInitialAlignment(*sequence0));
  EXPECT_TRUE(algorithm::RequiresInitialAlignment(*sequence1));

  sequence0->SetX(42.0);

  widget.SetScope(sequence1);
  QApplication::processEvents();
  EXPECT_FALSE(algorithm::RequiresInitialAlignment(*sequence1));

  widget.SetScope(sequence0);
  QApplication::processEvents();
  EXPECT_DOUBLE_EQ(sequence0->GetX(), 42.0);
}

//! A loose instruction is hidden when leaving its SubTree, and shown again on return.
TEST_F(NodeEditorWidgetTest, LooseInstruction)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);

  NodeEditorWidget widget(NodeEditorMode::kNodeEditor);
  widget.show();
  QApplication::processEvents();
  widget.SetProcedure(procedure);
  ASSERT_EQ(widget.GetScope(), sequence0);

  auto wait = InsertAt(domainconstants::kWaitInstructionType, container, 30.0, 30.0);
  EXPECT_EQ(GetSceneInstructions(widget), std::vector<InstructionItem*>({sequence0, wait}));

  widget.SetScope(sequence1);
  EXPECT_EQ(GetSceneInstructions(widget), std::vector<InstructionItem*>({sequence1}));

  widget.SetScope(sequence0);
  EXPECT_EQ(GetSceneInstructions(widget), std::vector<InstructionItem*>({sequence0, wait}));

  // after SetProcedure, the loose instruction is shown only as its own SubTree
  auto other_procedure = m_model.InsertItem<ProcedureItem>();
  widget.SetProcedure(other_procedure);
  widget.SetProcedure(procedure);
  EXPECT_EQ(GetSceneInstructions(widget), std::vector<InstructionItem*>({sequence0}));

  widget.SetScope(wait);
  EXPECT_EQ(GetSceneInstructions(widget), std::vector<InstructionItem*>({wait}));
}

//! After a switch, the view's scene rect is centered on the SubTree. Two SubTrees far apart get
//! the same scroll bar ranges.
TEST_F(NodeEditorWidgetTest, SceneRectPerSubTree)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, -5000.0, 1000.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 5000.0, 9000.0);

  NodeEditorWidget widget(NodeEditorMode::kNodeEditor);
  widget.resize(800, 600);
  widget.show();
  QApplication::processEvents();
  widget.SetProcedure(procedure);
  QApplication::processEvents();

  auto view = widget.findChild<NodeGraphicsView*>();
  auto range = [view]()
  {
    return std::make_pair(
        view->horizontalScrollBar()->maximum() - view->horizontalScrollBar()->minimum(),
        view->verticalScrollBar()->maximum() - view->verticalScrollBar()->minimum());
  };

  ASSERT_EQ(widget.GetScope(), sequence0);
  EXPECT_EQ(view->sceneRect().center(), view->scene()->itemsBoundingRect().center());
  EXPECT_DOUBLE_EQ(view->GetZoomFactor(), 1.0);
  const auto range0 = range();

  widget.SetScope(sequence1);
  QApplication::processEvents();
  EXPECT_EQ(view->sceneRect().center(), view->scene()->itemsBoundingRect().center());
  EXPECT_DOUBLE_EQ(view->GetZoomFactor(), 1.0);
  EXPECT_EQ(range(), range0);
}

}  // namespace oac_tree_gui::test
