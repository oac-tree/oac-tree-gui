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

#include "sequencergui/jobsystem/request_handler.h"

#include <sequencergui/jobsystem/request_types.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests for UserController class.

class RequestHandleTest : public ::testing::Test
{
};

TEST_F(RequestHandleTest, SetUserInput)
{
  const std::vector<std::string> choices({"a", "b", "c"});
  const std::string description("description");
  const UserChoiceArgs args{choices, description};

  RequestHandler<UserChoiceResult, UserChoiceArgs> handle;

  std::promise<void> ready_for_test;
  // runner to ask for user input (blocking)
  auto consumer = [&handle, &ready_for_test, &args]()
  {
    ready_for_test.set_value();
    return handle.GetData(args);
  };

  std::future<UserChoiceResult> future_result = std::async(std::launch::async, consumer);

  // waiting for consumer thread being ready
  ready_for_test.get_future().wait();

  handle.SendData(UserChoiceResult{42, true});

  // checking result
  auto result = future_result.get();  // making sure pushing thread has finished
  EXPECT_EQ(result.index, 42);
  EXPECT_EQ(result.processed, true);
}
