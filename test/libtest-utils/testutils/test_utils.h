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

#include <algorithm>
#include <chrono>
#include <functional>
#include <memory>
#include <vector>
#include <QSignalSpy>

//! Various common utils for unit tests.

namespace testutils
{
//! Returns full path to the main test folder, as defined by CMake at compile time.
//! Shoud point to CMAKE_BINARY_DIR/test_output
std::string GetTestSuiteOutputDir();

//! Returns full path to repository source directory.
std::string GetProjectSourceDir();

//! Creates vector of unique_ptr of given type.
template <typename B, typename D>
auto CreateRow(int ncolumns)
{
  std::vector<std::unique_ptr<B>> result;
  for (int i = 0; i < ncolumns; ++i)
  {
    result.emplace_back(std::make_unique<D>());
  }
  return result;
}

//! Creates vector of pointers from vector of unique_ptr.
template <typename T>
auto GetPointers(const std::vector<std::unique_ptr<T>>& vec)
{
  std::vector<T*> result;
  std::transform(vec.begin(), vec.end(), std::back_inserter(result),
                 [](auto& x) { return x.get(); });
  return result;
}

//! Create a pair of unique_ptr and raw ptr to the object of given type.
template <typename T>
auto CreateTestData()
{
  auto uptr = std::make_unique<T>();
  auto raw_ptr = uptr.get();
  return std::make_pair(std::move(uptr), raw_ptr);
}

//! Creates string representing XML document. User provided body will be enclosed between standard
//! header and footer.

std::string CreateXMLDocumentString(const std::string& body);

//! Returns text file content.
std::string GetTextFileContent(const std::string& file_name);

//! Create ASCII file with given content.
void CreateTextFile(const std::string& file_name, const std::string& content);

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

/**
 * @brief Helper function that retrieves an object from QSignalSpy arguments.
 */
template <typename T>
T GetSendItem(QSignalSpy& signal_spy)
{
  if (signal_spy.count() == 1)
  {
    auto arguments = signal_spy.takeFirst();
    if (arguments.size() == 1)
    {
      return arguments.at(0).value<T>();
    }
  }
  return {};
}

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_TEST_UTILS_H_
