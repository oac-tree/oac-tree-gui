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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/nodeeditor/connectable_view.h"

#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/nodeeditor/connectable_instruction_adapter.h>
#include <oac_tree_gui/nodeeditor/connectable_view.h>
#include <oac_tree_gui/nodeeditor/connectable_view_model_controller.h>
#include <oac_tree_gui/nodeeditor/node_graphics_scene.h>
#include <oac_tree_gui/views/nodeeditor/node_graphics_view.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

#include <QDebug>
#include <QTest>

namespace oac_tree_gui::test
{

/**
 * @brief Tests of ConnectableView class.
 */
class ConnectableViewTest : public ::testing::Test
{
public:
  ConnectableViewTest() : m_scene([](auto) {})
  {
    m_procedure_item = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  InstructionContainerItem* GetContainer() { return m_procedure_item->GetInstructionContainer(); }

  SequencerModel m_model;
  ProcedureItem* m_procedure_item{nullptr};
  NodeGraphicsScene m_scene;
};

TEST_F(ConnectableViewTest, CoordinateSynchronization)
{
  auto item = m_model.InsertItem<SequenceItem>();
  auto adapter = std::make_unique<ConnectableInstructionAdapter>(item);

  auto view = std::make_unique<ConnectableView>(std::move(adapter));
  auto view_ptr = view.get();
  m_scene.addItem(view.release());

  EXPECT_EQ(view_ptr->x(), 0.0);
  EXPECT_EQ(view_ptr->y(), 0.0);
  EXPECT_EQ(item->GetX(), 0.0);
  EXPECT_EQ(item->GetY(), 0.0);

  // propagation of coordinates from view to item
  view_ptr->setX(1.0);
  view_ptr->setY(2.0);
  EXPECT_EQ(item->GetX(), 1.0);
  EXPECT_EQ(item->GetY(), 2.0);
}

//! Emu
TEST_F(ConnectableViewTest, UndoComplexMove)
{
  qDebug() << "1.1";
  NodeGraphicsView graphics_view(&m_scene);
  graphics_view.setMouseTracking(true);
  graphics_view.setFocus();

  auto item = m_model.InsertItem<SequenceItem>();
  auto adapter = std::make_unique<ConnectableInstructionAdapter>(item);

  auto view = std::make_unique<ConnectableView>(std::move(adapter));
  auto view_ptr = view.get();
  m_scene.addItem(view.release());
  view_ptr->setSelected(true);

  EXPECT_EQ(view_ptr->x(), 0.0);
  EXPECT_EQ(view_ptr->y(), 0.0);
  EXPECT_EQ(item->GetX(), 0.0);
  EXPECT_EQ(item->GetY(), 0.0);

  // Out view is a rectangle with top-left-origin at (0,0). Selecting press coordinate so it is
  // inside this rectangle.
  QPointF scene_press_coordinate{10, 10};
  QPointF scene_move1_pos{400, 500};

  qDebug() << ")))" << scene_press_coordinate << graphics_view.mapFromScene(scene_press_coordinate) << graphics_view.viewport()->mapFromParent(graphics_view.mapFromScene(scene_press_coordinate));

  QPoint viewport_press_voordinate =
      graphics_view.viewport()->mapFromParent(graphics_view.mapFromScene(scene_press_coordinate));

  QPoint viewport_move1_pos =
      graphics_view.viewport()->mapFromParent(graphics_view.mapFromScene(scene_move1_pos));

  QTest::mousePress(graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                    viewport_press_voordinate);
  QCoreApplication::processEvents();

  QTest::mouseEvent(QTest::MouseMove, graphics_view.viewport(), Qt::NoButton, Qt::NoModifier,
                    viewport_move1_pos);
  QCoreApplication::processEvents();

  // QTest::mouseEvent(QTest::MouseMove, graphics_view.viewport(), Qt::LeftButton,
  //                   Qt::NoModifier, move2_pos);
  QCoreApplication::processEvents();

  QTest::mouseRelease(graphics_view.viewport(), Qt::LeftButton);
  QCoreApplication::processEvents();

  // QPointF final_coordinate = graphics_view.mapToScene(move1_pos);

  EXPECT_FLOAT_EQ(item->GetX(), scene_move1_pos.x());
  EXPECT_FLOAT_EQ(item->GetY(), scene_move1_pos.y());
}

}  // namespace oac_tree_gui::test
