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

#include "oac_tree_gui/viewmodel/job_list_viewmodel.h"

#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_job_items.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

#include <QMimeData>
#include <QSignalSpy>

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

  JobListViewModel view_model(&model);
  EXPECT_EQ(view_model.rowCount(), 1);
  EXPECT_EQ(view_model.columnCount(), 2);

  auto displayname_index = view_model.index(0, 0);
  auto status_index = view_model.index(0, 1);

  auto views = view_model.FindViews(item->GetItem(itemconstants::kStatus));
  ASSERT_EQ(views.size(), 1);
  EXPECT_EQ(view_model.indexFromItem(views[0]), status_index);

  EXPECT_EQ(view_model.GetSessionItemFromIndex(displayname_index), item);
  EXPECT_EQ(view_model.GetSessionItemFromIndex(status_index),
            item->GetItem(itemconstants::kStatus));

  EXPECT_EQ(view_model.data(displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("LocalJob"));
  EXPECT_EQ(view_model.data(status_index, Qt::DisplayRole).toString().toStdString(),
            std::string(ToString(expected_status)));

  // there shouldn't be children below
  EXPECT_EQ(view_model.rowCount(displayname_index), 0);
  EXPECT_EQ(view_model.rowCount(status_index), 0);
}

TEST_F(JobListViewModelTest, NotificationOnStatusChange)
{
  TestModel model;

  auto job0 = model.InsertItem<LocalJobItem>();
  auto job1 = model.InsertItem<LocalJobItem>();

  JobListViewModel view_model(&model);
  EXPECT_EQ(view_model.rowCount(), 2);
  EXPECT_EQ(view_model.columnCount(), 2);

  auto status_index = view_model.index(0, 1);

  const QSignalSpy spy_data_changed(&view_model, &JobListViewModel::dataChanged);

  job0->SetStatus(RunnerStatus::kInitial);
  EXPECT_EQ(spy_data_changed.count(), 1);
}

TEST_F(JobListViewModelTest, DragJobFromFirstPositionToLast)
{
  TestModel model;

  auto job0 = model.InsertItem<LocalJobItem>();
  auto job1 = model.InsertItem<LocalJobItem>();
  auto job2 = model.InsertItem<LocalJobItem>();

  JobListViewModel view_model(&model);
  auto job0_index = view_model.index(0, 0);
  auto job2_index = view_model.index(2, 0);

  const std::unique_ptr<QMimeData> mime_data(view_model.mimeData({job0_index}));

  // pretending to drop after the last item
  const std::int32_t drop_indicator_row = 3;
  const QModelIndex parent_index = QModelIndex();  // invalid

  EXPECT_TRUE(view_model.dropMimeData(mime_data.get(), Qt::MoveAction, drop_indicator_row, 0,
                                      parent_index));
  EXPECT_EQ(model.GetRootItem()->GetAllItems(),
            std::vector<mvvm::SessionItem*>({job1, job2, job0}));
}

TEST_F(JobListViewModelTest, DragLastJobOnTopOfFirst)
{
  TestModel model;

  auto job0 = model.InsertItem<LocalJobItem>();
  auto job1 = model.InsertItem<LocalJobItem>();
  auto job2 = model.InsertItem<LocalJobItem>();

  JobListViewModel view_model(&model);
  auto job0_name_index = view_model.index(0, 0);
  auto job0_status_index = view_model.index(0, 1);
  auto job2_name_index = view_model.index(2, 0);
  auto job2_status_index = view_model.index(2, 1);

  // pretending to drop on top of the first item
  const std::int32_t drop_indicator_row = -1;
  const QModelIndex parent_index = job0_name_index; // droping on cell containing the name

  // pretending the whole row with job2 is dragged
  const std::unique_ptr<QMimeData> mime_data(
      view_model.mimeData({job2_name_index, job2_status_index}));

  EXPECT_TRUE(view_model.dropMimeData(mime_data.get(), Qt::MoveAction, drop_indicator_row, 0,
                                      parent_index));
  EXPECT_EQ(model.GetRootItem()->GetAllItems(),
            std::vector<mvvm::SessionItem*>({job2, job0, job1}));
}

}  // namespace oac_tree_gui::test
