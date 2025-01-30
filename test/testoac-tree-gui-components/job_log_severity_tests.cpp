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

#include "oac-tree-gui/jobsystem/job_log_severity.h"

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Tests for helper functions in job_log_severity.h

class JobLogSeverityTest : public ::testing::Test
{
};

TEST_F(JobLogSeverityTest, SeverityToString)
{
  EXPECT_EQ(ToString(Severity::kEmergency), std::string("EMERGENCY"));
  EXPECT_EQ(ToString(Severity::kAlert), std::string("ALERT"));
  EXPECT_EQ(ToString(Severity::kCritical), std::string("CRITICAL"));
  EXPECT_EQ(ToString(Severity::kError), std::string("ERROR"));
  EXPECT_EQ(ToString(Severity::kWarning), std::string("WARNING"));
  EXPECT_EQ(ToString(Severity::kNotice), std::string("NOTICE"));
  EXPECT_EQ(ToString(Severity::kInfo), std::string("INFO"));
  EXPECT_EQ(ToString(Severity::kDebug), std::string("DEBUG"));
  EXPECT_EQ(ToString(Severity::kTrace), std::string("TRACE"));
}

}  // namespace oac_tree_gui::test
