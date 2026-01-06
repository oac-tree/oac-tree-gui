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

#include "oac_tree_gui/nodeeditor/objects/graphics_scene_component_provider.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/epics_instruction_items.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/application_model.h>
#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>
#include <mvvm/nodeeditor/node_connection_shape.h>
#include <mvvm/test/mock_item_listener.h>
#include <mvvm/test/test_container_helper.h>

#include <sup/dto/anyvalue.h>

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
      mvvm::utils::HaveSameElements(provider->GetSelectedInstructions(), expected_selection));

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
      mvvm::utils::HaveSameElements(provider->GetSelectedInstructions(), expected_selection));

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
      mvvm::utils::HaveSameElements(provider->GetSelectedInstructions(), expected_selection));

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

TEST_F(GraphicsSceneComponentProviderTest, SelectionAfterRemoval)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence);

  const QSignalSpy spy_scene_selected(&m_graphics_scene, &QGraphicsScene::selectionChanged);

  auto provider = CreateProvider();
  provider->SetSelectedInstructions({wait1, wait2});

  const QSignalSpy spy_provider_selected(provider.get(),
                                         &GraphicsSceneComponentProvider::selectionChanged);

  m_model.RemoveItem(wait1);

  const std::vector<InstructionItem*> expected_selection({wait2});
  EXPECT_TRUE(
      mvvm::utils::HaveSameElements(provider->GetSelectedInstructions(), expected_selection));

  EXPECT_EQ(spy_provider_selected.count(), 1);
}

//! Scene is looking into an empty container. Inserting another container, scene should ignore any
//! activity in it.
TEST_F(GraphicsSceneComponentProviderTest, InsertAnotherContainer)
{
  auto provider = CreateProvider();
  auto wait0 = m_model.InsertItem<WaitItem>(m_instruction_container);

  auto another_container = m_model.InsertItem<InstructionContainerItem>();
  auto sequence = m_model.InsertItem<SequenceItem>(another_container);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);

  auto instructions = FindSceneInstructions();
  ASSERT_EQ(instructions.size(), 1);
  EXPECT_EQ(instructions.front(), wait0);
}

//! Scene is looking in the container with an instruction. Remove container and make sure, that the
//! scene was cleaned up.
TEST_F(GraphicsSceneComponentProviderTest, RemoveContainer)
{
  auto provider = CreateProvider();
  auto wait0 = m_model.InsertItem<WaitItem>(m_instruction_container);

  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  EXPECT_EQ(shapes.size(), 1);

  // scene should be cleaned up after the container removal
  m_model.RemoveItem(m_instruction_container);
  EXPECT_EQ(m_graphics_scene.items().size(), 0);

  // any activity after is ignored
  auto new_container = m_model.InsertItem<InstructionContainerItem>();
  auto wait1 = m_model.InsertItem<WaitItem>(new_container);
  EXPECT_EQ(m_graphics_scene.items().size(), 0);
}

//! Testing the case when AnyValue is replaced on board of PvAccessWriteInstructionItem. The
//! graphics controller should be happy with that and shouldn't try to create/remove instructions
//! related views.
TEST_F(GraphicsSceneComponentProviderTest, InsertAnyValueItem)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  auto provider = CreateProvider();

  auto instruction_item = m_model.InsertItem<PvAccessWriteInstructionItem>(m_instruction_container);

  auto instructions = FindSceneInstructions();
  ASSERT_EQ(instructions.size(), 1);
  EXPECT_EQ(instructions.front(), instruction_item);

  const sup::dto::AnyValue expected_anyvalue(sup::dto::SignedInteger32Type, 42);

  EXPECT_NO_THROW(SetAnyValue(expected_anyvalue, *instruction_item));
  instructions = FindSceneInstructions();
  ASSERT_EQ(instructions.size(), 1);
  EXPECT_EQ(instructions.front(), instruction_item);
}

TEST_F(GraphicsSceneComponentProviderTest, SequenceAndWaitWhenSequenceCollapsed)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  SetCollapsed(true, *sequence);

  auto provider = CreateProvider();

  auto instructions = FindSceneInstructions();
  EXPECT_EQ(instructions, std::vector<InstructionItem*>({sequence}));

  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 1U);

  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();

  ASSERT_EQ(connections.size(), 0);

  // controller ignores Wait
  ASSERT_EQ(FindSceneInstructions(), std::vector<InstructionItem*>({sequence}));
}

TEST_F(GraphicsSceneComponentProviderTest, InsertSequenceAndWaitWhenSequenceCollapsed)
{
  // controller created before the insert event
  auto provider = CreateProvider();

  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  SetCollapsed(true, *sequence);
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  auto instructions = FindSceneInstructions();
  EXPECT_EQ(instructions, std::vector<InstructionItem*>({sequence}));

  auto shapes = FindSceneShapes<mvvm::ConnectableShape>();
  ASSERT_EQ(shapes.size(), 1U);

  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();

  ASSERT_EQ(connections.size(), 0);

  // controller ignores Wait
  ASSERT_EQ(FindSceneInstructions(), std::vector<InstructionItem*>({sequence}));
}

TEST_F(GraphicsSceneComponentProviderTest, ValidateUpdateOnSequenceCollapsePropertyChange)
{
  auto provider = CreateProvider();

  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  auto wait = m_model.InsertItem<WaitItem>(sequence);

  ASSERT_EQ(FindSceneInstructions(), std::vector<InstructionItem*>({sequence, wait}));
  auto connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 1U);

  SetCollapsed(true, *sequence);
  ASSERT_EQ(FindSceneInstructions(), std::vector<InstructionItem*>({sequence}));
  connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 0U);

  SetCollapsed(false, *sequence);
  ASSERT_EQ(FindSceneInstructions(), std::vector<InstructionItem*>({sequence, wait}));
  connections = FindSceneShapes<mvvm::NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 1U);
}

TEST_F(GraphicsSceneComponentProviderTest, SelectInstructionBranch)
{
  auto sequence = m_model.InsertItem<SequenceItem>(m_instruction_container);
  auto wait0 = m_model.InsertItem<WaitItem>(sequence);
  auto wait1 = m_model.InsertItem<WaitItem>(sequence);
  auto wait2 = m_model.InsertItem<WaitItem>(sequence);

  const QSignalSpy spy_scene_selected(&m_graphics_scene, &QGraphicsScene::selectionChanged);

  auto provider = CreateProvider();

  const QSignalSpy spy_provider_selected(provider.get(),
                                         &GraphicsSceneComponentProvider::selectionChanged);

  provider->SelectInstructionBranch(wait1);

  const std::vector<InstructionItem*> expected_selection({wait1});
  EXPECT_TRUE(
      mvvm::utils::HaveSameElements(provider->GetSelectedInstructions(), expected_selection));

  EXPECT_EQ(spy_provider_selected.count(), 1);

  provider->SelectInstructionBranch(sequence);

  const std::vector<InstructionItem*> expected_selection2({sequence, wait0, wait1, wait2});
  EXPECT_TRUE(
      mvvm::utils::HaveSameElements(provider->GetSelectedInstructions(), expected_selection2));

  EXPECT_EQ(spy_provider_selected.count(), 6);
}

}  // namespace oac_tree_gui::test
