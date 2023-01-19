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

#include "sequencergui/jobsystem/job_log_severity.h"

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for helper functions in job_log_severity.h

class JobLogSeverityTests : public ::testing::Test
{
};

TEST_F(JobLogSeverityTests, SeverityToString)
{
  EXPECT_EQ(SeverityToString(Severity::kEmergency), std::string("EMERGENCY"));
  EXPECT_EQ(SeverityToString(Severity::kAlert), std::string("ALERT"));
  EXPECT_EQ(SeverityToString(Severity::kCritical), std::string("CRITICAL"));
  EXPECT_EQ(SeverityToString(Severity::kError), std::string("ERROR"));
  EXPECT_EQ(SeverityToString(Severity::kWarning), std::string("WARNING"));
  EXPECT_EQ(SeverityToString(Severity::kNotice), std::string("NOTICE"));
  EXPECT_EQ(SeverityToString(Severity::kInfo), std::string("INFO"));
  EXPECT_EQ(SeverityToString(Severity::kDebug), std::string("DEBUG"));
  EXPECT_EQ(SeverityToString(Severity::kTrace), std::string("TRACE"));
}
