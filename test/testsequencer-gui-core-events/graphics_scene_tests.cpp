/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/nodeeditor/graphics_scene.h"

#include <gtest/gtest.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/nodeeditor/connectable_instruction_adapter.h>
#include <sequencergui/nodeeditor/connectable_view.h>
#include <sequencergui/nodeeditor/graphics_scene_controller.h>
#include <sequencergui/nodeeditor/node_connection.h>
#include <sequencergui/nodeeditor/node_port.h>
#include <testutils/mock_message_handler.h>

#include <mvvm/core/exceptions.h>
#include <mvvm/model/model_utils.h>

#include <QSignalSpy>
#include <QTest>

using namespace sequencergui;
using ::testing::_;

//! Tests for GraphicsScene class. Supplements graphicscontroller.test.cpp

class GraphicsSceneTest : public ::testing::Test
{
public:
  GraphicsSceneTest()
  {
    m_model.InsertItem<ProcedureItem>();
    m_scene.SetInstructionContainer(GetInstructionContainer());
  }

  InstructionContainerItem* GetInstructionContainer()
  {
    return ::mvvm::utils::GetTopItem<ProcedureItem>(&m_model)->GetInstructionContainer();
  }

  std::vector<ConnectableView*> GetConnectedChildren(ConnectableView* parent)
  {
    std::vector<ConnectableView*> result;
    for (auto connection : parent->GetParentPort()->connections())
    {
      result.push_back(connection->childView());
    }
    return result;
  }

  std::unique_ptr<GraphicsSceneController> CreateController()
  {
    auto result = std::make_unique<GraphicsSceneController>(&m_model, &m_scene);
    result->Init(GetInstructionContainer());
    return result;
  }

  template <typename C>
  C Sorted(const C& container)
  {
    C result = container;
    std::sort(result.begin(), result.end());
    return result;
  }

  SequencerModel m_model;
  GraphicsScene m_scene;
};

TEST_F(GraphicsSceneTest, InitialState)
{
  GraphicsScene scene;
  EXPECT_FALSE(scene.HasContext());
}

//! Scene with single instruction. Make it selected and emit delete request.

TEST_F(GraphicsSceneTest, onDeleteSelectedRequest)
{
  auto sequence = m_model.InsertItem<SequenceItem>(GetInstructionContainer());

  auto controller = CreateController();

  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  ASSERT_TRUE(sequence_view != nullptr);
  sequence_view->setSelected(true);

  // deleting view
  m_scene.OnDeleteSelectedRequest();
  EXPECT_TRUE(m_scene.FindViewForInstruction(sequence) == nullptr);

  // the model should be empty too
  EXPECT_EQ(GetInstructionContainer()->GetTotalItemCount(), 0);
}

//! Scene with two instructions. One gets connected with another.

TEST_F(GraphicsSceneTest, onConnectionRequest)
{
  auto controller = CreateController();

  auto sequence = m_model.InsertItem<SequenceItem>(GetInstructionContainer());
  auto wait = m_model.InsertItem<WaitItem>(GetInstructionContainer());

  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);

  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  ASSERT_TRUE(sequence_view != nullptr);
  auto wait_view = m_scene.FindViewForInstruction(wait);
  ASSERT_TRUE(wait_view != nullptr);

  // connecting two views
  m_scene.onConnectionRequest(wait_view, sequence_view);
  QTest::qWait(200);

  // still two connectable view
  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);

  // sequence view should be as before
  EXPECT_EQ(sequence_view, m_scene.FindViewForInstruction(sequence));

  // but the wait view was regenerated
  auto new_wait_view = m_scene.FindViewForInstruction(wait);
  //  EXPECT_NE(wait_view, new_wait_view); // FIXME failing in the container

  // sequence now connected with new wait_view
  auto children_views = GetConnectedChildren(sequence_view);
  EXPECT_EQ(children_views, std::vector<ConnectableView*>({new_wait_view}));

  // the model should have sequence as a top instruciton, and wait into it
  EXPECT_EQ(GetInstructionContainer()->GetTotalItemCount(), 1);
  EXPECT_EQ(sequence->GetInstructions(), std::vector<InstructionItem*>({wait}));
}

