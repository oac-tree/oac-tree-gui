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

#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_job_items.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

#include "oac_tree_gui/viewmodel/job_list_viewmodel.h"

namespace oac_tree_gui::test
{

//! Tests for items from instructionitems.h

class JobListViewModelTest : public ::testing::Test
{
public:
  class TestModel : public mvvm::ApplicationModel
  {
  public:
    TestModel() : mvvm::ApplicationModel("TestModel") {}
  };
};

TEST_F(JobListViewModelTest, SingleProcedure)
{
  const RunnerStatus expected_status = RunnerStatus::kInitial;
  TestModel model;

  auto item = model.InsertItem<LocalJobItem>();
  item->SetStatus(expected_status);

  JobListViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  auto displayname_index = viewmodel.index(0, 0);
  auto status_index = viewmodel.index(0, 1);

  auto views = viewmodel.FindViews(item->GetItem(itemconstants::kStatus));
  ASSERT_EQ(views.size(), 1);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), status_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(displayname_index), item);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(status_index), item->GetItem(itemconstants::kStatus));

  EXPECT_EQ(viewmodel.data(displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("LocalJob"));
  EXPECT_EQ(viewmodel.data(status_index, Qt::DisplayRole).toString().toStdString(),
            std::string(ToString(expected_status)));

  // there shouldn't be children below
  EXPECT_EQ(viewmodel.rowCount(displayname_index), 0);
  EXPECT_EQ(viewmodel.rowCount(status_index), 0);
}

TEST_F(JobListViewModelTest, NotificationOnStatusChange)
{
  TestModel model;

  auto procedure0 = model.InsertItem<LocalJobItem>();
  auto procedure1 = model.InsertItem<LocalJobItem>();

  JobListViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 2);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  auto status_index = viewmodel.index(0, 1);

  const QSignalSpy spy_data_changed(&viewmodel, &JobListViewModel::dataChanged);

  procedure0->SetStatus(RunnerStatus::kInitial);
  EXPECT_EQ(spy_data_changed.count(), 1);
}

}  // namespace oac_tree_gui::test
