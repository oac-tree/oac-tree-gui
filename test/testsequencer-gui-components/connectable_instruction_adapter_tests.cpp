/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/nodeeditor/connectable_instruction_adapter.h"

#include <sequencergui/model/standard_instruction_items.h>

#include <gtest/gtest.h>

#include <QString>

using namespace sequencergui;

//! Testing ConnectableInstructionAdapter class.

class ConnectableInstructionAdapterTest : public ::testing::Test
{
};

TEST_F(ConnectableInstructionAdapterTest, AdapterFromSequence)
{
  SequenceItem item;
  item.SetX(1.0);
  item.SetY(2.0);

  ConnectableInstructionAdapter adapter(&item);

  // adapter getters
  EXPECT_EQ(adapter.GetInstruction(), &item);
  EXPECT_EQ(adapter.GetDisplayName().toStdString(), item.GetDisplayName());
  EXPECT_EQ(adapter.GetX(), 1.0);
  EXPECT_EQ(adapter.GetY(), 2.0);

  // adapter setters
  adapter.SetX(3.0);
  adapter.SetY(4.0);
  EXPECT_EQ(item.GetX(), 3.0);
  EXPECT_EQ(item.GetY(), 4.0);
}
