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
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/application_model.h>
#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>
#include <mvvm/nodeeditor/node_connection_shape.h>
#include <mvvm/test/mock_item_listener.h>
#include <mvvm/test/test_container_helper.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/test_node_editor_helper.h>

#include <QGraphicsScene>
#include <QSignalSpy>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for GraphicsSceneComponentProviderTest class.
 *
 * Here we are testing the basic correspondence of instructions on the model, and connections and
 * shapes on the scene.
 */
class GraphicsSceneComponentProviderTest : public ::testing::Test
{
public:
  GraphicsSceneComponentProviderTest()
      : m_instruction_container(m_model.InsertItem<InstructionContainerItem>())
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
  mvvm::ApplicationModel m_model;
  InstructionContainerItem* m_instruction_container{nullptr};

  testing::MockFunction<void(const sup::gui::MessageEvent&)> m_mock_message;
  testing::MockFunction<std::string(const std::string&)> m_mock_object_name;
};

TEST_F(GraphicsSceneComponentProviderTest, InitialState)
{
  auto provider = CreateProvider();
  EXPECT_TRUE(provider->GetSelectedInstructions().empty());
  EXPECT_TRUE(FindSceneInstructions().empty());
  EXPECT_TRUE(m_graphics_scene.items().empty());
}

TEST_F(GraphicsSceneComponentProviderTest, SingleInstructionBeforeInit)
{
  auto wait = m_model.InsertItem<WaitItem>(m_instruction_container);
  auto provider = CreateProvider();

  EXPECT_TRUE(provider->GetSelectedInstructions().empty());

  auto instructions = FindSceneInstructions();
  ASSERT_EQ(instructions.size(), 1);
  EXPECT_EQ(instructions.front(), wait);
}

TEST_F(GraphicsSceneComponentProviderTest, SequenceWithChildBeforeInit)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  auto provider = CreateProvider();

  EXPECT_TRUE(provider->GetSelectedInstructions().empty());

  auto instructions = FindSceneInstructions();
  EXPECT_EQ(instructions, std::vector<InstructionItem*>({sequence, wait}));

  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 1);

  ASSERT_TRUE(mvvm::IsParentChildConnection(shapes.at(0), shapes.at(1), connections.at(0)));
}

TEST_F(GraphicsSceneComponentProviderTest, SetSingleSelectedViaProvider)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);

  const QSignalSpy spy_scene_selected(&m_graphics_scene, &QGraphicsScene::selectionChanged);

  auto provider = CreateProvider();
  const QSignalSpy spy_provider_selected(provider.get(),
                                         &GraphicsSceneComponentProvider::selectionChanged);

  EXPECT_TRUE(provider->GetSelectedInstructions().empty());

  provider->SetSelectedInstructions({sequence});
  const std::vector<InstructionItem*> expected_selection({sequence});
  EXPECT_TRUE(
      mvvm::test::HaveSameElements(provider->GetSelectedInstructions(), expected_selection));

  EXPECT_EQ(spy_scene_selected.count(), 1);
  EXPECT_EQ(spy_provider_selected.count(), 1);
}

TEST_F(GraphicsSceneComponentProviderTest, SetTwoSelectedViaProvider)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);

  const QSignalSpy spy_scene_selected(&m_graphics_scene, &QGraphicsScene::selectionChanged);

  auto provider = CreateProvider();
  const QSignalSpy spy_provider_selected(provider.get(),
                                         &GraphicsSceneComponentProvider::selectionChanged);

  EXPECT_TRUE(provider->GetSelectedInstructions().empty());

  provider->SetSelectedInstructions({sequence, wait1});
  const std::vector<InstructionItem*> expected_selection({sequence, wait1});
  EXPECT_TRUE(
      mvvm::test::HaveSameElements(provider->GetSelectedInstructions(), expected_selection));

  EXPECT_EQ(spy_scene_selected.count(), 2);
  EXPECT_EQ(spy_provider_selected.count(), 2);
}

TEST_F(GraphicsSceneComponentProviderTest, SetSelectedViaScene)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);

  const QSignalSpy spy_scene_selected(&m_graphics_scene, &QGraphicsScene::selectionChanged);

  auto provider = CreateProvider();
  const QSignalSpy spy_provider_selected(provider.get(),
                                         &GraphicsSceneComponentProvider::selectionChanged);

  EXPECT_TRUE(provider->GetSelectedInstructions().empty());

  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 3);
  shapes.at(0)->setSelected(true);
  shapes.at(2)->setSelected(true);

  const std::vector<InstructionItem*> expected_selection({sequence, wait1});
  EXPECT_TRUE(
      mvvm::test::HaveSameElements(provider->GetSelectedInstructions(), expected_selection));

  EXPECT_EQ(spy_scene_selected.count(), 2);
  EXPECT_EQ(spy_provider_selected.count(), 2);
}

