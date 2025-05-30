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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

#include <QTest>
#include <future>

#include "oac_tree_gui/jobsystem/user_input_provider.h"

namespace oac_tree_gui::test
{

//! Tests for UserInputProvider class.

class UserInputProviderTest : public ::testing::Test
{
};

//! Single consumer thread asks for user input.

TEST_F(UserInputProviderTest, SingleThreadAsksForUserInput)
{
  const sup::dto::AnyValue initial_value{sup::dto::SignedInteger32Type, 41};
  const std::string description("description");
  const UserInputArgs args{initial_value, description};

  const sup::dto::AnyValue expected_value{sup::dto::SignedInteger32Type, 42};
  const UserInputResult expected_result{expected_value, true};

  // User input callback. Returns input AnyValue + 1.
  auto on_user_input = [](const UserInputArgs& args)
  {
    auto initial_value = args.value;
    sup::dto::AnyValue new_value{sup::dto::SignedInteger32Type, initial_value.As<int>() + 1};
    return UserInputResult{new_value, true};
  };

  UserInputProvider provider(on_user_input);

  std::promise<void> ready_for_test;
  // runner to ask for user input (blocking)
  auto runner = [&provider, &ready_for_test, &args]()
  {
    ready_for_test.set_value();
    return provider.GetUserInput(args);
  };

  // launching runner in a thread
  std::future<UserInputResult> future_result = std::async(std::launch::async, runner);

  // waiting for threads being prepared for racing
  ready_for_test.get_future().wait();

  // processing event loop to make queued connection passing in UserChoiceProvider
  QTest::qWait(100);

  // making sure the thread has finished
  auto result = future_result.get();

  // validating
  EXPECT_EQ(result.value, expected_value);
  EXPECT_EQ(result.processed, true);
}

}  // namespace oac_tree_gui::test
