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

#include "oac_tree_gui/jobsystem/objects/job_log.h"

#include <gtest/gtest.h>

#include <QSignalSpy>

namespace oac_tree_gui::test
{

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

}  // namespace oac_tree_gui::test
