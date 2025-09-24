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

#include "oac_tree_gui/nodeeditor/connectable_instruction_adapter_v2.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Testing ConnectableInstructionAdapter class.

class ConnectableInstructionAdapterV2Test : public ::testing::Test
{
};

TEST_F(ConnectableInstructionAdapterV2Test, AdapterInitialState)
{
  EXPECT_THROW(ConnectableInstructionAdapterV2(nullptr), RuntimeException);

  SequenceItem sequence_item;
  const ConnectableInstructionAdapterV2 sequence_adapter(&sequence_item);
  EXPECT_EQ(sequence_adapter.GetItem(), &sequence_item);
  EXPECT_EQ(sequence_adapter.GetDisplayName().toStdString(), sequence_item.GetDisplayName());
  EXPECT_EQ(sequence_adapter.GetX(), 0.0);
  EXPECT_EQ(sequence_adapter.GetY(), 0.0);
  EXPECT_EQ(sequence_adapter.GetBaseColor(), GetBaseColor(&sequence_item));

  ASSERT_EQ(sequence_adapter.GetPortInfos().size(), 2U);
  EXPECT_EQ(sequence_adapter.GetPortInfos().at(0).GetPortDirection(), mvvm::PortDirection::kInput);
  EXPECT_EQ(sequence_adapter.GetPortInfos().at(1).GetPortDirection(), mvvm::PortDirection::kOutput);

  WaitItem wait_item;
  const ConnectableInstructionAdapterV2 wait_adapter(&wait_item);

  ASSERT_EQ(wait_adapter.GetPortInfos().size(), 1U);
  EXPECT_EQ(wait_adapter.GetPortInfos().at(0).GetPortDirection(), mvvm::PortDirection::kOutput);
}

TEST_F(ConnectableInstructionAdapterV2Test, SetCoordinates)
{
  SequenceItem item;
  item.SetX(1.0);
  item.SetY(2.0);

  ConnectableInstructionAdapterV2 adapter(&item);

  EXPECT_EQ(adapter.GetX(), 1.0);
  EXPECT_EQ(adapter.GetY(), 2.0);

  // adapter setters
  adapter.SetX(3.0);
  adapter.SetY(4.0);
  EXPECT_EQ(item.GetX(), 3.0);
  EXPECT_EQ(item.GetY(), 4.0);
}
}  // namespace oac_tree_gui::test
