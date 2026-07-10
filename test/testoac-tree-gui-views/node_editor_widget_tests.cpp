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

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/nodeeditor/sequencer_align_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/signals/model_listener.h>

#include <gtest/gtest.h>

#include <QApplication>

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
    auto sequence = m_model.InsertItem<SequenceItem>(container);
    (void)m_model.InsertItem<SequenceItem>(sequence);
    return procedure;
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
  EXPECT_NO_THROW((void)m_model.InsertItem<SequenceItem>(container));

  // the deferred alignment runs cleanly once the command has finished
  EXPECT_NO_THROW(QApplication::processEvents());
  EXPECT_FALSE(algorithm::RequiresInitialAlignment(container->GetInstructions()));
}

}  // namespace oac_tree_gui::test
