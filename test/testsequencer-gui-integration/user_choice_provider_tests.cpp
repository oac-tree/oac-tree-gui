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

//! Single consumer thread asks for user choice.

TEST_F(UserChoiceProviderTest, SingleThreadAsksForUserChoice)
{
  const std::vector<std::string> choices({"a", "b", "c"});
  const sup::dto::AnyValue metadata;
  const UserChoiceArgs args{choices, metadata};

  UserChoiceResult expected_result{42, true};

  // User choice callback.
  auto on_user_choice = [expected_result](auto) { return expected_result; };

  UserChoiceProvider provider(on_user_choice);

  std::promise<void> ready_for_test;
  // runner to ask for user input (blocking)
  auto runner = [&provider, &ready_for_test, &args]()
  {
    ready_for_test.set_value();
    return provider.GetUserChoice(args);
  };

  // launching runner in a thread
  std::future<UserChoiceResult> future_result = std::async(std::launch::async, runner);

  // waiting for threads being prepared for racing
  ready_for_test.get_future().wait();

  // processing event loop to make queued connection passing in UserChoiceProvider
  QTest::qWait(100);

  // making sure the thread has finished
  auto result = future_result.get();

  // validating
  EXPECT_EQ(result.index, 42);
  EXPECT_EQ(result.processed, true);
}

TEST_F(UserChoiceProviderTest, TwoThreadsAskForUserChoice)
{
  const std::vector<std::string> choices({"a", "b", "c"});
  const sup::dto::AnyValue metadata;
  const UserChoiceArgs args{choices, metadata};

  // User choice callback. Will prvide `0` for the first call, and `1` for the second.
  int user_selection{0};
  auto on_user_choice = [&user_selection](auto) {
    return UserChoiceResult{user_selection++, true};
  };

  UserChoiceProvider provider(on_user_choice);

  std::promise<void> ready_for_test1;

  // runner to ask for user input (blocking)
  auto consumer1 = [&provider, &ready_for_test1, &args]()
  {
    ready_for_test1.set_value();
    return provider.GetUserChoice(args);
  };

  std::promise<void> ready_for_test2;
  // runner to ask for user input (blocking)
  auto consumer2 = [&provider, &ready_for_test2, &args]()
  {
    ready_for_test2.set_value();
    return provider.GetUserChoice(args);
  };

  // launching runner in a thread
  std::future<UserChoiceResult> future_result1 = std::async(std::launch::async, consumer1);
  std::future<UserChoiceResult> future_result2 = std::async(std::launch::async, consumer2);

  // waiting for threads being prepared for racing
  ready_for_test1.get_future().wait();
  ready_for_test2.get_future().wait();

  // processing event loop to make queued connection passing in UserChoiceProvider
  QTest::qWait(100);

  // making sure the thread has finished
  auto result1 = future_result1.get();
  auto result2 = future_result2.get();

  std::vector<int> user_choices;  // as reported by two threads
  user_choices.push_back(result1.index);
  user_choices.push_back(result2.index);
  std::sort(user_choices.begin(), user_choices.end());
  ASSERT_EQ(user_choices.size(), 2);
  EXPECT_EQ(user_choices.at(0), 0);
  EXPECT_EQ(user_choices.at(1), 1);
}
