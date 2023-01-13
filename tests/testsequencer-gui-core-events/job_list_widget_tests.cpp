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

#include "sequencergui/monitor/job_list_widget.h"

#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/item_constants.h>

#include <mvvm/viewmodel/viewmodel.h>

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QTreeView>

using namespace sequencergui;

Q_DECLARE_METATYPE(sequencergui::JobItem*)

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
  auto job = model.InsertItem<JobItem>();

  JobListWidget view;
  QSignalSpy spy_selected(&view, &JobListWidget::JobSelected);

  view.SetJobModel(&model);
  EXPECT_EQ(view.GetSelectedJob(), nullptr);

  // selecting an item and checking results
  view.SetSelectedJob(job);
  EXPECT_EQ(view.GetSelectedJob(), job);
  EXPECT_EQ(spy_selected.count(), 1);
  QList<QVariant> arguments = spy_selected.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto selected_job = arguments.at(0).value<sequencergui::JobItem*>();
  EXPECT_EQ(selected_job, job);

  spy_selected.clear();

  // removing selection

  view.SetSelectedJob(nullptr);
  EXPECT_EQ(view.GetSelectedJob(), nullptr);
  EXPECT_EQ(spy_selected.count(), 1);

  arguments = spy_selected.takeFirst();
  selected_job = arguments.at(0).value<sequencergui::JobItem*>();
  EXPECT_EQ(selected_job, nullptr);
}

//! Removing selected and checking notifications

TEST_F(JobListWidgetTest, SelectionAfterRemoval)
{
  JobModel model;
  auto job = model.InsertItem<JobItem>();

  JobListWidget view;
  view.SetJobModel(&model);

  // selecting single item
  view.SetSelectedJob(job);

  // checking selections
  QSignalSpy spy_selected(&view, &JobListWidget::JobSelected);

  // removing item
  model.RemoveItem(job);

  // signal should emit once and report nullptr as selected item
  EXPECT_EQ(spy_selected.count(), 1);
  auto arguments = spy_selected.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto selected_job = arguments.at(0).value<sequencergui::JobItem*>();
  EXPECT_EQ(selected_job, nullptr);
}

//! Checking selection when acting through the view.

TEST_F(JobListWidgetTest, SetCurrentIndex)
{
  JobModel model;
  auto job = model.InsertItem<JobItem>();

  JobListWidget view;
  view.SetJobModel(&model);
  QSignalSpy spy_selected(&view, &JobListWidget::JobSelected);

  // checking indexes
  auto job_indexes = view.GetViewModel()->GetIndexOfSessionItem(job);
  ASSERT_EQ(job_indexes.size(), 1);

  auto job_status_indexes = view.GetViewModel()->GetIndexOfSessionItem(job->GetItem(itemconstants::kStatus));
  ASSERT_EQ(job_status_indexes.size(), 1);

  view.GetTreeView()->setCurrentIndex(job_indexes.at(0));

  EXPECT_EQ(view.GetSelectedJob(), job);
  EXPECT_EQ(spy_selected.count(), 1);
  QList<QVariant> arguments = spy_selected.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto selected_job = arguments.at(0).value<sequencergui::JobItem*>();
  EXPECT_EQ(selected_job, job);
}
