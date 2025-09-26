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

  // now there should be connection between two shapes
  shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);
  connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 1);

  ASSERT_TRUE(mvvm::IsParentChildConnection(shapes.at(0), shapes.at(1), connections.at(0)));
}

// This test is failing because of double deletion
// 1. NodeConnectionGuideImpl::ProcessMouseRelease ->  m_current_connection.reset()
// 2. ConnectableViewModelController::OnModelEvent -> FindHangingConnectionShapes
// FIXME: change the way connections are cleaned-up after Shape removal
TEST_F(GraphicsSceneComponentProviderSceneTest, DISABLED_EstablishConnectionFromWaitToSequence)
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

}  // namespace oac_tree_gui::test
