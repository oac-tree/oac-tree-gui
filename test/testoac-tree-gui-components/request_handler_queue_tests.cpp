/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "oac_tree_gui/jobsystem/request_handler_queue.h"

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Unit tests for RequestHandlerQueue class.

class RequestHandlerQueueTest : public ::testing::Test
{
public:
};

TEST_F(RequestHandlerQueueTest, SingleThreadAskForData)
{
  const std::string request_params("abc");

  // Callback mimicking user responce from the GUI thread. It has a string as an input argument,
  // and it will return size of string as user responce.
  auto data_provider = [](const std::string& args) { return static_cast<int>(args.size()); };

  // handler will process thread's request (strings) and provide it with answers (integers)
  RequestHandlerQueue<int, std::string> handler(data_provider);

  // callback mimicking queued request for data, will trigger data_provider call
  auto queued_request = [&handler]() { handler.OnDataRequest(); };

  std::promise<void> ready_for_test;

  // runner to ask for user input (blocking)
  auto consumer = [&handler, &ready_for_test, queued_request, request_params]()
  {
    ready_for_test.set_value();
    return handler.GetData(request_params, queued_request);
  };

  std::future<int> future_result = std::async(std::launch::async, consumer);

  // waiting for consumer thread being ready
  ready_for_test.get_future().wait();

  // checking result
  auto result = future_result.get();  // making sure thread has finished
  EXPECT_EQ(result, request_params.size());
}

}  // namespace oac_tree_gui::test
