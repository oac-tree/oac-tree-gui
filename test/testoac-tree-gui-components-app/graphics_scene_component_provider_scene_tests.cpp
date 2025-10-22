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

#include "oac_tree_gui/nodeeditor/objects/graphics_scene_component_provider.h"

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/universal_item_helper.h>

#include <mvvm/model/application_model.h>
#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>
#include <mvvm/nodeeditor/i_node_port.h>
#include <mvvm/nodeeditor/node_connection_shape.h>
#include <mvvm/nodeeditor/node_port_shape.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/test_node_editor_helper.h>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSignalSpy>
#include <QTest>

namespace oac_tree_gui::test
{

/**
 * @brief Extended tests for GraphicsSceneComponentProviderTest class.
 *
 * Testing various scenario related to commands issued from the graphics scene or the graphics view.
 */
class GraphicsSceneComponentProviderSceneTest : public ::testing::Test
{
public:
  GraphicsSceneComponentProviderSceneTest()
      : m_graphics_view(&m_graphics_scene)
      , m_instruction_container(m_model.InsertItem<InstructionContainerItem>())
  {
  }

  /**
   * @brief Creates provider instance for testing.
   */
  std::unique_ptr<GraphicsSceneComponentProvider> CreateProvider()
  {
    return std::make_unique<GraphicsSceneComponentProvider>(
        m_mock_message.AsStdFunction(), m_mock_object_name.AsStdFunction(), &m_graphics_scene,
        m_instruction_container);
  }

  /**
   * @brief Finds all underlying instructions by exploring shapes on the scene.
   */
  std::vector<InstructionItem*> FindSceneInstructions() const
  {
    return test::FindSceneInstructions(m_graphics_scene);
  }

  /**
   * @brief Helper to get shapes of specific type from the scene.
   *
   * The result will be sorted according the order of the addition.
   */
  template <typename ShapeT>
  std::vector<ShapeT*> FindSceneShapes() const
  {
    return ::mvvm::GetShapes<ShapeT>(m_graphics_scene);
  }

  QGraphicsScene m_graphics_scene;
  QGraphicsView m_graphics_view;
  mvvm::ApplicationModel m_model;
  InstructionContainerItem* m_instruction_container{nullptr};

  testing::MockFunction<void(const sup::gui::MessageEvent&)> m_mock_message;
  testing::MockFunction<std::string(const std::string&)> m_mock_object_name;
};

TEST_F(GraphicsSceneComponentProviderSceneTest, EstablishConnectionFromSequenceToWait)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  sequence->SetX(10.0);
  sequence->SetY(20.0);
  auto wait = m_model.InsertItem<WaitItem>(m_instruction_container);
  wait->SetX(100.0);
  wait->SetY(200.0);

  auto provider = CreateProvider();

  const QSignalSpy spy_connection_started(provider.get(),
                                          &GraphicsSceneComponentProvider::connectionStarted);
  const QSignalSpy spy_connection_finished(provider.get(),
                                           &GraphicsSceneComponentProvider::connectionFinished);

  // two shapes and no connection between them
  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);
  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 0);

  // clicking on the output port of the sequence and then on the input port of the wait
  auto sequence_port_pos = test::GetInputPortScenePosition(*shapes.at(0));
  auto wait_port_pos = test::GetOutputPortScenePosition(*shapes.at(1));

  QTest::mousePress(m_graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                    m_graphics_view.mapFromScene(sequence_port_pos));

  QTest::mouseRelease(m_graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                      m_graphics_view.mapFromScene(wait_port_pos));

  EXPECT_EQ(spy_connection_started.count(), 1);
  EXPECT_EQ(spy_connection_finished.count(), 1);

  // now there should be connection between two shapes
  shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);
  connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 1);

  ASSERT_TRUE(mvvm::IsParentChildConnection(shapes.at(0), shapes.at(1), connections.at(0)));
}

TEST_F(GraphicsSceneComponentProviderSceneTest, EstablishConnectionFromWaitToSequence)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  sequence->SetX(10.0);
  sequence->SetY(20.0);
  auto wait = m_model.InsertItem<WaitItem>(m_instruction_container);
  wait->SetX(100.0);
  wait->SetY(200.0);

  auto provider = CreateProvider();

  // two shapes and no connection between them
  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);
  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 0);

  // clicking on the output port of the sequence and then on the input port of the wait
  auto sequence_port_pos = test::GetInputPortScenePosition(*shapes.at(0));
  auto wait_port_pos = test::GetOutputPortScenePosition(*shapes.at(1));

  QTest::mousePress(m_graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                    m_graphics_view.mapFromScene(wait_port_pos));

  QTest::mouseRelease(m_graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                      m_graphics_view.mapFromScene(sequence_port_pos));

  // now there should be connection between two shapes
  shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);
  connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 1);

  ASSERT_TRUE(mvvm::IsParentChildConnection(shapes.at(0), shapes.at(1), connections.at(0)));
}

TEST_F(GraphicsSceneComponentProviderSceneTest, DropInstruction)
{
  const double expected_x{42.0};
  const double expected_y{43.0};

  auto provider = CreateProvider();

  provider->DropInstruction(mvvm::GetTypeName<SequenceItem>(), {expected_x, expected_y});
  ASSERT_EQ(m_instruction_container->GetInstructionCount(), 1);
  auto inserted_sequence = m_instruction_container->GetInstructions().at(0);
  EXPECT_EQ(inserted_sequence->GetX(), expected_x);
  EXPECT_EQ(inserted_sequence->GetY(), expected_y);

  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 1);
}

