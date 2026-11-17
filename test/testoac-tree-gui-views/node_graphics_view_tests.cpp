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

#include "oac_tree_gui/views/nodeeditor/node_graphics_view.h"

#include <oac_tree_gui/nodeeditor/scene_utils.h>

#include <gtest/gtest.h>

#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QLineF>

namespace oac_tree_gui::test
{

//! Tests for NodeGraphicsView class.
class NodeGraphicsViewTest : public ::testing::Test
{
public:
  NodeGraphicsViewTest() : m_view(&m_scene)
  {
    m_scene.setSceneRect(-500.0, -500.0, 1000.0, 1000.0);
    m_view.resize(800, 600);
    m_view.show();
    QApplication::processEvents();
  }

  QGraphicsScene m_scene;
  NodeGraphicsView m_view;
};

//! Without items, the view takes the scene rect.
TEST_F(NodeGraphicsViewTest, FocusOnItemsWithoutItems)
{
  m_view.FocusOnItems();

  EXPECT_EQ(m_view.sceneRect(), m_scene.sceneRect());
  EXPECT_DOUBLE_EQ(m_view.GetZoomFactor(), 1.0);
}

//! A small item is centered, with zoom capped at 100%.
TEST_F(NodeGraphicsViewTest, FocusOnSmallItem)
{
  m_scene.addRect(1000.0, 2000.0, 100.0, 50.0);
  const auto items_rect = m_scene.itemsBoundingRect();

  m_view.FocusOnItems();

  const double size = GetGraphicsViewportSize().width();
  EXPECT_EQ(m_view.sceneRect().center(), items_rect.center());
  EXPECT_DOUBLE_EQ(m_view.sceneRect().width(), items_rect.width() + size);
  EXPECT_DOUBLE_EQ(m_view.sceneRect().height(), items_rect.height() + size);
  EXPECT_DOUBLE_EQ(m_view.GetZoomFactor(), 1.0);
  const auto view_center = m_view.mapToScene(m_view.viewport()->rect().center());
  EXPECT_LT(QLineF(view_center, items_rect.center()).length(), 2.0);
}

//! Items larger than the viewport are zoomed out to fit.
TEST_F(NodeGraphicsViewTest, FocusOnLargeItems)
{
  m_scene.addRect(-5000.0, 0.0, 100.0, 50.0);
  m_scene.addRect(5000.0, 3000.0, 100.0, 50.0);

  m_view.FocusOnItems();

  EXPECT_EQ(m_view.sceneRect().center(), m_scene.itemsBoundingRect().center());
  EXPECT_LT(m_view.GetZoomFactor(), 1.0);
}

}  // namespace oac_tree_gui::test