//! Scene with two instructions not intended for connection. One tries to get connected with
//! another.

TEST_F(GraphicsSceneTest, OnInvalidConnectionRequest)
{
  auto controller = CreateController();

  auto wait0 = m_model.InsertItem<WaitItem>(GetInstructionContainer());
  auto wait1 = m_model.InsertItem<WaitItem>(GetInstructionContainer());

  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);

  auto view0 = m_scene.FindViewForInstruction(wait0);
  ASSERT_TRUE(view0 != nullptr);
  auto view1 = m_scene.FindViewForInstruction(wait1);
  ASSERT_TRUE(view1 != nullptr);

  // attempt to connect to leaves together
  EXPECT_THROW(m_scene.onConnectionRequest(view0, view1), mvvm::InvalidOperationException);

  testutils::MockMessageHandler mock_handler;
  m_scene.SetMessageHandler(CreateMessageHandlerDecorator(&mock_handler));

  // after handler set, we expect no throws; handler method should be called
  EXPECT_CALL(mock_handler, SendMessage(_)).Times(1);
  EXPECT_NO_THROW(m_scene.onConnectionRequest(view0, view1));
}

//! Scene with two instructions. One connected with the another, when we delete connection.
//! Children instruction should move on top of the container.

TEST_F(GraphicsSceneTest, onConnectionDeletionViaDisconnect)
{
  auto controller = CreateController();

  auto sequence = m_model.InsertItem<SequenceItem>(GetInstructionContainer());
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);

  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  ASSERT_TRUE(sequence_view != nullptr);
  auto wait_view = m_scene.FindViewForInstruction(wait);
  ASSERT_TRUE(wait_view != nullptr);

  // sequence now connected with new wait_view
  auto children_views = GetConnectedChildren(sequence_view);
  EXPECT_EQ(children_views, std::vector<ConnectableView*>({wait_view}));

  // selecting and deleting connection
  auto connections = sequence_view->GetParentPort()->connections();
  ASSERT_EQ(connections.size(), 1);

  // deleting connection
  m_scene.disconnectConnectedViews(connections.at(0));
  QTest::qWait(200);

  // new sequence view is the same, wait view is different
  EXPECT_EQ(sequence_view, m_scene.FindViewForInstruction(sequence));
  auto new_wait_view = m_scene.FindViewForInstruction(wait);
  //  ASSERT_NE(wait_view, new_wait_view); // FIXME failing in the container

  // there is no connections anymore
  EXPECT_EQ(GetConnectedChildren(sequence_view).size(), 0);

  // procedure contains two instructions
  EXPECT_EQ(GetInstructionContainer()->GetTotalItemCount(), 2);
  EXPECT_EQ(GetInstructionContainer()->GetInstructions(),
            std::vector<InstructionItem*>({sequence, wait}));
}

//! Scene with two instructions. One connected with the another, when we delete child.

TEST_F(GraphicsSceneTest, onDeleteSelectedChild)
{
  auto controller = CreateController();

  auto sequence = m_model.InsertItem<SequenceItem>(GetInstructionContainer());
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);

  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  ASSERT_TRUE(sequence_view != nullptr);
  auto wait_view = m_scene.FindViewForInstruction(wait);
  ASSERT_TRUE(wait_view != nullptr);

  EXPECT_EQ(sequence_view->GetConnectableItem()->GetInstruction(), sequence);
  EXPECT_EQ(wait_view->GetConnectableItem()->GetInstruction(), wait);

  // sequence now connected with new wait_view
  auto children_views = GetConnectedChildren(sequence_view);
  EXPECT_EQ(children_views, std::vector<ConnectableView*>({wait_view}));

  // selecting and deleting connection
  wait_view->setSelected(true);

  ASSERT_NO_FATAL_FAILURE(m_scene.OnDeleteSelectedRequest());
  QTest::qWait(200);

  auto connections = sequence_view->GetParentPort()->connections();
  ASSERT_EQ(connections.size(), 0);

  // new sequence view is the same, wait view is different
  EXPECT_EQ(sequence_view, m_scene.FindViewForInstruction(sequence));

  // there is no connections anymore
  EXPECT_EQ(GetConnectedChildren(sequence_view).size(), 0);
}

