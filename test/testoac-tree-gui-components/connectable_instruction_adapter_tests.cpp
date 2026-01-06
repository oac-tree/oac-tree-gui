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

#include "oac_tree_gui/nodeeditor/connectable_instruction_adapter.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>

#include <mvvm/nodeeditor/node_port.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for ConnectableInstructionAdapter class.
 */
class ConnectableInstructionAdapterTest : public ::testing::Test
{
};

TEST_F(ConnectableInstructionAdapterTest, AdapterInitialState)
{
  EXPECT_THROW(ConnectableInstructionAdapter(nullptr), RuntimeException);

  SequenceItem sequence_item;
  const ConnectableInstructionAdapter sequence_adapter(&sequence_item);
  EXPECT_EQ(sequence_adapter.GetItem(), &sequence_item);
  EXPECT_EQ(sequence_adapter.GetDisplayName().toStdString(), sequence_item.GetDisplayName());
  EXPECT_EQ(sequence_adapter.GetX(), 0.0);
  EXPECT_EQ(sequence_adapter.GetY(), 0.0);

  ASSERT_EQ(sequence_adapter.GetPortInfos().size(), 2U);
  EXPECT_EQ(sequence_adapter.GetPortInfos().at(0).GetPortDirection(), mvvm::PortDirection::kInput);
  EXPECT_EQ(sequence_adapter.GetPortInfos().at(1).GetPortDirection(), mvvm::PortDirection::kOutput);

  auto style = sequence_adapter.GetStyle();
  EXPECT_EQ(style.base_color, GetBaseColor(&sequence_item));

  WaitItem wait_item;
  const ConnectableInstructionAdapter wait_adapter(&wait_item);

  ASSERT_EQ(wait_adapter.GetPortInfos().size(), 1U);
  EXPECT_EQ(wait_adapter.GetPortInfos().at(0).GetPortDirection(), mvvm::PortDirection::kOutput);
  EXPECT_TRUE(wait_adapter.GetOperationStates().None());
}

TEST_F(ConnectableInstructionAdapterTest, SetCoordinates)
{
  SequenceItem item;
  item.SetX(1.0);
  item.SetY(2.0);

  ConnectableInstructionAdapter adapter(&item);

  EXPECT_EQ(adapter.GetX(), 1.0);
  EXPECT_EQ(adapter.GetY(), 2.0);

  // adapter setters
  adapter.SetX(3.0);
  adapter.SetY(4.0);
  EXPECT_EQ(item.GetX(), 3.0);
  EXPECT_EQ(item.GetY(), 4.0);
}

TEST_F(ConnectableInstructionAdapterTest, OperationStatesForInstructionStatus)
{
  WaitItem item;
  const ConnectableInstructionAdapter adapter(&item);

  mvvm::NodeOperationStates expected_states;
  EXPECT_EQ(adapter.GetOperationStates(), expected_states);

  item.SetStatus(InstructionStatus::kRunning);
  expected_states.Set(mvvm::NodeOperationState::kRunning);
  EXPECT_EQ(adapter.GetOperationStates(), expected_states);
}

TEST_F(ConnectableInstructionAdapterTest, OperationStatesForSequenceInstruction)
{
  SequenceItem item;
  item.InsertItem<WaitItem>(mvvm::TagIndex::Append());

  const ConnectableInstructionAdapter adapter(&item);

  mvvm::NodeOperationStates expected_states;
  EXPECT_EQ(adapter.GetOperationStates(), expected_states);

  expected_states.Set(mvvm::NodeOperationState::kCollapsedChildren);
  SetCollapsed(true, item);
  EXPECT_EQ(adapter.GetOperationStates(), expected_states);

  item.SetStatus(InstructionStatus::kRunning);
  expected_states.Set(mvvm::NodeOperationState::kRunning);
  EXPECT_EQ(adapter.GetOperationStates(), expected_states);
}

}  // namespace oac_tree_gui::test
