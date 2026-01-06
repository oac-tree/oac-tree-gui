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

#include "oac_tree_gui/nodeeditor/objects/node_graphics_scene.h"

#include <oac_tree_gui/components/drag_and_drop_helper.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/nodeeditor/connectable_shape_factory.h>

#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>
#include <mvvm/nodeeditor/i_node_port.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>
#include <testutils/test_node_editor_helper.h>

#include <QApplication>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QSignalSpy>

Q_DECLARE_METATYPE(const mvvm::INodePort*)
Q_DECLARE_METATYPE(oac_tree_gui::InstructionItem*)

namespace oac_tree_gui::test
{

class NodeGraphicsSceneTest : public ::testing::Test
{
};

TEST_F(NodeGraphicsSceneTest, SendDropEvent)
{
  auto mime_data = CreateNewInstructionMimeData(
      QString::fromStdString(domainconstants::kIncludeInstructionType));

  QGraphicsSceneDragDropEvent event(QEvent::GraphicsSceneDrop);
  event.setScenePos(QPointF(10, 100));
  event.setMimeData(mime_data.get());

  NodeGraphicsScene scene;

  QSignalSpy spy(&scene, &NodeGraphicsScene::dropInstructionRequested);

  QApplication::sendEvent(&scene, &event);

  QCoreApplication::processEvents();

  ASSERT_EQ(spy.count(), 1);
  auto args = spy.takeFirst();
  EXPECT_EQ(args.at(0).toString(),
            QString::fromStdString(domainconstants::kIncludeInstructionType));

  // not clear how to test drop coordinates
  // EXPECT_EQ(args.at(1).toPointF(), QPointF(10, 100));
}

TEST_F(NodeGraphicsSceneTest, DoubleClickOnInstruction)
{
  const ConnectableShapeFactory factory;

  NodeGraphicsScene scene;

  SequenceItem item;
  item.SetX(1.0);
  item.SetY(2.0);

  auto shape = factory.CreateShape(&item);
  scene.addItem(shape.release());

  QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMouseDoubleClick);
  event.setScenePos(QPointF(10, 10));

  QSignalSpy spy_shape_clicks(&scene, &NodeGraphicsScene::instructionDoubleClick);
  const QSignalSpy spy_port_clicks(&scene, &NodeGraphicsScene::portDoubleClick);

  QApplication::sendEvent(&scene, &event);
  QCoreApplication::processEvents();

  EXPECT_EQ(spy_shape_clicks.count(), 1);
  EXPECT_EQ(spy_port_clicks.count(), 0);

  EXPECT_EQ(mvvm::test::GetSendItem<InstructionItem*>(spy_shape_clicks), &item);
}

TEST_F(NodeGraphicsSceneTest, DoubleClickOnInstructionPort)
{
  const ConnectableShapeFactory factory;

  NodeGraphicsScene scene;

  SequenceItem item;
  item.SetX(1.0);
  item.SetY(2.0);

  auto shape = factory.CreateShape(&item);
  auto shape_ptr = shape.get();
  scene.addItem(shape.release());

  auto port_pos = GetInputPortScenePosition(*shape_ptr);

  QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMouseDoubleClick);
  event.setScenePos(port_pos);

  const QSignalSpy spy_shape_clicks(&scene, &NodeGraphicsScene::instructionDoubleClick);
  QSignalSpy spy_port_clicks(&scene, &NodeGraphicsScene::portDoubleClick);

  QApplication::sendEvent(&scene, &event);
  QCoreApplication::processEvents();

  EXPECT_EQ(spy_shape_clicks.count(), 0);
  EXPECT_EQ(spy_port_clicks.count(), 1);

  EXPECT_EQ(mvvm::test::GetSendItem<const mvvm::INodePort*>(spy_port_clicks),
            mvvm::GetInputPort(*shape_ptr));
}

}  // namespace oac_tree_gui::test
