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

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/application_model.h>

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

//! Checking macro mode in undo/redo scenario.
TEST_F(ConnectableInstructionAdapterTest, UndoRedoAndSetXY)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<SequenceItem>();
  ConnectableInstructionAdapter adapter(item);

  model.SetUndoEnabled(true);

  adapter.SetXY(1.0, 2.0);
  EXPECT_EQ(item->GetX(), 1.0);
  EXPECT_EQ(item->GetY(), 2.0);

  adapter.SetXY(3.0, 4.0);
  EXPECT_EQ(item->GetX(), 3.0);
  EXPECT_EQ(item->GetY(), 4.0);

  model.GetCommandStack()->Undo();
  EXPECT_EQ(item->GetX(), 1.0);
  EXPECT_EQ(item->GetY(), 2.0);

  model.GetCommandStack()->Undo();
  EXPECT_EQ(item->GetX(), 0.0);
  EXPECT_EQ(item->GetY(), 0.0);
}

//! Checking macro mode in undo/redo scenario. The difference with previous is one of the values is
//! the same. We are checking that macro is working well, when one fo the command, setY, is in
//! obsolete state.
TEST_F(ConnectableInstructionAdapterTest, UndoRedoAndSetXAndSameY)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<SequenceItem>();
  item->SetX(1.0);
  item->SetY(2.0);

  ConnectableInstructionAdapter adapter(item);

  model.SetUndoEnabled(true);

  adapter.SetXY(3.0, 2.0);  // new x, same y
  EXPECT_EQ(item->GetX(), 3.0);
  EXPECT_EQ(item->GetY(), 2.0);

  model.GetCommandStack()->Undo();
  EXPECT_EQ(item->GetX(), 1.0);
  EXPECT_EQ(item->GetY(), 2.0);
}
