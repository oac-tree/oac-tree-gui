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
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/nodeeditor/sequencer_align_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/signals/model_listener.h>

#include <gtest/gtest.h>

#include <QAction>
#include <QApplication>
#include <QGraphicsItem>
#include <QScrollBar>

namespace oac_tree_gui::test
{

//! Tests for NodeEditorWidget class. Scope logic is tested in NodeEditorSceneController tests.
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

  //! Inserts a top-level instruction away from the origin, so it needs no alignment.
  static InstructionItem* InsertAt(const std::string& domain_type, mvvm::SessionItem* parent,
                                   double x, double y)
  {
    auto result = InsertInstruction(domain_type, parent);
    result->SetX(x);
    result->SetY(y);
    return result;
  }

  //! Returns the "Follow selection" action of the widget.
  static QAction* FindFollowSelectionAction(const NodeEditorWidget& widget)
  {
    for (auto action : widget.actions())
    {
      if (action->text() == QString("Follow selection"))
      {
        return action;
      }
    }
    return nullptr;
  }

  //! Returns the scene point in the center of the viewport.
  static QPointF GetViewportCenter(const NodeGraphicsView& view)
  {
    return view.mapToScene(view.viewport()->rect().center());
  }

  //! Checks if selected shapes are inside the viewport.
  static bool IsSelectionVisible(const NodeGraphicsView& view)
  {
    QRectF rect;
    for (const auto* item : view.scene()->selectedItems())
    {
      rect |= item->sceneBoundingRect();
    }
    return !rect.isNull() && view.mapToScene(view.viewport()->rect()).boundingRect().contains(rect);
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

//! With "Follow selection" on, a selection in a hidden SubTree switches the scope, and the
//! selection is centered. Off, only shown shapes are selected and the view doesn't move. An
//! instruction inside a collapsed parent selects the parent.
TEST_F(NodeEditorWidgetTest, FollowSelection)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, -5000.0, 1000.0);
  auto wait0 = InsertAt(domainconstants::kWaitInstructionType, sequence0, -2000.0, 1000.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 5000.0, 9000.0);
  auto wait1 = InsertAt(domainconstants::kWaitInstructionType, sequence1, 8000.0, 9000.0);

  NodeEditorWidget widget(NodeEditorMode::kNodeEditor);
  widget.resize(800, 600);
  widget.show();
  QApplication::processEvents();
  widget.SetProcedure(procedure);
  QApplication::processEvents();

  auto view = widget.findChild<NodeGraphicsView*>();
  auto follow_action = FindFollowSelectionAction(widget);
  ASSERT_NE(follow_action, nullptr);
  ASSERT_TRUE(follow_action->isChecked());
  ASSERT_EQ(widget.GetScope(), sequence0);

  // a hidden SubTree: switch, select, and keep the selection in view after the fit
  widget.SetSelectedInstructions({wait1});
  EXPECT_EQ(widget.GetScope(), sequence1);
  EXPECT_EQ(widget.GetSelectedInstructions(), std::vector<InstructionItem*>({wait1}));
  QApplication::processEvents();
  EXPECT_TRUE(IsSelectionVisible(*view));

  // the shown SubTree: an instruction outside the viewport is centered
  view->SetZoomFactor(1.0);
  view->centerOn(view->scene()->selectedItems().at(0));
  const auto center = GetViewportCenter(*view);
  widget.SetSelectedInstructions({sequence1});
  EXPECT_TRUE(IsSelectionVisible(*view));
  EXPECT_NE(GetViewportCenter(*view), center);

  // off: no switch, no view move
  follow_action->setChecked(false);
  const auto center_off = GetViewportCenter(*view);
  widget.SetSelectedInstructions({wait0});
  EXPECT_EQ(widget.GetScope(), sequence1);
  EXPECT_TRUE(widget.GetSelectedInstructions().empty());

  widget.SetSelectedInstructions({wait1});
  EXPECT_EQ(widget.GetSelectedInstructions(), std::vector<InstructionItem*>({wait1}));
  EXPECT_FALSE(IsSelectionVisible(*view));
  EXPECT_EQ(GetViewportCenter(*view), center_off);

  // a collapsed parent is selected instead of its child
  SetCollapsed(true, *sequence1);
  widget.SetSelectedInstructions({wait1});
  EXPECT_EQ(widget.GetSelectedInstructions(), std::vector<InstructionItem*>({sequence1}));
}

}  // namespace oac_tree_gui::test
