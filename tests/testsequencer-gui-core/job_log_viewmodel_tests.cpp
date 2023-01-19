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

#include "sequencergui/monitor/job_log.h"
#include "sequencergui/viewmodel/job_log_viewmodel.h"

#include <gtest/gtest.h>

#include <QSignalSpy>

using namespace sequencergui;

//! Tests for JobItem class.

class JobLogViewModelTests : public ::testing::Test
{
};

TEST_F(JobLogViewModelTests, InitialState)
{
  JobLog job_log;

  JobLogViewModel view_model(&job_log);
  EXPECT_EQ(view_model.rowCount(QModelIndex()), 0);
  EXPECT_EQ(view_model.columnCount(QModelIndex()), 5);  // date, time, severity, source, message
}

//! JobLog contains a single event.
//! Checking the data method for all columns.

TEST_F(JobLogViewModelTests, SingleRowData)
{
  JobLog job_log;
  LogEvent log_event{"date", "time", Severity::kNotice, "source", "message"};
  job_log.Append(log_event);

  JobLogViewModel view_model(&job_log);
  EXPECT_EQ(view_model.rowCount(QModelIndex()), 1);
  EXPECT_EQ(view_model.columnCount(QModelIndex()), 5);

  EXPECT_EQ(view_model.data(view_model.index(0, 0), Qt::DisplayRole), QString("date"));
  EXPECT_EQ(view_model.data(view_model.index(0, 1), Qt::DisplayRole), QString("time"));
  EXPECT_EQ(view_model.data(view_model.index(0, 2), Qt::DisplayRole), QString(""));
  EXPECT_EQ(view_model.data(view_model.index(0, 3), Qt::DisplayRole), QString("source"));
  EXPECT_EQ(view_model.data(view_model.index(0, 4), Qt::DisplayRole), QString("message"));
}

TEST_F(JobLogViewModelTests, HeaderData)
{
  JobLog job_log;
  JobLogViewModel view_model(&job_log);

  EXPECT_EQ(view_model.headerData(0, Qt::Horizontal, Qt::DisplayRole), QString("date"));
  EXPECT_EQ(view_model.headerData(1, Qt::Horizontal, Qt::DisplayRole), QString("time"));
  EXPECT_EQ(view_model.headerData(2, Qt::Horizontal, Qt::DisplayRole), QString("severity"));
  EXPECT_EQ(view_model.headerData(3, Qt::Horizontal, Qt::DisplayRole), QString("source"));
  EXPECT_EQ(view_model.headerData(4, Qt::Horizontal, Qt::DisplayRole), QString("message"));
}

TEST_F(JobLogViewModelTests, Flags)
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

TEST_F(JobLogViewModelTests, AppendRow)
{
  JobLog job_log;

  JobLogViewModel view_model(&job_log);
  QSignalSpy spy_insert(&view_model, &JobLogViewModel::rowsInserted);

  job_log.Append(LogEvent{"date", "time", Severity::kNotice, "source", "message"});

  view_model.OnJobLogNewEntry();

  EXPECT_EQ(view_model.rowCount(QModelIndex()), 1);

  EXPECT_EQ(spy_insert.count(), 1);

  QList<QVariant> arguments = spy_insert.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), QModelIndex());
  EXPECT_EQ(arguments.at(1).value<int>(), 0);
  EXPECT_EQ(arguments.at(2).value<int>(), 0);
}
