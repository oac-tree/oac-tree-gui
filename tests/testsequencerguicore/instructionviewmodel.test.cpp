/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/monitor/instructionviewmodel.h"

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/model/applicationmodel.h"

#include <gtest/gtest.h>

#include <QSignalSpy>

using namespace sequencergui;

//! Tests for items from instructionitems.h

class InstructionViewModelTest : public ::testing::Test
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

TEST_F(InstructionViewModelTest, SingleInstruction)
{
  TestModel model;

  auto sequence = model.InsertItem<SequenceItem>();
  sequence->SetStatus("abc");

  InstructionViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 3);

  auto sequence_displayname_index = viewmodel.index(0, 0);
  auto sequence_customname_index = viewmodel.index(0, 1);
  auto sequence_status_index = viewmodel.index(0, 2);

  auto views = viewmodel.FindViews(sequence->GetStatusItem());
  EXPECT_EQ(views.size(), 1);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), sequence_status_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(sequence_displayname_index), sequence);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(sequence_customname_index), sequence->GetNameItem());
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(sequence_status_index), sequence->GetStatusItem());

  EXPECT_EQ(viewmodel.data(sequence_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Sequence"));
  EXPECT_EQ(viewmodel.data(sequence_customname_index, Qt::DisplayRole).toString().toStdString(),
            std::string(""));
  EXPECT_EQ(viewmodel.data(sequence_status_index, Qt::DisplayRole).toString().toStdString(),
            std::string("abc"));
}

TEST_F(InstructionViewModelTest, SequenceWithChild)
{
  TestModel model;

  auto sequence = model.InsertItem<SequenceItem>();
  auto wait0 = model.InsertItem<WaitItem>(sequence);
  auto wait1 = model.InsertItem<WaitItem>(sequence);

  InstructionViewModel viewmodel(&model);
  auto sequence_ndex = viewmodel.index(0, 0);
  EXPECT_EQ(viewmodel.rowCount(sequence_ndex), 2);
  EXPECT_EQ(viewmodel.columnCount(sequence_ndex), 3);

  auto wait0_displayname_index = viewmodel.index(0, 0, sequence_ndex);
  auto wait1_displayname_index = viewmodel.index(1, 0, sequence_ndex);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(wait0_displayname_index), wait0);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(wait1_displayname_index), wait1);

  EXPECT_EQ(viewmodel.data(wait0_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Wait0"));
  EXPECT_EQ(viewmodel.data(wait1_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Wait1"));
}

TEST_F(InstructionViewModelTest, NotificationOnStatusChange)
{
  TestModel model;

  auto sequence = model.InsertItem<SequenceItem>();

  InstructionViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 3);

  auto sequence_status_index = viewmodel.index(0, 2);

  QSignalSpy spy_data_changed(&viewmodel, &InstructionViewModel::dataChanged);

  sequence->SetStatus("abc");
  EXPECT_EQ(spy_data_changed.count(), 1);
}
