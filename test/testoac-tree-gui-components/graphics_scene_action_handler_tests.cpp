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

#include "oac_tree_gui/nodeeditor/graphics_scene_action_handler.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/nodeeditor/connectable_shape_factory.h>

#include <mvvm/model/application_model.h>
#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>
#include <mvvm/nodeeditor/node_connection.h>
#include <mvvm/nodeeditor/node_editor_helper.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for GraphicsSceneActionHandler class.
 */
class GraphicsSceneActionHandlerTest : public ::testing::Test
{
public:
  /**
   * @brief Creates instance of GraphicsSceneActionHandler for testing with mock send_message
   * callback.
   */
  std::unique_ptr<GraphicsSceneActionHandler> CreateGraphicsSceneActionHandler()
  {
    return std::make_unique<GraphicsSceneActionHandler>(m_model.GetRootItem(),
                                                        m_mock_send_message.AsStdFunction());
  }

  ConnectableShapeFactory m_shape_factory;
  mvvm::ApplicationModel m_model;
  ::testing::MockFunction<void(const sup::gui::MessageEvent&)> m_mock_send_message;
};

TEST_F(GraphicsSceneActionHandlerTest, UndefinedCallback)
{
  EXPECT_THROW(GraphicsSceneActionHandler(nullptr, {}), RuntimeException);

  mvvm::SessionItem item;
  EXPECT_THROW(GraphicsSceneActionHandler(&item, {}), RuntimeException);
}

TEST_F(GraphicsSceneActionHandlerTest, ConnectSequenceToWaitStartingFromSequence)
{
  auto action_handler = CreateGraphicsSceneActionHandler();

  auto sequence_item = m_model.InsertItem<SequenceItem>();
  auto wait_item = m_model.InsertItem<WaitItem>();

  auto sequence_shape = m_shape_factory.CreateShape(sequence_item);
  auto wait_shape = m_shape_factory.CreateShape(wait_item);

  EXPECT_CALL(m_mock_send_message, Call(::testing::_)).Times(0);

  // Connect wait to sequence (wait becomes child of sequence)
  action_handler->ConnectPorts(mvvm::GetInputPort(*sequence_shape),
                               mvvm::GetOutputPort(*wait_shape));

  EXPECT_EQ(wait_item->GetParent(), sequence_item);
  EXPECT_EQ(sequence_item->GetInstructions().size(), 1U);
}

TEST_F(GraphicsSceneActionHandlerTest, ConnectSequenceToWaitStartingFromWait)
{
  auto action_handler = CreateGraphicsSceneActionHandler();

  auto sequence_item = m_model.InsertItem<SequenceItem>();
  auto wait_item = m_model.InsertItem<WaitItem>();

  auto sequence_shape = m_shape_factory.CreateShape(sequence_item);
  auto wait_shape = m_shape_factory.CreateShape(wait_item);

  EXPECT_CALL(m_mock_send_message, Call(::testing::_)).Times(0);

  // the order is changed, but the result is the same as in previous test
  action_handler->ConnectPorts(mvvm::GetOutputPort(*wait_shape),
                               mvvm::GetInputPort(*sequence_shape));

  EXPECT_EQ(wait_item->GetParent(), sequence_item);
  EXPECT_EQ(sequence_item->GetInstructions().size(), 1U);
}

TEST_F(GraphicsSceneActionHandlerTest, AttemptToConnectUnconnectablePorts)
{
  auto action_handler = CreateGraphicsSceneActionHandler();

  auto sequence_item = m_model.InsertItem<SequenceItem>();
  auto wait_item = m_model.InsertItem<WaitItem>();

  auto sequence_shape = m_shape_factory.CreateShape(sequence_item);
  auto wait_shape = m_shape_factory.CreateShape(wait_item);

  // No error reported, since uncomatible ports is no-op.
  EXPECT_CALL(m_mock_send_message, Call(::testing::_)).Times(0);

  // Attempt to connect two input ports
  action_handler->ConnectPorts(mvvm::GetInputPort(*sequence_shape),
                               mvvm::GetInputPort(*wait_shape));

  // No connection happened.
  EXPECT_EQ(wait_item->GetParent(), m_model.GetRootItem());
  EXPECT_EQ(sequence_item->GetInstructions().size(), 0U);
}

