/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/nodeeditor/graphicsscene.h"

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/nodeeditor/connectableinstructionadapter.h"
#include "sequencergui/nodeeditor/connectableview.h"
#include "sequencergui/nodeeditor/graphicsscenecontroller.h"
#include "sequencergui/nodeeditor/nodeconnection.h"
#include "sequencergui/nodeeditor/nodeport.h"

#include <gtest/gtest.h>

using namespace sequi;

//! Tests for GraphicsScene class. Supplements graphicscontroller.test.cpp

class GraphicsSceneTest : public ::testing::Test
{
public:
  GraphicsSceneTest()
  {
    m_model.InsertItem<ProcedureItem>();
    m_scene.SetContext(&m_model, GetContainer());
  }

  InstructionContainerItem* GetContainer()
  {
    return m_model.GetTopItem<ProcedureItem>()->GetInstructionContainer();
  }

  std::vector<ConnectableView*> GetConnectedChildren(ConnectableView* parent)
  {
    std::vector<ConnectableView*> result;
    for (auto connection : parent->parentPort()->connections())
    {
      result.push_back(connection->childView());
    }
    return result;
  }

  std::unique_ptr<GraphicsSceneController> CreateController()
  {
    auto result = std::make_unique<GraphicsSceneController>(&m_model, &m_scene);
    result->Init(GetContainer());
    return result;
  }

  SequencerModel m_model;
  GraphicsScene m_scene;
};

//! Scene with single instruction. Make it selected and emit delete request.

TEST_F(GraphicsSceneTest, onDeleteSelectedRequest)
{
  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());

  auto controller = CreateController();

  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  ASSERT_TRUE(sequence_view != nullptr);
  sequence_view->setSelected(true);

  // deleting view
  m_scene.onDeleteSelectedRequest();
  EXPECT_TRUE(m_scene.FindViewForInstruction(sequence) == nullptr);

  // the model should be empty too
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 0);
}

//! Scene with two instructions. One gets connected with another.

TEST_F(GraphicsSceneTest, onConnectionRequest)
{
  auto controller = CreateController();

  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());
  auto wait = m_model.InsertItem<WaitItem>(GetContainer());

  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);

  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  ASSERT_TRUE(sequence_view != nullptr);
  auto wait_view = m_scene.FindViewForInstruction(wait);
  ASSERT_TRUE(wait_view != nullptr);

  // connecting two views
  m_scene.onConnectionRequest(wait_view, sequence_view);

  // still two connectable view
  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);

  // sequence view should be as before
  EXPECT_EQ(sequence_view, m_scene.FindViewForInstruction(sequence));

  // but the wait view was regenerated
  auto new_wait_view = m_scene.FindViewForInstruction(wait);
  EXPECT_NE(wait_view, new_wait_view);

  // sequence now connected with new wait_view
  auto children_views = GetConnectedChildren(sequence_view);
  EXPECT_EQ(children_views, std::vector<ConnectableView*>({new_wait_view}));

  // the model should have sequence as a top instruciton, and wait into it
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  EXPECT_EQ(sequence->GetInstructions(), std::vector<InstructionItem*>({wait}));
}

//! Scene with two instructions. One connected with the another, when we delete connection.
//! Children instruction should move on top of the container.

TEST_F(GraphicsSceneTest, onConnectionDeletionViaDisconnect)
{
  auto controller = CreateController();

  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());
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
  auto connections = sequence_view->parentPort()->connections();
  ASSERT_EQ(connections.size(), 1);

  // deleting connection
  m_scene.disconnectConnectedViews(connections.at(0));

  // new sequence view is the same, wait view is different
  EXPECT_EQ(sequence_view, m_scene.FindViewForInstruction(sequence));
  auto new_wait_view = m_scene.FindViewForInstruction(wait);
  ASSERT_NE(wait_view, new_wait_view);

  // there is no connections anymore
  EXPECT_EQ(GetConnectedChildren(sequence_view).size(), 0);

  // procedure contains two instructions
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 2);
  EXPECT_EQ(GetContainer()->GetInstructions(), std::vector<InstructionItem*>({sequence, wait}));
}

//! Scene with two instructions. One connected with the another, when we delete child.

TEST_F(GraphicsSceneTest, onDeleteSelectedChild)
{
  auto controller = CreateController();

  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);

  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  ASSERT_TRUE(sequence_view != nullptr);
  auto wait_view = m_scene.FindViewForInstruction(wait);
  ASSERT_TRUE(wait_view != nullptr);

  EXPECT_EQ(sequence_view->connectableItem()->GetInstruction(), sequence);
  EXPECT_EQ(wait_view->connectableItem()->GetInstruction(), wait);

  // sequence now connected with new wait_view
  auto children_views = GetConnectedChildren(sequence_view);
  EXPECT_EQ(children_views, std::vector<ConnectableView*>({wait_view}));

  // selecting and deleting connection
  wait_view->setSelected(true);

  ASSERT_NO_FATAL_FAILURE(m_scene.onDeleteSelectedRequest());

  auto connections = sequence_view->parentPort()->connections();
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

  auto sequence = m_model.InsertItem<SequenceItem>(GetContainer());
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  EXPECT_EQ(m_scene.GetConnectableViews().size(), 2);

  auto sequence_view = m_scene.FindViewForInstruction(sequence);
  ASSERT_TRUE(sequence_view != nullptr);
  auto wait_view = m_scene.FindViewForInstruction(wait);
  ASSERT_TRUE(wait_view != nullptr);

  EXPECT_EQ(sequence_view->connectableItem()->GetInstruction(), sequence);
  EXPECT_EQ(wait_view->connectableItem()->GetInstruction(), wait);

  // sequence now connected with new wait_view
  auto children_views = GetConnectedChildren(sequence_view);
  EXPECT_EQ(children_views, std::vector<ConnectableView*>({wait_view}));

  // selecting and deleting connection
  sequence_view->setSelected(true);

  ASSERT_NO_FATAL_FAILURE(m_scene.onDeleteSelectedRequest());

  // WaitView vas regenerated on the move of ViewItem on top.
  auto new_wait_view = m_scene.FindViewForInstruction(wait);
  ASSERT_NE(wait_view, new_wait_view);

  EXPECT_EQ(new_wait_view->connectableItem()->GetInstruction(), wait);
}
