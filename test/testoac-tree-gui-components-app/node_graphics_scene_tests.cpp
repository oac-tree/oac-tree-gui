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

#include "oac_tree_gui/nodeeditor/objects/node_graphics_scene.h"

#include <oac_tree_gui/components/drag_and_drop_helper.h>
#include <oac_tree_gui/domain/domain_constants.h>

#include <gtest/gtest.h>

#include <QApplication>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QSignalSpy>

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

}  // namespace oac_tree_gui::test
