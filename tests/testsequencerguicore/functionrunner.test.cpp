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
  //! Auxiliary class to listen for RunnerStatus changed events.
  class MockListener
  {
  public:
    MOCK_METHOD1(StatusChanged, void(RunnerStatus status));

    //! Creates callback to listen for status change.
    //! The call will be propagated to StatusChanged mock method to benefit from gmock.
    std::function<void(RunnerStatus)> CreateCallback()
    {
      return [this](auto status) { StatusChanged(status); };
    }
  };
};

//! Checking that listener works as expected.

TEST_F(FunctionRunnerTest, CheckListener)
{
  MockListener listener;

  EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));

  auto callback = listener.CreateCallback();
  callback(RunnerStatus::kRunning);
}

//! Initial state of FunctionRunner.

TEST_F(FunctionRunnerTest, InitialState)
{
  FunctionRunner runner([]() { return false; });
  EXPECT_EQ(runner.GetRunnerStatus(), RunnerStatus::kIdle);
  EXPECT_FALSE(runner.IsBusy());
  EXPECT_TRUE(WaitForCompletion(runner, 0.001));
}

//! Running worker which immediately returns `false`.

TEST_F(FunctionRunnerTest, StartSingleCall)
{
  MockListener listener;
  auto worker = []() { return false; };
  FunctionRunner runner(worker, listener.CreateCallback());

  // expecting two calls with status change kIdle, kRunning, kCompleted
  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kCompleted));
  }

  EXPECT_TRUE(runner.Start());  // triggering action
  WaitForCompletion(runner, 0.02);
  EXPECT_EQ(runner.GetRunnerStatus(), RunnerStatus::kCompleted);
}
