/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_TEST_UTILS_H_
#define LIBTEST_UTILS_TESTUTILS_TEST_UTILS_H_

//! Collection of utility functions for various unit tests.

#include <chrono>
#include <functional>
#include <string>

//! Various common utils for unit tests.

namespace testutils
{
//! hard-coded value in Wait instruction
const std::chrono::milliseconds kDefaultWaitPrecision(50);

const auto duration = [](auto time_interval)
{ return std::chrono::duration_cast<std::chrono::milliseconds>(time_interval).count(); };

//! Wait for timeout in milisseconds or until predicate returns true.
bool WaitFor(std::function<bool()> predicate, std::chrono::milliseconds timeout);

template <typename T>
bool WaitForCompletion(const T& runner, std::chrono::milliseconds timeout_msec)
{
  return WaitFor([&runner]() { return !runner.IsBusy(); }, timeout_msec);
}

double GetTimeoutInSec(std::chrono::milliseconds timeout);

//! Creates Sequencer XML procedure by wrapping the body into necessary Procedure directive.
std::string CreateProcedureString(const std::string& body, bool schema = true);

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_TEST_UTILS_H_
