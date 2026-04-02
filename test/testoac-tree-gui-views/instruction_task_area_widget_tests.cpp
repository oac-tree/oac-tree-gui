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

#include "oac_tree_gui/views/operation/instruction_task_area_widget.h"

#include <oac_tree_gui/views/operation/task_widget.h>

#include <gtest/gtest.h>

#include <QScrollArea>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for InstructionTaskAreaWidget class.
 */
class InstructionTaskAreaWidgetTest : public ::testing::Test
{
};

TEST_F(InstructionTaskAreaWidgetTest, SetTestWidgetAndClear)
{
  InstructionTaskAreaWidget area_widget;

  auto scroll_area = area_widget.findChild<QScrollArea*>();
  ASSERT_NE(scroll_area, nullptr);
  EXPECT_EQ(scroll_area->widget(), nullptr);

  auto widget1 = std::make_unique<TaskWidget>("abc");

  area_widget.SetTaskWidget(std::move(widget1));
  EXPECT_NE(scroll_area->widget(), nullptr);

  area_widget.Clear();
  EXPECT_EQ(scroll_area->widget(), nullptr);
}

}  // namespace oac_tree_gui::test
