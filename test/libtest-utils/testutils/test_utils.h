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

#ifndef LIBTEST_UTILS_TESTUTILS_TEST_UTILS_H_
#define LIBTEST_UTILS_TESTUTILS_TEST_UTILS_H_

//! @file
//! Collection of utility functions for various unit tests.

#include <chrono>
#include <functional>
#include <future>
#include <string>

namespace oac_tree_gui::test
{

//! Default timeout for waits in the Qt event loop.
constexpr std::chrono::milliseconds kWaitTimeout{20000};

/**
 * @brief Wait for timeout in milisseconds or until predicate returns true.
 */
bool WaitFor(std::function<bool()> predicate, std::chrono::milliseconds timeout);

/**
 * @brief Processes Qt events until the predicate returns true or the timeout expires.
 *
 * @return True if the predicate returned true.
 */
bool WaitQtFor(const std::function<bool()>& predicate,
               std::chrono::milliseconds timeout = kWaitTimeout);

/**
 * @brief Processes Qt events until the future is ready or the timeout expires.
 *
 * @return True if the future is ready.
 */
template <typename T>
bool WaitForReady(const std::future<T>& future, std::chrono::milliseconds timeout = kWaitTimeout)
{
  return WaitQtFor(
      [&future]() { return future.wait_for(std::chrono::seconds(0)) == std::future_status::ready; },
      timeout);
}

/**
 * @brief Converts milliseconds in timeout as expected by Wait instruction.
 */
double GetTimeoutInSec(std::chrono::milliseconds timeout);

/**
 * @brief Creates oac-tree XML procedure by wrapping the body into necessary Procedure directive.
 */
std::string CreateProcedureString(const std::string& body, bool schema = true);

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_TEST_UTILS_H_
