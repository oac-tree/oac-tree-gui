/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/viewmodel/instruction_editor_viewmodel.h"

#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

using namespace sequencergui;

//! Tests for InstructionEditorViewModel class.

class InstructionEditorViewModelTest : public ::testing::Test
{
public:
  class TestModel : public mvvm::ApplicationModel
  {
  public:
    TestModel() : mvvm::ApplicationModel("TestModel")
    {
      RegisterItem<SequenceItem>();
      RegisterItem<WaitItem>();
    }
  };
};

//! Single instruction in a model.
//! ViewModel should see single row and 3 columns.

TEST_F(InstructionEditorViewModelTest, SingleInstruction)
{
  TestModel model;

  auto sequence = model.InsertItem<SequenceItem>();
  sequence->SetStatus("abc");

  InstructionEditorViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  auto sequence_displayname_index = viewmodel.index(0, 0);
  auto sequence_customname_index = viewmodel.index(0, 1);

  auto views = viewmodel.FindViews(GetNameItem(*sequence));
  EXPECT_EQ(views.size(), 1);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), sequence_customname_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(sequence_displayname_index), sequence);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(sequence_customname_index), GetNameItem(*sequence));

  EXPECT_EQ(viewmodel.data(sequence_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Sequence"));
  EXPECT_EQ(viewmodel.data(sequence_customname_index, Qt::DisplayRole).toString().toStdString(),
            std::string(""));
}

TEST_F(InstructionEditorViewModelTest, SequenceWithChild)
{
  TestModel model;

  auto sequence = model.InsertItem<SequenceItem>();
  auto wait0 = model.InsertItem<WaitItem>(sequence);
  auto wait1 = model.InsertItem<WaitItem>(sequence);

  InstructionEditorViewModel viewmodel(&model);
  auto sequence_ndex = viewmodel.index(0, 0);
  EXPECT_EQ(viewmodel.rowCount(sequence_ndex), 2);
  EXPECT_EQ(viewmodel.columnCount(sequence_ndex), 2);

  auto wait0_displayname_index = viewmodel.index(0, 0, sequence_ndex);
  auto wait1_displayname_index = viewmodel.index(1, 0, sequence_ndex);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(wait0_displayname_index), wait0);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(wait1_displayname_index), wait1);

  EXPECT_EQ(viewmodel.data(wait0_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Wait"));
  EXPECT_EQ(viewmodel.data(wait1_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Wait"));
}

TEST_F(InstructionEditorViewModelTest, NotificationOnStatusChange)
{
  TestModel model;

  auto sequence = model.InsertItem<SequenceItem>();

  InstructionEditorViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  QSignalSpy spy_data_changed(&viewmodel, &InstructionEditorViewModel::dataChanged);

  sequence->SetName("abc");
  EXPECT_EQ(spy_data_changed.count(), 1);
}
