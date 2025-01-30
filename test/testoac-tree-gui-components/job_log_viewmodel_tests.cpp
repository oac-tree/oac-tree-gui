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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <oac_tree_gui/jobsystem/job_log.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

#include "oac_tree_gui/viewmodel/job_log_viewmodel.h"

namespace oac_tree_gui::test
{

//! Tests for JobItem class.

class JobLogViewModelTest : public ::testing::Test
{
};

TEST_F(JobLogViewModelTest, InitialState)
{
  JobLog job_log;

  JobLogViewModel view_model(&job_log);
  EXPECT_EQ(view_model.rowCount(QModelIndex()), 0);
  EXPECT_EQ(view_model.columnCount(QModelIndex()), 5);  // date, time, severity, source, message
}

//! JobLog contains a single event.
//! Checking the data method for all columns.

TEST_F(JobLogViewModelTest, SingleRowData)
{
  JobLog job_log;
  LogEvent log_event{"date", "time", Severity::kNotice, "source", "message"};
  job_log.Append(log_event);

  JobLogViewModel view_model(&job_log);
  EXPECT_EQ(view_model.rowCount(QModelIndex()), 1);
  EXPECT_EQ(view_model.columnCount(QModelIndex()), 5);

  EXPECT_EQ(view_model.data(view_model.index(0, 0), Qt::DisplayRole), QString("date"));
  EXPECT_EQ(view_model.data(view_model.index(0, 1), Qt::DisplayRole), QString("time"));
  EXPECT_FALSE(view_model.data(view_model.index(0, 2), Qt::DisplayRole).toString().isEmpty());
  EXPECT_EQ(view_model.data(view_model.index(0, 3), Qt::DisplayRole), QString("source"));
  EXPECT_EQ(view_model.data(view_model.index(0, 4), Qt::DisplayRole), QString("message"));
}

TEST_F(JobLogViewModelTest, HeaderData)
{
  JobLog job_log;
  JobLogViewModel view_model(&job_log);

  EXPECT_EQ(view_model.headerData(0, Qt::Horizontal, Qt::DisplayRole), QString("date"));
  EXPECT_EQ(view_model.headerData(1, Qt::Horizontal, Qt::DisplayRole), QString("time"));
  EXPECT_EQ(view_model.headerData(2, Qt::Horizontal, Qt::DisplayRole), QString("severity"));
  EXPECT_EQ(view_model.headerData(3, Qt::Horizontal, Qt::DisplayRole), QString("source"));
  EXPECT_EQ(view_model.headerData(4, Qt::Horizontal, Qt::DisplayRole), QString("message"));
}

TEST_F(JobLogViewModelTest, Flags)
{
  JobLog job_log;
  LogEvent log_event{"date", "time", Severity::kNotice, "source", "message"};
  job_log.Append(log_event);

  JobLogViewModel view_model(&job_log);

  for (int col = 0; col < 5; ++col)
  {
    EXPECT_TRUE(view_model.flags(view_model.index(0, col)) & Qt::ItemIsSelectable);
    EXPECT_TRUE(view_model.flags(view_model.index(0, col)) & Qt::ItemIsEnabled);
    EXPECT_FALSE(view_model.flags(view_model.index(0, col)) & Qt::ItemIsEditable);
  }
}

TEST_F(JobLogViewModelTest, AppendRow)
{
  JobLog job_log;

  JobLogViewModel view_model(&job_log);
  QSignalSpy spy_insert(&view_model, &JobLogViewModel::rowsInserted);

  job_log.Append(LogEvent{"date", "time", Severity::kNotice, "source", "message"});

  EXPECT_EQ(view_model.rowCount(QModelIndex()), 1);
  EXPECT_EQ(spy_insert.count(), 1);

  QList<QVariant> arguments = spy_insert.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), QModelIndex());
  EXPECT_EQ(arguments.at(1).value<int>(), 0);
  EXPECT_EQ(arguments.at(2).value<int>(), 0);
}

TEST_F(JobLogViewModelTest, ResetJobLog)
{
  JobLog job_log;
  job_log.Append(LogEvent{"date", "time", Severity::kNotice, "source", "message"});

  JobLogViewModel view_model(&job_log);

  QSignalSpy spy_about_to_reset(&view_model, &JobLogViewModel::modelAboutToBeReset);
  QSignalSpy spy_reset(&view_model, &JobLogViewModel::modelReset);

  EXPECT_EQ(view_model.rowCount(QModelIndex()), 1);

  job_log.ClearLog();

  EXPECT_EQ(spy_about_to_reset.count(), 1);
  EXPECT_EQ(spy_reset.count(), 1);

  EXPECT_EQ(view_model.rowCount(QModelIndex()), 0);
}

TEST_F(JobLogViewModelTest, ResetJobLogToNull)
{
  JobLog job_log;
  job_log.Append(LogEvent{"date", "time", Severity::kNotice, "source", "message"});

  JobLogViewModel view_model(&job_log);

  QSignalSpy spy_about_to_reset(&view_model, &JobLogViewModel::modelAboutToBeReset);
  QSignalSpy spy_reset(&view_model, &JobLogViewModel::modelReset);

  EXPECT_EQ(view_model.rowCount(QModelIndex()), 1);

  view_model.SetLog(nullptr);

  // shouldn't affect viewmodel anymore
  job_log.Append(LogEvent{"date", "time", Severity::kNotice, "source", "message"});

  EXPECT_EQ(spy_about_to_reset.count(), 1);
  EXPECT_EQ(spy_reset.count(), 1);

  EXPECT_EQ(view_model.rowCount(QModelIndex()), 0);
}

TEST_F(JobLogViewModelTest, SwitchToAnotherJobLog)
{
  JobLog job_log1;
  job_log1.Append(LogEvent{"date", "time", Severity::kNotice, "source", "message"});

  JobLog job_log2;
  job_log2.Append(LogEvent{"date", "time", Severity::kNotice, "source", "message"});
  job_log2.Append(LogEvent{"date", "time", Severity::kNotice, "source", "message"});

  // initialise ViewModel with the first log
  JobLogViewModel view_model(&job_log1);

  QSignalSpy spy_about_to_reset(&view_model, &JobLogViewModel::modelAboutToBeReset);
  QSignalSpy spy_reset(&view_model, &JobLogViewModel::modelReset);
  QSignalSpy spy_insert(&view_model, &JobLogViewModel::rowsInserted);

  EXPECT_EQ(view_model.rowCount(QModelIndex()), 1);

  // switch to a second log
  view_model.SetLog(&job_log2);

  EXPECT_EQ(spy_about_to_reset.count(), 1);
  EXPECT_EQ(spy_reset.count(), 1);
  EXPECT_EQ(spy_insert.count(), 0);
  EXPECT_EQ(view_model.rowCount(QModelIndex()), 2);

  // check that viewmodel keeps functioning
  job_log2.Append(LogEvent{"date", "time", Severity::kNotice, "source", "message"});
  // and that first lock doesn't influence
  job_log1.Append(LogEvent{"date", "time", Severity::kNotice, "source", "message"});
  job_log1.Append(LogEvent{"date", "time", Severity::kNotice, "source", "message"});

  EXPECT_EQ(spy_insert.count(), 1);
  EXPECT_EQ(view_model.rowCount(QModelIndex()), 3);
}

}  // namespace oac_tree_gui::test