TEST_F(GraphicsSceneActionHandlerTest, AttemptToAddSecondChildToRepeat)
{
  auto action_handler = CreateGraphicsSceneActionHandler();

  auto repeat_item = m_model.InsertItem<RepeatItem>();
  auto sequence_item = m_model.InsertItem<SequenceItem>(repeat_item);
  auto wait_item = m_model.InsertItem<WaitItem>();

  auto repeat_shape = m_shape_factory.CreateShape(repeat_item);
  auto wait_shape = m_shape_factory.CreateShape(wait_item);

  // No error reported, since uncomatible ports is no-op.
  EXPECT_CALL(m_mock_send_message, Call(::testing::_)).Times(1);

  // Attempt to connect two input ports
  action_handler->ConnectPorts(mvvm::GetInputPort(*repeat_shape), mvvm::GetOutputPort(*wait_shape));

  // No connection happened.
  EXPECT_EQ(wait_item->GetParent(), m_model.GetRootItem());
  EXPECT_EQ(repeat_item->GetInstructions().size(), 1U);
}

TEST_F(GraphicsSceneActionHandlerTest, DisconnectWaitFromSequence)
{
  auto action_handler = CreateGraphicsSceneActionHandler();

  auto sequence_item = m_model.InsertItem<SequenceItem>();
  auto wait_item = m_model.InsertItem<WaitItem>(sequence_item);

  auto sequence_shape = m_shape_factory.CreateShape(sequence_item);
  auto wait_shape = m_shape_factory.CreateShape(wait_item);

  // manually connecting shapes to reflect actual model state
  auto input_port = mvvm::GetInputPort(*sequence_shape);
  auto output_port = mvvm::GetOutputPort(*wait_shape);
  auto connection = std::make_unique<mvvm::NodeConnection>(output_port, input_port);

  EXPECT_CALL(m_mock_send_message, Call(::testing::_)).Times(0);
  action_handler->Disconnect(connection.get());
  EXPECT_EQ(wait_item->GetParent(), m_model.GetRootItem());
  EXPECT_EQ(sequence_item->GetInstructions().size(), 0U);
}

TEST_F(GraphicsSceneActionHandlerTest, AttemptToDisconnectWaitFromSequence)
{
  auto action_handler = CreateGraphicsSceneActionHandler();

  auto sequence_item = m_model.InsertItem<SequenceItem>();
  auto wait_item = m_model.InsertItem<WaitItem>();  // wait is not a child of a sequence

  auto sequence_shape = m_shape_factory.CreateShape(sequence_item);
  auto wait_shape = m_shape_factory.CreateShape(wait_item);

  // manually connecting shapes to pretend items are connected, which is not the case
  auto input_port = mvvm::GetInputPort(*sequence_shape);
  auto output_port = mvvm::GetOutputPort(*wait_shape);
  auto connection = std::make_unique<mvvm::NodeConnection>(output_port, input_port);

  EXPECT_CALL(m_mock_send_message, Call(::testing::_)).Times(1);
  action_handler->Disconnect(connection.get());

  EXPECT_EQ(wait_item->GetParent(), m_model.GetRootItem());
  EXPECT_EQ(sequence_item->GetInstructions().size(), 0U);
}

TEST_F(GraphicsSceneActionHandlerTest, DoubleClickPort)
{
  auto action_handler = CreateGraphicsSceneActionHandler();

  auto sequence_item = m_model.InsertItem<SequenceItem>();
  auto wait_item = m_model.InsertItem<WaitItem>();  // wait is not a child of a sequence

  auto sequence_shape = m_shape_factory.CreateShape(sequence_item);
  auto wait_shape = m_shape_factory.CreateShape(wait_item);

  // manually connecting shapes to pretend items are connected, which is not the case
  auto input_port = mvvm::GetInputPort(*sequence_shape);
  auto output_port = mvvm::GetOutputPort(*wait_shape);

  EXPECT_CALL(m_mock_send_message, Call(::testing::_)).Times(0);

  EXPECT_FALSE(IsCollapsed(*sequence_item));

  action_handler->DoubleClickPort(input_port);
  EXPECT_TRUE(IsCollapsed(*sequence_item));

  action_handler->DoubleClickPort(input_port);
  EXPECT_FALSE(IsCollapsed(*sequence_item));

  // clicking on output port is noop
  action_handler->DoubleClickPort(output_port);
  EXPECT_FALSE(IsCollapsed(*sequence_item));
}

}  // namespace oac_tree_gui::test