//! Scene with two instructions. One connected with the another, when we delete parent. Child should
//! gets disconnected and survive.

TEST_F(GraphicsSceneTest, onDeleteSelectedParent)
{
  auto controller = CreateController();

  auto sequence = m_model.InsertItem<SequenceItem>(GetInstructionContainer());
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);

  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  ASSERT_TRUE(sequence_view != nullptr);
  auto wait_view = m_scene.FindViewForInstruction(wait);
  ASSERT_TRUE(wait_view != nullptr);

  EXPECT_EQ(sequence_view->GetConnectableItem()->GetInstruction(), sequence);
  EXPECT_EQ(wait_view->GetConnectableItem()->GetInstruction(), wait);

  // sequence now connected with new wait_view
  auto children_views = GetConnectedChildren(sequence_view);
  EXPECT_EQ(children_views, std::vector<ConnectableView*>({wait_view}));

  // selecting and deleting connection
  sequence_view->setSelected(true);

  ASSERT_NO_FATAL_FAILURE(m_scene.OnDeleteSelectedRequest());
  QTest::qWait(200);

  // WaitView vas regenerated on the move of ViewItem on top.
  auto new_wait_view = m_scene.FindViewForInstruction(wait);
  //  ASSERT_NE(wait_view, new_wait_view); // failing in the container

  EXPECT_EQ(new_wait_view->GetConnectableItem()->GetInstruction(), wait);
}

TEST_F(GraphicsSceneTest, GetSelectedInstructions)
{
  auto sequence = m_model.InsertItem<SequenceItem>(GetInstructionContainer());
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence);

  auto controller = CreateController();

  EXPECT_TRUE(m_scene.GetSelectedViewItems<QGraphicsItem>().empty());
  EXPECT_TRUE(m_scene.GetSelectedInstructions().empty());

  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  sequence_view->setSelected(true);
  EXPECT_EQ(m_scene.GetSelectedInstructions(), std::vector<InstructionItem*>({sequence}));

  auto wait1_view = m_scene.FindViewForInstruction(wait1);
  wait1_view->setSelected(true);
  EXPECT_EQ(Sorted(m_scene.GetSelectedInstructions()),
            Sorted(std::vector<InstructionItem*>({sequence, wait1})));
}

TEST_F(GraphicsSceneTest, SetSelectedInstructions)
{
  auto sequence = m_model.InsertItem<SequenceItem>(GetInstructionContainer());
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence);

  auto controller = CreateController();

  EXPECT_TRUE(m_scene.GetSelectedViewItems<QGraphicsItem>().empty());
  EXPECT_TRUE(m_scene.GetSelectedInstructions().empty());

  QSignalSpy spy_selected(&m_scene, &QGraphicsScene::selectionChanged);

  m_scene.SetSelectedInstructions(std::vector<InstructionItem*>({sequence, wait1}));

  EXPECT_EQ(spy_selected.count(), 2);

  EXPECT_EQ(Sorted(m_scene.GetSelectedInstructions()),
            Sorted(std::vector<InstructionItem*>({sequence, wait1})));

  EXPECT_TRUE(m_scene.FindViewForInstruction(sequence)->isSelected());
  EXPECT_TRUE(m_scene.FindViewForInstruction(wait1)->isSelected());
}

//! Check selection after the removal

TEST_F(GraphicsSceneTest, SelectionAfterRemoval)
{
  auto sequence = m_model.InsertItem<SequenceItem>(GetInstructionContainer());
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence);

  auto controller = CreateController();

  m_scene.SetSelectedInstructions(std::vector<InstructionItem*>({wait1, wait2}));

  QSignalSpy spy_selected(&m_scene, &QGraphicsScene::selectionChanged);

  m_model.RemoveItem(wait1);

  EXPECT_EQ(spy_selected.count(), 1);

  EXPECT_EQ(Sorted(m_scene.GetSelectedInstructions()),
            Sorted(std::vector<InstructionItem*>({wait2})));
}
