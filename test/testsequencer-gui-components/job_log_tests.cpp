/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/jobsystem/job_log.h"

#include <gtest/gtest.h>

#include <QSignalSpy>

using namespace sequencergui;

//! Tests for JobItem class.

class JobLogTest : public ::testing::Test
{
};

TEST_F(JobLogTest, InitialState)
{
  JobLog job_log;
  EXPECT_EQ(job_log.GetSize(), 0);
}

TEST_F(JobLogTest, Append)
{
  JobLog job_log;

  auto log_event = CreateLogEvent(Severity::kNotice, "abc");
  QSignalSpy spy_appended(&job_log, &JobLog::LogEventAppended);
  QSignalSpy spy_cleared(&job_log, &JobLog::LogCleared);

  job_log.Append(log_event);

  EXPECT_EQ(job_log.GetSize(), 1);
  EXPECT_EQ(job_log.At(0), log_event);

  EXPECT_EQ(spy_appended.count(), 1);
  EXPECT_EQ(spy_cleared.count(), 0);
}

TEST_F(JobLogTest, ClearLog)
{
  JobLog job_log;

  auto log_event = CreateLogEvent(Severity::kNotice, "abc");
  job_log.Append(log_event);

  QSignalSpy spy_appended(&job_log, &JobLog::LogEventAppended);
  QSignalSpy spy_cleared(&job_log, &JobLog::LogCleared);

  job_log.ClearLog();

  EXPECT_EQ(job_log.GetSize(), 0);

  EXPECT_EQ(spy_appended.count(), 0);
  EXPECT_EQ(spy_cleared.count(), 1);
}
