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

#include "sequencergui/jobsystem/user_controller.h"

#include <gtest/gtest.h>

#include <QTest>
#include <future>

using namespace sequencergui;

//! Tests for UserController class.

class UserControllerTest : public ::testing::Test
{
};

//! Testing UserController::GetUserInput method.
//! - Create a callback mimicking user responce.
//! - Run a thread with blocking call GetUserInput().
//! - Wait with QTest::qWait to make event loop rolling.
//! - Checking user input as reported by runner thread.

TEST_F(UserControllerTest, SetUserInput)
{
  // User input callback.
  auto on_user_input = [](auto value, auto description)
  {
    return value + description;  // Concatenates two input parameters.
  };

  UserController controller;
  controller.SetUserContext({on_user_input});

  std::promise<void> ready_for_test;
  // runner to ask for user input (blocking)
  auto runner = [&controller, &ready_for_test]()
  {
    const std::string value("value");
    const std::string description("description");
    ready_for_test.set_value();
    return controller.GetUserInput(value, description);
  };

  // launching runner in a thread
  std::future<std::string> result_obtained = std::async(std::launch::async, runner);

  // waiting for threads being prepared for racing
  ready_for_test.get_future().wait();

  // processing event loop to make queued connection passing in UserController
  QTest::qWait(100);

  // making sure the thread has finished
  auto result = result_obtained.get();

  // result from the thread should contain the result of concatenation
  EXPECT_EQ(result, std::string("valuedescription"));
}

//! Testing UserController::GetUserInput method.
//! - Create a callback mimicking user responce.
//! - Run a thread with blocking call GetUserInput().
//! - Wait with QTest::qWait to make event loop rolling.
//! - Checking user input as reported by runner thread.

//TEST_F(UserControllerTest, SeingleUserChoice)
//{
//  const int user_choice = {42};
//  // User choice callback.
//  auto on_user_choice = [user_choice](auto choices, auto description)
//  {
//    (void)choices;
//    (void)description;
//    return user_choice;
//  };

//  UserController controller;
//  controller.SetUserContext({{}, on_user_choice});

//  std::promise<void> ready_for_test;
//  // runner to ask for user input (blocking)
//  auto runner = [&controller, &ready_for_test]()
//  {
//    const std::vector<std::string> choices({"a", "b", "c"});
//    const std::string description("description");
//    ready_for_test.set_value();
//    return controller.GetUserChoice(choices, description);
//  };

//  // launching runner in a thread
//  std::future<int> result_obtained = std::async(std::launch::async, runner);

//  // waiting for threads being prepared for racing
//  ready_for_test.get_future().wait();

//  // processing event loop to make queued connection passing in UserController
//  QTest::qWait(100);

//  // making sure the thread has finished
//  auto result = result_obtained.get();

//  // result from the thread should contain the result of concatenation
//  EXPECT_EQ(result, user_choice);
//}
