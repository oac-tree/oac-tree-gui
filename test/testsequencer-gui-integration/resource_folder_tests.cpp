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

#include <gtest/gtest.h>

//! Tests for all XML procedures found in resource/functional folder.

class ResourceFolderTests : public ::testing::TestWithParam<std::string>
{
};

TEST_P(ResourceFolderTests, RunProcedure)
{
  EXPECT_EQ(GetParam(), std::string("aaa"));
}

INSTANTIATE_TEST_SUITE_P(FunctionalTests, ResourceFolderTests, ::testing::Values("aaa", "bbb"));
