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

#include <oac_tree_gui/jobsystem/request_types.h>

#include <gtest/gtest.h>

#include "oac_tree_gui/jobsystem/request_handler.h"

namespace oac_tree_gui::test
{

//! Tests for RequestHandler class.

class RequestHandlerTest : public ::testing::Test
{
};

TEST_F(RequestHandlerTest, SetUserInput)
{
  const std::vector<std::string> choices({"a", "b", "c"});
  const sup::dto::AnyValue metadata;
  const UserChoiceArgs args{choices, metadata};

  RequestHandler<UserChoiceResult> handler;

  std::promise<void> ready_for_test;
  // runner to ask for user input (blocking)
  auto consumer = [&handler, &ready_for_test, &args]()
  {
    ready_for_test.set_value();
    return handler.GetData();
  };

  std::future<UserChoiceResult> future_result = std::async(std::launch::async, consumer);

  // waiting for consumer thread being ready
  ready_for_test.get_future().wait();

  handler.SendData(UserChoiceResult{42, true});

  // checking result
  auto result = future_result.get();  // making sure pushing thread has finished
  EXPECT_EQ(result.index, 42);
  EXPECT_EQ(result.processed, true);
}

}  // namespace oac_tree_gui::test
