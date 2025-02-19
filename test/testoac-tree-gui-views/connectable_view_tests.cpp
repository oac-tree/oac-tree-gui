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

#include <mvvm/commands/i_command_stack.h>
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

//! Emulate clicking on instruction Node and dragging it to another scene's point.
//! Single undo command should bring it back.
TEST_F(ConnectableViewTest, UndoComplexMove)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  // QTest::mouseEvent seems having problems, see https://bugreports.qt.io/browse/QTBUG-5232
  GTEST_SKIP();
#endif

  NodeGraphicsView graphics_view(&m_scene);
  graphics_view.setMouseTracking(true);
  graphics_view.setFocus();

  m_model.SetUndoEnabled(true);

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
  const QPointF scene_press_coordinate{10, 10};
  const QPointF scene_move1_pos{100, 200};
  const QPointF scene_move2_pos{500, 600};

  {  // simulating drag-and-move

    const QPoint viewport_press_coordinate =
        graphics_view.viewport()->mapFromParent(graphics_view.mapFromScene(scene_press_coordinate));

    const QPoint viewport_move1_pos =
        graphics_view.viewport()->mapFromParent(graphics_view.mapFromScene(scene_move1_pos));

    const QPoint viewport_move2_pos =
        graphics_view.viewport()->mapFromParent(graphics_view.mapFromScene(scene_move2_pos));

    QTest::mousePress(graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                      viewport_press_coordinate);

    QTest::mouseEvent(QTest::MouseMove, graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                      viewport_move1_pos);
    QTest::mouseEvent(QTest::MouseMove, graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                      viewport_move2_pos);

    QTest::mouseRelease(graphics_view.viewport(), Qt::LeftButton);

    QCoreApplication::processEvents();
  }

  // instruction insert, and single macro with moves
  ASSERT_EQ(m_model.GetCommandStack()->GetCommandCount(), 2);

  // new coordinates of item's top-left corner
  const double expected_final_item_x = scene_move2_pos.x() - scene_press_coordinate.x();
  const double expected_final_item_y = scene_move2_pos.y() - scene_press_coordinate.y();

  EXPECT_FLOAT_EQ(item->GetX(), expected_final_item_x);
  EXPECT_FLOAT_EQ(item->GetY(), expected_final_item_y);

  // single undo should bring it back without intermediate moves
  m_model.GetCommandStack()->Undo();
  EXPECT_EQ(item->GetX(), 0.0);
  EXPECT_EQ(item->GetY(), 0.0);

  m_model.GetCommandStack()->Redo();
  EXPECT_FLOAT_EQ(item->GetX(), expected_final_item_x);
  EXPECT_FLOAT_EQ(item->GetY(), expected_final_item_y);
}

}  // namespace oac_tree_gui::test
