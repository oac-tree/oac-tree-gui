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
//! Returns full path to the main test folder, as defined by CMake at compile time.
//! Shoud point to CMAKE_BINARY_DIR/test_output
std::string GetTestSuiteOutputDir();

//! Returns full path to repository source directory.
std::string GetProjectSourceDir();

//! Returns true if given item can be casted to desired type.
template <typename DesiredT, typename ItemT>
bool CanCast(const ItemT* item)
{
  return dynamic_cast<const DesiredT*>(item) != nullptr;
}

//! Returns true if clone method is implemented.
template <typename T>
bool IsCloneImplemented()
{
  // We expect that the specified object can be created, cloned, and the result of clone can
  // be casted to the object type itself.
  T item;
  auto clone = item.Clone(/*make_unique_id*/ false);
  return testutils::CanCast<T>(clone.get());
}

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
