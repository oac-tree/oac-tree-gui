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

#include "oac_tree_gui/viewmodel/instruction_operation_viewmodel.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/instruction_info_item.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/operation/breakpoint_helper.h>

#include <mvvm/model/application_model.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_info.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

#include <QDebug>
#include <QSignalSpy>

namespace oac_tree_gui::test
{

//! Tests for InstructionOperationViewModel class.

class InstructionOperationViewModelTest : public ::testing::Test
{
public:
  /**
   * @brief Test helper which initializes given InstructionInfoItem with all content for given
   * domain type.
   */
  static void InitFromDomainType(const std::string& domain_type, InstructionInfoItem& info_item)
  {
    auto instruction = CreateDomainInstruction(domain_type);
    info_item.InitFromDomainInfo(test::CreateInstructionInfo(*instruction));
  }

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

//! Validating how view model depicts simplified tree made of InstructionInfoItem.
TEST_F(InstructionOperationViewModelTest, InfoItemWithChildren)
{
  TestModel model;

  auto sequence = model.InsertItem<InstructionInfoItem>();
  InitFromDomainType(domainconstants::kSequenceInstructionType,
                     *sequence);  // to get children tag initialized

  auto wait0 = model.InsertItem<InstructionInfoItem>(sequence);
  InitFromDomainType(domainconstants::kWaitInstructionType,
                     *wait0);  // to get children tag initialized
  wait0->SetDisplayName("Wait");
  auto wait1 = model.InsertItem<InstructionInfoItem>(sequence);
  InitFromDomainType(domainconstants::kWaitInstructionType,
                     *wait1);  // to get children tag initialized
  wait1->SetDisplayName("Wait");

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

}  // namespace oac_tree_gui::test
