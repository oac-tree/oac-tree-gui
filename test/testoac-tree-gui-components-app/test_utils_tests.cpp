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

#include "testutils/test_utils.h"

#include <gtest/gtest.h>

#include <QTimer>
#include <future>

using msec = std::chrono::milliseconds;

namespace oac_tree_gui::test
{

/**
 * @brief Tests for Qt event loop waits from test_utils.h.
 */
class TestUtilsTest : public ::testing::Test
{
};

TEST_F(TestUtilsTest, WaitQtForPredicateAlreadyTrue)
{
  EXPECT_TRUE(WaitQtFor([]() { return true; }));
}

//! WaitQtFor processes queued events.
TEST_F(TestUtilsTest, WaitQtForProcessesEvents)
{
  bool is_called{false};
  QTimer::singleShot(0, [&is_called]() { is_called = true; });
  EXPECT_FALSE(is_called);

  EXPECT_TRUE(WaitQtFor([&is_called]() { return is_called; }));
}

TEST_F(TestUtilsTest, WaitQtForTimeout)
{
  EXPECT_FALSE(WaitQtFor([]() { return false; }, msec(10)));
}

//! WaitForReady processes queued events while waiting.
TEST_F(TestUtilsTest, WaitForReady)
{
  std::promise<int> promise;
  auto future = promise.get_future();
  QTimer::singleShot(0, [&promise]() { promise.set_value(42); });

  EXPECT_TRUE(WaitForReady(future));
  EXPECT_EQ(future.get(), 42);
}

TEST_F(TestUtilsTest, WaitForReadyTimeout)
{
  std::promise<int> promise;
  auto future = promise.get_future();

  EXPECT_FALSE(WaitForReady(future, msec(10)));
}

}  // namespace oac_tree_gui::test
