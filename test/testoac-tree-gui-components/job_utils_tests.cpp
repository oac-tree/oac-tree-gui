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

#include <gtest/gtest.h>

#include "oac-tree-gui/jobsystem/job_utils.h"

namespace oac_tree_gui::test
{

//! Tests for utility methods in job_utils.h

class JobUtilsTest : public ::testing::Test
{
};

TEST_F(JobUtilsTest, GetRegExpPattern)
{
  EXPECT_EQ(GetRegExpPattern(std::vector<std::string>()), std::string("()"));
  EXPECT_EQ(GetRegExpPattern(std::vector<std::string>({"abc"})), std::string("(abc)"));
  EXPECT_EQ(GetRegExpPattern(std::vector<std::string>({"INFO", "DEBUG"})),
            std::string("(INFO|DEBUG)"));
}

}  // namespace oac_tree_gui::test
