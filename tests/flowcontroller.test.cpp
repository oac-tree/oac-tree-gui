/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/monitor/flowcontroller.h"

#include <gtest/gtest.h>

#include <chrono>
#include <future>

namespace
{
const auto duration = [](auto time_interval)
{ return std::chrono::duration_cast<std::chrono::milliseconds>(time_interval).count(); };
}

using namespace sequi;

//! Tests for FlowController class.

class FlowControllerTest : public ::testing::Test
{
public:
  using clock_used = std::chrono::high_resolution_clock;
  using time_t = std::chrono::time_point<clock_used>;
  using duration_unit = std::chrono::milliseconds;
  using msec = std::chrono::milliseconds;
};

TEST_F(FlowControllerTest, ProceedMode)
{
  FlowController controller;

  controller.SetWaitingMode(WaitingMode::kProceed);

  time_t start_time = clock_used::now();
  controller.WaitIfNecessary();
  time_t end_time = clock_used::now();

  controller.WaitIfNecessary();

  // not clear how to test immmediate execution
  EXPECT_TRUE(duration(end_time - start_time) < 10);
}

TEST_F(FlowControllerTest, SleepMode)
{
  FlowController controller;
  controller.SetSleepTime(100);
  controller.SetWaitingMode(WaitingMode::kSleepFor);

  time_t start_time = clock_used::now();
  controller.WaitIfNecessary();
  time_t end_time = clock_used::now();

  EXPECT_TRUE(duration(end_time - start_time) >= 100);
}

TEST_F(FlowControllerTest, WaitForReleaseMode)
{
  FlowController controller;
  controller.SetWaitingMode(WaitingMode::kWaitForRelease);
  time_t start_time;
  time_t end_time;
  std::future<void> waiting_done;
  std::promise<void> ready_for_test;

  waiting_done = std::async(std::launch::async,
                            [&controller, &start_time, &end_time, &ready_for_test]()
                            {
                              ready_for_test.set_value();
                              start_time = clock_used::now();
                              controller.WaitIfNecessary();
                              end_time = clock_used::now();
                            });

  // waiting for threads being prepared for racing
  ready_for_test.get_future().wait();

  std::this_thread::sleep_for(msec(100));
  controller.StepRequest();
  // checking result
  waiting_done.get();  // making sure pushing thread has finished
  EXPECT_TRUE(duration(end_time - start_time) >= 100);
}

TEST_F(FlowControllerTest, WaitForReleaseChange)
{
  FlowController controller;
  controller.SetWaitingMode(WaitingMode::kWaitForRelease);
  time_t start_time;
  time_t end_time;
  std::future<void> waiting_done;
  std::promise<void> ready_for_test;

  waiting_done = std::async(std::launch::async,
                            [&controller, &start_time, &end_time, &ready_for_test]()
                            {
                              ready_for_test.set_value();
                              start_time = clock_used::now();
                              controller.WaitIfNecessary();
                              end_time = clock_used::now();
                            });

  // waiting for threads being prepared for racing
  ready_for_test.get_future().wait();

  std::this_thread::sleep_for(msec(100));

  // change of waiting mode should release waiting
  controller.SetWaitingMode(WaitingMode::kProceed);

  // checking result
  waiting_done.get();  // making sure pushing thread has finished
  EXPECT_TRUE(duration(end_time - start_time) >= 100);
}
