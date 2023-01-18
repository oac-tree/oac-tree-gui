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

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for JobItem class.

class JobLogTests : public ::testing::Test
{
};

TEST_F(JobLogTests, InitialState)
{
  JobLog job_log;
  EXPECT_EQ(job_log.GetSize(), 0);
}

TEST_F(JobLogTests, Append)
{
  JobLog job_log;

  auto log_event = CreateLogEvent(Severity::kNotice, "abc");
  job_log.Append(log_event);

  EXPECT_EQ(job_log.GetSize(), 1);
  EXPECT_EQ(job_log.At(0), log_event);
}
