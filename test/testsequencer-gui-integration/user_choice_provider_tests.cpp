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

#include "sequencergui/jobsystem/user_choice_provider.h"

#include <gtest/gtest.h>

#include <QTest>
#include <chrono>
#include <future>
#include <iostream>

using namespace sequencergui;

//! Tests for UserChoiceProvider class.

class UserChoiceProviderTest : public ::testing::Test
{
};

//! - Create a callback mimicking user responce.
//! - Run a thread with blocking call GetUserInput().
//! - Wait with QTest::qWait to make event loop rolling.
//! - Checking user input as reported by runner thread.

TEST_F(UserChoiceProviderTest, SetUserChoice)
{
  const std::vector<std::string> choices({"a", "b", "c"});
  const std::string description("description");
  const UserChoiceArgs args{choices, description};
  UserChoiceResult expected_result{42, true};

  // User choice callback.
  auto on_user_choice = [expected_result](auto) { return expected_result; };

  UserChoiceProvider provider(on_user_choice);

  std::promise<void> ready_for_test;
  // runner to ask for user input (blocking)
  auto runner = [&provider, &ready_for_test, &args]()
  {
    const std::vector<std::string> choices({"a", "b", "c"});
    const std::string description("description");
    ready_for_test.set_value();
    return provider.GetUserChoice(args);
  };

  // launching runner in a thread
  std::future<UserChoiceResult> result_obtained = std::async(std::launch::async, runner);

  // waiting for threads being prepared for racing
  ready_for_test.get_future().wait();

  // processing event loop to make queued connection passing in UserChoiceProvider
  QTest::qWait(100);

  // making sure the thread has finished
  auto result = result_obtained.get();

  // result from the thread should contain the result of concatenation
  EXPECT_EQ(result.index, 43);
  EXPECT_EQ(result.processed, true);
}