TEST_F(GraphicsSceneComponentProviderTest, InsertSequenceAndWait)
{
  auto provider = CreateProvider();

  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  auto instructions = FindSceneInstructions();
  EXPECT_EQ(instructions, std::vector<InstructionItem*>({sequence, wait}));

  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 1);

  ASSERT_TRUE(IsParentChildConnection(shapes.at(0), shapes.at(1), connections.at(0)));
}

TEST_F(GraphicsSceneComponentProviderTest, RemoveInstruction)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);

  auto provider = CreateProvider();

  auto instructions = FindSceneInstructions();
  ASSERT_EQ(instructions.size(), 1);
  EXPECT_EQ(instructions.front(), sequence);

  m_model.RemoveItem(sequence);
  instructions = FindSceneInstructions();
  EXPECT_TRUE(instructions.empty());
}

TEST_F(GraphicsSceneComponentProviderTest, RemoveInstructionWithChid)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  auto provider = CreateProvider();

  m_model.RemoveItem(sequence);

  auto instructions = FindSceneInstructions();
  EXPECT_TRUE(instructions.empty());
  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 0);
}

TEST_F(GraphicsSceneComponentProviderTest, MoveItem)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  sequence->SetX(42.0);
  EXPECT_DOUBLE_EQ(sequence->GetX(), 42.0);
  EXPECT_DOUBLE_EQ(sequence->GetY(), 0.0);

  auto provider = CreateProvider();

  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 1);
  auto shape = shapes.front();
  EXPECT_DOUBLE_EQ(shape->x(), 42.0);
  EXPECT_DOUBLE_EQ(shape->y(), 0.0);

  mvvm::test::MockItemListener listener(sequence);

  const mvvm::PropertyChangedEvent expected_event{sequence, itemconstants::kYpos};
  EXPECT_CALL(listener, OnPropertyChanged(expected_event)).Times(1);

  sequence->SetX(42.0);
  sequence->SetY(43.0);

  EXPECT_DOUBLE_EQ(shape->x(), 42.0);
  EXPECT_DOUBLE_EQ(shape->y(), 43.0);
}

TEST_F(GraphicsSceneComponentProviderTest, MoveShape)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);

  auto provider = CreateProvider();

  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 1);
  auto shape = shapes.front();
  EXPECT_DOUBLE_EQ(shape->x(), 0.0);
  EXPECT_DOUBLE_EQ(shape->y(), 0.0);

  mvvm::test::MockItemListener listener(sequence);
  const mvvm::PropertyChangedEvent expected_event{sequence, itemconstants::kXpos};
  EXPECT_CALL(listener, OnPropertyChanged(expected_event)).Times(1);

  shape->setX(42.0);

  EXPECT_EQ(sequence->GetX(), 42.0);
  EXPECT_EQ(sequence->GetY(), 0.0);
}

TEST_F(GraphicsSceneComponentProviderTest, MoveShapeAndUndo)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  sequence->SetX(42.0);
  EXPECT_DOUBLE_EQ(sequence->GetX(), 42.0);
  EXPECT_DOUBLE_EQ(sequence->GetY(), 0.0);

  m_model.SetUndoEnabled(true);

  auto provider = CreateProvider();

  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 1);
  auto shape = shapes.front();
  EXPECT_DOUBLE_EQ(shape->x(), 42.0);
  EXPECT_DOUBLE_EQ(shape->y(), 0.0);

  mvvm::test::MockItemListener listener(sequence);
  const mvvm::PropertyChangedEvent expected_event{sequence, itemconstants::kXpos};
  EXPECT_CALL(listener, OnPropertyChanged(expected_event)).Times(1);

  shape->setX(43.0);
  EXPECT_DOUBLE_EQ(sequence->GetX(), 43.0);
  EXPECT_DOUBLE_EQ(sequence->GetY(), 0.0);

  ASSERT_EQ(m_model.GetCommandStack()->GetCommandCount(), 1);

  EXPECT_CALL(listener, OnPropertyChanged(expected_event)).Times(1);

  EXPECT_NO_FATAL_FAILURE(m_model.GetCommandStack()->Undo());
  EXPECT_DOUBLE_EQ(sequence->GetX(), 42.0);
  EXPECT_DOUBLE_EQ(sequence->GetY(), 0.0);
}

}  // namespace oac_tree_gui::test
