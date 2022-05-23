/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/jobsystem/functionrunner.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace sequencergui;
using ::testing::NiceMock;

//! Tests for JobStates.

class FunctionRunnerTest : public ::testing::Test
{
public:
};

TEST_F(FunctionRunnerTest, InitialState)
{
  //  int count{0};
  //  auto worker = [&count]() -> bool { return count++; return false; };

  FunctionRunner runner([]() { return false; });
  EXPECT_EQ(runner.GetRunnerStatus(), RunnerStatus::kIdle);
}
