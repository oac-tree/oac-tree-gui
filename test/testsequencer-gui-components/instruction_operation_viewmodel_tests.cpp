/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "sequencergui/viewmodel/instruction_operation_viewmodel.h"

#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/operation/breakpoint_helper.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

using namespace sequencergui;

//! Tests for InstructionOperationViewModel class.

class InstructionOperationViewModelTest : public ::testing::Test
{
public:
  class TestModel : public mvvm::ApplicationModel
  {
  public:
    TestModel() : mvvm::ApplicationModel("TestModel") {}
  };
};

//! Single instruction in a model.
//! ViewModel should see single row and 3 columns.

TEST_F(InstructionOperationViewModelTest, SingleInstruction)
{
  TestModel model;

  auto sequence = model.InsertItem<SequenceItem>();
  sequence->SetStatus("abc");

  SetBreakpointStatus(*sequence, BreakpointStatus::kSet);

  InstructionOperationViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 3);

  auto sequence_displayname_index = viewmodel.index(0, 0);
  auto sequence_status_index = viewmodel.index(0, 1);
  auto sequence_breakpoint_index = viewmodel.index(0, 2);

  auto views = viewmodel.FindViews(GetStatusItem(*sequence));
  EXPECT_EQ(views.size(), 1);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), sequence_status_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(sequence_displayname_index), sequence);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(sequence_status_index), GetStatusItem(*sequence));
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(sequence_breakpoint_index),
            GetBreakpointItem(*sequence));

  EXPECT_EQ(viewmodel.data(sequence_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Sequence"));
  EXPECT_EQ(viewmodel.data(sequence_status_index, Qt::DisplayRole).toString().toStdString(),
            std::string("abc"));
  // returns int corresponding to BreakpointStatus::kSet
  EXPECT_EQ(viewmodel.data(sequence_breakpoint_index, Qt::DisplayRole).toInt(), 1);
}

TEST_F(InstructionOperationViewModelTest, SequenceWithChild)
{
  TestModel model;

  auto sequence = model.InsertItem<SequenceItem>();
  auto wait0 = model.InsertItem<WaitItem>(sequence);
  auto wait1 = model.InsertItem<WaitItem>(sequence);

  InstructionOperationViewModel viewmodel(&model);
  auto sequence_ndex = viewmodel.index(0, 0);
  EXPECT_EQ(viewmodel.rowCount(sequence_ndex), 2);
  EXPECT_EQ(viewmodel.columnCount(sequence_ndex), 3);

  auto wait0_displayname_index = viewmodel.index(0, 0, sequence_ndex);
  auto wait1_displayname_index = viewmodel.index(1, 0, sequence_ndex);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(wait0_displayname_index), wait0);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(wait1_displayname_index), wait1);

  EXPECT_EQ(viewmodel.data(wait0_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Wait"));
  EXPECT_EQ(viewmodel.data(wait1_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Wait"));
}

TEST_F(InstructionOperationViewModelTest, NotificationOnStatusChange)
{
  TestModel model;

  auto sequence = model.InsertItem<SequenceItem>();

  InstructionOperationViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 3);

  QSignalSpy spy_data_changed(&viewmodel, &InstructionOperationViewModel::dataChanged);

  sequence->SetStatus("abc");
  EXPECT_EQ(spy_data_changed.count(), 1);
}
