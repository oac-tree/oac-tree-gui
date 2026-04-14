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

#include "oac_tree_gui/views/operation/instruction_task_monitor.h"

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/views/operation/instruction_task_widget_builder.h>
#include <oac_tree_gui/views/operation/task_widget.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests of InstructionTaskMonitor class.
 */
class InstructionTaskMonitorTest : public ::testing::Test
{
};

TEST_F(InstructionTaskMonitorTest, InitialState)
{
  InstructionTaskMonitor task_monitor;
  EXPECT_EQ(task_monitor.GetTaskWidgetBuilder()->GetInstructionCount(), 0U);
}

TEST_F(InstructionTaskMonitorTest, SetEmptyContainer)
{
  mvvm::ApplicationModel model;
  auto container = model.InsertItem<InstructionContainerItem>();

  InstructionTaskMonitor task_monitor;
  task_monitor.SetInstructionContainer(container);
  EXPECT_EQ(task_monitor.GetTaskWidgetBuilder()->GetInstructionCount(), 0U);
}

TEST_F(InstructionTaskMonitorTest, SetNonEmptyContainerAndThenChangeItToEmpty)
{
  mvvm::ApplicationModel model;
  auto container0 = model.InsertItem<InstructionContainerItem>();
  auto sequence = model.InsertItem<SequenceItem>(container0);
  auto container1 = model.InsertItem<InstructionContainerItem>();

  InstructionTaskMonitor task_monitor;
  task_monitor.SetInstructionContainer(container0);
  EXPECT_EQ(task_monitor.GetTaskWidgetBuilder()->GetInstructionCount(), 1U);

  task_monitor.SetInstructionContainer(container1);
  EXPECT_EQ(task_monitor.GetTaskWidgetBuilder()->GetInstructionCount(), 0U);
}

}  // namespace oac_tree_gui::test
