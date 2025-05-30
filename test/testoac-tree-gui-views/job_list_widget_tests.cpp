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
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/model/standard_job_items.h>

#include <mvvm/test/test_helper.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QTreeView>

#include "oac_tree_gui/views/operation/job_list_widget.h"

Q_DECLARE_METATYPE(oac_tree_gui::JobItem*)

namespace oac_tree_gui::test
{

//! Tests for utility functions related to the domain to presentation transformations.

class JobListWidgetTest : public ::testing::Test
{
};

TEST_F(JobListWidgetTest, InitialState)
{
  JobListWidget view;
  EXPECT_EQ(view.GetSelectedJob(), nullptr);
}

TEST_F(JobListWidgetTest, SelectJob)
{
  JobModel model;
  auto job = model.InsertItem<LocalJobItem>();

  JobListWidget view;
  QSignalSpy spy_selected(&view, &JobListWidget::JobSelected);

  view.SetJobModel(&model);
  EXPECT_EQ(view.GetSelectedJob(), nullptr);

  // selecting an item and checking results
  view.SetSelectedJob(job);
  EXPECT_EQ(view.GetSelectedJob(), job);

  EXPECT_EQ(mvvm::test::GetSendItem<oac_tree_gui::JobItem*>(spy_selected), job);

  // removing selection

  view.SetSelectedJob(nullptr);
  EXPECT_EQ(view.GetSelectedJob(), nullptr);
  EXPECT_EQ(spy_selected.count(), 1);

  EXPECT_EQ(mvvm::test::GetSendItem<oac_tree_gui::JobItem*>(spy_selected), nullptr);
}

//! Removing selected and checking notifications

TEST_F(JobListWidgetTest, SelectionAfterRemoval)
{
  JobModel model;
  auto job = model.InsertItem<LocalJobItem>();

  JobListWidget view;
  view.SetJobModel(&model);

  // selecting single item
  view.SetSelectedJob(job);

  // checking selections
  QSignalSpy spy_selected(&view, &JobListWidget::JobSelected);

  // removing item
  model.RemoveItem(job);

  // signal should emit once and report nullptr as selected item
  EXPECT_EQ(mvvm::test::GetSendItem<oac_tree_gui::JobItem*>(spy_selected), nullptr);
}

//! Checking selection when acting through the view.

TEST_F(JobListWidgetTest, SetCurrentIndex)
{
  JobModel model;
  auto job = model.InsertItem<LocalJobItem>();

  JobListWidget view;
  view.SetJobModel(&model);
  QSignalSpy spy_selected(&view, &JobListWidget::JobSelected);

  // checking indexes
  auto job_indexes = view.GetViewModel()->GetIndexOfSessionItem(job);
  ASSERT_EQ(job_indexes.size(), 1);

  auto job_status_indexes =
      view.GetViewModel()->GetIndexOfSessionItem(job->GetItem(itemconstants::kStatus));
  ASSERT_EQ(job_status_indexes.size(), 1);

  view.GetTreeView()->setCurrentIndex(job_indexes.at(0));

  EXPECT_EQ(view.GetSelectedJob(), job);

  EXPECT_EQ(mvvm::test::GetSendItem<oac_tree_gui::JobItem*>(spy_selected), job);
}

}  // namespace oac_tree_gui::test
