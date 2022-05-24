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

#include "sequencergui/jobsystem/functionrunnerv2.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <thread>

using namespace sequencergui;
using ::testing::NiceMock;
using msec = std::chrono::milliseconds;

//! Tests for JobStates.

class FunctionRunnerV2Test : public ::testing::Test
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

TEST_F(FunctionRunnerV2Test, CheckListener)
{
  MockListener listener;

  EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));

  auto callback = listener.CreateCallback();
  callback(RunnerStatus::kRunning);
}

//! Initial state of FunctionRunner.

TEST_F(FunctionRunnerV2Test, InitialState)
{
  FunctionRunnerV2 runner([]() { return false; });
  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kIdle);
  EXPECT_FALSE(runner.IsBusy());
  EXPECT_TRUE(WaitForCompletion(runner, 0.001));
}

//! Start and normal completion of the short task.

//TEST_F(FunctionRunnerV2Test, ShortTaskNormalCompletion)
//{
//  MockListener listener;
//  auto worker = []() { return false; };  // worker asks to exit immediately
//  FunctionRunnerV2 runner(worker, listener.CreateCallback());

//  {  // expecting calls with status change in this order
//    ::testing::InSequence seq;
//    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kRunning));
//    EXPECT_CALL(listener, StatusChanged(RunnerStatus::kCompleted));
//  }

//  EXPECT_TRUE(runner.PerformAction(JobAction::kStart));  // triggering action
//  WaitForCompletion(runner, 0.02);
//  EXPECT_EQ(runner.GetStatus(), RunnerStatus::kCompleted);
//  EXPECT_FALSE(runner.IsBusy());
//}
