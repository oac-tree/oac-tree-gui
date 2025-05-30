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

#include "oac_tree_gui/nodeeditor/connectable_view_model_controller.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/epics_instruction_items.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/nodeeditor/connectable_instruction_adapter.h>
#include <oac_tree_gui/nodeeditor/connectable_view.h>
#include <oac_tree_gui/nodeeditor/node_connection.h>
#include <oac_tree_gui/nodeeditor/node_graphics_scene.h>
#include <oac_tree_gui/nodeeditor/node_port.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <mvvm/commands/command_stack.h>
#include <mvvm/commands/i_command_stack.h>
#include <mvvm/commands/macro_command.h>
#include <mvvm/standarditems/container_item.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

class ConnectableViewModelControllerTest : public ::testing::Test
{
public:
  ConnectableViewModelControllerTest() : m_scene([](auto item) { (void)item; })
  {
    m_procedure_item = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  InstructionContainerItem* GetContainer() { return m_procedure_item->GetInstructionContainer(); }

  std::vector<ConnectableView*> GetConnectedChildren(ConnectableView* parent)
  {
    std::vector<ConnectableView*> result;
    for (auto connection : parent->GetParentPort()->connections())
    {
      result.push_back(connection->childView());
    }
    return result;
  }

  std::unique_ptr<ConnectableViewModelController> CreateController()
  {
    auto result = std::make_unique<ConnectableViewModelController>(&m_model, &m_scene);
    result->Init(GetContainer());
    return result;
  }

  SequencerModel m_model;
  NodeGraphicsScene m_scene;
  ProcedureItem* m_procedure_item{nullptr};
};

//! Empty procedure and model.
TEST_F(ConnectableViewModelControllerTest, EmptyModel)
{
  ConnectableViewModelController controller(&m_model, &m_scene);

  EXPECT_EQ(m_scene.items().size(), 0);
}

TEST_F(ConnectableViewModelControllerTest, SingleInstruction)
{
  m_model.InsertItem<WaitItem>(GetContainer());

  auto controller = CreateController();

  EXPECT_EQ(m_scene.GetConnectableViews().size(), 1);
}

//! Checking views corresponding to the sequence with `wait` as a child.
TEST_F(ConnectableViewModelControllerTest, ProcedureWithSequenceAndWait)
{
  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  auto controller = CreateController();

  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);
  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  auto weight_view = m_scene.FindViewForInstruction(wait);
  auto children_views = GetConnectedChildren(sequence_view);
  EXPECT_EQ(children_views, std::vector<ConnectableView*>({weight_view}));
}

//! Scene is looking to empty instruction container. After insertion of one instruction
//! corresponding view should appear.
TEST_F(ConnectableViewModelControllerTest, InsertInstruction)
{
  auto controller = CreateController();

  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());

  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  EXPECT_EQ(m_scene.GetConnectableViews(), std::vector<ConnectableView*>({sequence_view}));
}

//! Scene is looking to empty procedure. After insertion of of wait into the sequence two connected
//! children should appear.
TEST_F(ConnectableViewModelControllerTest, InsertSequenceAndWait)
{
  auto controller = CreateController();

  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);
  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  auto wait_view = m_scene.FindViewForInstruction(wait);
  auto children_views = GetConnectedChildren(sequence_view);
  EXPECT_EQ(children_views, std::vector<ConnectableView*>({wait_view}));
}

//! Scene is looking to the procedure with single instruction. After its deletion, corresponding
//! view should dissapear.
TEST_F(ConnectableViewModelControllerTest, RemoveInstruction)
{
  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());

  auto controller = CreateController();

  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  EXPECT_EQ(m_scene.GetConnectableViews(), std::vector<ConnectableView*>({sequence_view}));

  m_model.RemoveItem(sequence);
  EXPECT_EQ(m_scene.GetConnectableViews().size(), 0);
}

//! Scene is looking to the procedure with sequence and single child in it. Sequence is deleted,
//! thish should lead to the disappearance of two vews.
TEST_F(ConnectableViewModelControllerTest, ParentWithChildRemoveChild)
{
  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  auto controller = CreateController();

  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  auto wait_view = m_scene.FindViewForInstruction(wait);
  EXPECT_EQ(sequence_view->GetConnectableItem()->GetInstruction(), sequence);
  EXPECT_EQ(wait_view->GetConnectableItem()->GetInstruction(), wait);

  m_model.RemoveItem(wait);
  EXPECT_EQ(m_scene.GetConnectableViews().size(), 1);

  auto sequence_view2 = m_scene.FindViewForInstruction(sequence);
  EXPECT_EQ(sequence_view2->GetConnectableItem()->GetInstruction(), sequence);
}

//! Scene is looking to the procedure with sequence and single child in it. Sequence is deleted,
//! thish should lead to the disappearance of two views.
TEST_F(ConnectableViewModelControllerTest, RemoveParentWithChild)
{
  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  auto controller = CreateController();

  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  auto wait_view = m_scene.FindViewForInstruction(wait);
  EXPECT_EQ(sequence_view->GetConnectableItem()->GetInstruction(), sequence);
  EXPECT_EQ(wait_view->GetConnectableItem()->GetInstruction(), wait);

  m_model.RemoveItem(sequence);
  EXPECT_EQ(m_scene.GetConnectableViews().size(), 0);
}