TEST_F(GraphicsSceneComponentProviderSceneTest, DropComplexAggregate)
{
  const double expected_x{42.0};
  const double expected_y{43.0};
  auto provider = CreateProvider();

  provider->DropInstruction("if-then-else", {expected_x, expected_y});
  ASSERT_EQ(m_instruction_container->GetInstructionCount(), 1);
  auto inserted_aggregate = m_instruction_container->GetInstructions().at(0);
  EXPECT_EQ(inserted_aggregate->GetX(), expected_x);
  EXPECT_EQ(inserted_aggregate->GetY(), expected_y);

  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 6);
  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 5);
}

TEST_F(GraphicsSceneComponentProviderSceneTest, DeleteSelectedInstruction)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);

  auto provider = CreateProvider();
  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 1);
  auto sequence_shape = shapes.at(0);

  sequence_shape->setSelected(true);
  provider->OnDeleteSelected();
  EXPECT_EQ(m_instruction_container->GetInstructionCount(), 0);
  shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 0);
}

TEST_F(GraphicsSceneComponentProviderSceneTest, DeleteSelectedConnection)
{
  auto provider = CreateProvider();

  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);
  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 1);

  auto connection_shape = connections.at(0);
  connection_shape->setSelected(true);

  provider->OnDeleteSelected();

  EXPECT_EQ(m_instruction_container->GetInstructionCount(), 2);
  shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);

  connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 0);

  EXPECT_EQ(sequence->GetInstructions().size(), 0);
  const std::vector<InstructionItem*> expected_instructions({sequence, wait});
  EXPECT_EQ(m_instruction_container->GetInstructions(), expected_instructions);

  EXPECT_EQ(wait->GetParent(), m_instruction_container);
}

TEST_F(GraphicsSceneComponentProviderSceneTest, DeleteSelectedChild)
{
  auto provider = CreateProvider();

  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  provider->SetSelectedInstructions({wait});

  provider->OnDeleteSelected();
  EXPECT_EQ(m_instruction_container->GetInstructionCount(), 1);
  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 1);
  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 0);

  EXPECT_EQ(test::FindSceneInstructions(m_graphics_scene),
            m_instruction_container->GetInstructions());
}

TEST_F(GraphicsSceneComponentProviderSceneTest, DeleteSelectedParent)
{
  auto provider = CreateProvider();

  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  provider->SetSelectedInstructions({sequence});

  provider->OnDeleteSelected();

  EXPECT_EQ(m_instruction_container->GetInstructionCount(), 0);
  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 0);
  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 0);
}

//! Scenario with complex 3-level instruction tree. Everything is selected and deleted all together.
//! The model should be empty at the end. This validates that no connection is deleted twice.

TEST_F(GraphicsSceneComponentProviderSceneTest, ComplexAggregateRemoval)
{
  auto provider = CreateProvider();

  auto item = InsertAggregate("if-then-else", m_instruction_container);

  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 6);
  for (auto shape : shapes)
  {
    shape->setSelected(true);
  }

  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 5);
  for (auto connection : connections)
  {
    connection->setSelected(true);
  }

  provider->OnDeleteSelected();

  EXPECT_EQ(m_instruction_container->GetTotalItemCount(), 0);
  EXPECT_TRUE(m_graphics_scene.items().isEmpty());
}

TEST_F(GraphicsSceneComponentProviderSceneTest, CollapseWhenDoubleClickOnInputPort)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  sequence->SetX(10.0);
  sequence->SetY(20.0);
  auto wait = m_model.InsertItem<WaitItem>(m_instruction_container);
  wait->SetX(100.0);
  wait->SetY(200.0);

  auto provider = CreateProvider();

  // two shapes and no connection between them
  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);
  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 0);

  // clicking on the output port of the sequence and then on the input port of the wait
  auto sequence_port_pos = test::GetInputPortScenePosition(*shapes.at(0));

  const QPoint viewport_click_coordinate =
      m_graphics_view.viewport()->mapFromParent(m_graphics_view.mapFromScene(sequence_port_pos));

  EXPECT_FALSE(IsCollapsed(*sequence));

  QTest::mouseDClick(m_graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                     viewport_click_coordinate);

  EXPECT_TRUE(IsCollapsed(*sequence));

  QTest::mouseDClick(m_graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                     viewport_click_coordinate);
}

TEST_F(GraphicsSceneComponentProviderSceneTest, CollapseExpandWhenDoubleClickOnInputPort)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  sequence->SetX(10.0);
  sequence->SetY(20.0);
  auto wait = m_model.InsertItem<WaitItem>(m_instruction_container);
  wait->SetX(100.0);
  wait->SetY(200.0);

  auto provider = CreateProvider();

  // two shapes and no connection between them
  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);
  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 0);

  // clicking on the output port of the sequence and then on the input port of the wait
  auto sequence_port_pos = test::GetInputPortScenePosition(*shapes.at(0));

  const QPoint viewport_click_coordinate =
      m_graphics_view.viewport()->mapFromParent(m_graphics_view.mapFromScene(sequence_port_pos));

  EXPECT_FALSE(IsCollapsed(*sequence));

  QTest::mouseDClick(m_graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                     viewport_click_coordinate);

  EXPECT_TRUE(IsCollapsed(*sequence));

  QTest::mouseDClick(m_graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                     viewport_click_coordinate);

  EXPECT_FALSE(IsCollapsed(*sequence));
}

}  // namespace oac_tree_gui::test
