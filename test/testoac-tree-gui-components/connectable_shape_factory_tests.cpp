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

#include "oac_tree_gui/nodeeditor/connectable_shape_factory.h"

#include <oac_tree_gui/model/standard_instruction_items.h>

#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/i_connectable_shape_adapter.h>
#include <mvvm/standarditems/vector_item.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Testing for ConnectableShapeFactoryTest class.
 */
class ConnectableShapeFactoryTest : public ::testing::Test
{
};

TEST_F(ConnectableShapeFactoryTest, InitialState)
{
  const ConnectableShapeFactory factory;

  SequenceItem item;
  item.SetX(1.0);
  item.SetY(2.0);

  auto shape = factory.CreateShape(&item);
  EXPECT_EQ(shape->x(), 1.0);
  EXPECT_EQ(shape->y(), 2.0);
}

TEST_F(ConnectableShapeFactoryTest, CreateShapeForUnrelatedItem)
{
  const ConnectableShapeFactory factory;

  mvvm::VectorItem item;
  auto shape = factory.CreateShape(&item);

  EXPECT_EQ(shape.get(), nullptr);
}

}  // namespace oac_tree_gui::test