//! View is moving, check item.
TEST_F(ConnectableViewModelControllerTest, OnViewMove)
{
  // creating sequence with initial position
  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());
  sequence->SetX(42.0);
  EXPECT_DOUBLE_EQ(sequence->GetX(), 42.0);
  EXPECT_DOUBLE_EQ(sequence->GetY(), 0.0);

  auto controller = CreateController();

  // checking the view which was just created
  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  EXPECT_EQ(m_scene.GetConnectableViews(), std::vector<ConnectableView*>({sequence_view}));

  // checking its position
  EXPECT_EQ(sequence_view->x(), 42.0);
  EXPECT_EQ(sequence_view->y(), 0.0);

  // changing coordinates of item
  sequence_view->setX(43.0);

  EXPECT_DOUBLE_EQ(sequence->GetX(), 43.0);
  EXPECT_DOUBLE_EQ(sequence->GetY(), 0.0);
}

//! Enable undo/redo. View is moving, check item.
TEST_F(ConnectableViewModelControllerTest, OnViewMoveUndo)
{
  // creating sequence with initial position
  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());
  sequence->SetX(42.0);
  EXPECT_DOUBLE_EQ(sequence->GetX(), 42.0);
  EXPECT_DOUBLE_EQ(sequence->GetY(), 0.0);

  m_model.SetUndoEnabled(true);

  auto controller = CreateController();

  // checking the view which was just created
  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  EXPECT_EQ(m_scene.GetConnectableViews(), std::vector<ConnectableView*>({sequence_view}));

  // checking its position
  EXPECT_EQ(sequence_view->x(), 42.0);
  EXPECT_EQ(sequence_view->y(), 0.0);

  // changing coordinates of item
  sequence_view->setX(43.0);

  EXPECT_DOUBLE_EQ(sequence->GetX(), 43.0);
  EXPECT_DOUBLE_EQ(sequence->GetY(), 0.0);

  ASSERT_EQ(m_model.GetCommandStack()->GetCommandCount(), 1);
  EXPECT_NO_FATAL_FAILURE(m_model.GetCommandStack()->Undo());

  EXPECT_EQ(sequence->GetX(), 42.0);
  EXPECT_EQ(sequence->GetY(), 0.0);
}

//! Item is moving, check view.
TEST_F(ConnectableViewModelControllerTest, OnItemMove)
{
  // creating sequence with initial position
  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());

  auto controller = CreateController();

  // checking the view which was just created
  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  EXPECT_EQ(m_scene.GetConnectableViews(), std::vector<ConnectableView*>({sequence_view}));

  // checking its position
  EXPECT_EQ(sequence_view->x(), 0.0);
  EXPECT_EQ(sequence_view->y(), 0.0);

  // changing coordinates of item
  sequence->SetX(43.0);
  sequence->SetY(44.0);

  EXPECT_DOUBLE_EQ(sequence_view->x(), 43.0);
  EXPECT_DOUBLE_EQ(sequence_view->y(), 44.0);
}

//! Scene is looking to empty procedure. Insert second procedure, controller should ignore any
//! activity outside of its root item
TEST_F(ConnectableViewModelControllerTest, InsertProcedure)
{
  auto controller = CreateController();

  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);
  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  auto wait_view = m_scene.FindViewForInstruction(wait);
  auto children_views = GetConnectedChildren(sequence_view);
  EXPECT_EQ(children_views, std::vector<ConnectableView*>({wait_view}));

  // insert second procedure (outside of controller scope)
  auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  auto sequence2 = m_model.InsertItem<SequenceItem>(procedure->GetInstructionContainer());

  // removing second sequence
  EXPECT_NO_FATAL_FAILURE(m_model.RemoveItem(sequence2));

  // graphics scene should remain as before
  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);
  sequence_view = m_scene.FindViewForInstruction(sequence);
  wait_view = m_scene.FindViewForInstruction(wait);
  children_views = GetConnectedChildren(sequence_view);
  EXPECT_EQ(children_views, std::vector<ConnectableView*>({wait_view}));
}

//! Scene is looking to the procedure with instruction. Remove procedure and make sure that scene
//! has been cleaned up. Real bug.
TEST_F(ConnectableViewModelControllerTest, RemoveProcedure)
{
  auto controller = CreateController();

  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);
  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  auto wait_view = m_scene.FindViewForInstruction(wait);
  auto children_views = GetConnectedChildren(sequence_view);
  EXPECT_EQ(children_views, std::vector<ConnectableView*>({wait_view}));

  // remove procedure
  m_model.RemoveItem(m_procedure_item);

  // graphics scene should be cleaned up
  EXPECT_EQ(m_scene.GetConnectableViews().size(), 0);
}

//! Testing the case when AnyValue is replaced on board of PvAccessWriteInstructionItem. The
//! graphics controller should be happy with that and shouldn't try to create/remove instructions
//! related views.
TEST_F(ConnectableViewModelControllerTest, InsertAnyValueItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  auto controller = CreateController();

  auto instruction_item = m_model.InsertItem<PvAccessWriteInstructionItem>(GetContainer());

  auto instruction_view = m_scene.FindViewForInstruction(instruction_item);
  EXPECT_EQ(m_scene.GetConnectableViews(), std::vector<ConnectableView*>({instruction_view}));

  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);

  EXPECT_NO_THROW(SetAnyValue(expected_anyvalue, *instruction_item));
  EXPECT_EQ(m_scene.GetConnectableViews(), std::vector<ConnectableView*>({instruction_view}));
}

}  // namespace oac_tree_gui::test
