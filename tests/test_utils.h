/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef TESTUTILS_H
#define TESTUTILS_H

//! @file test_utils.h
//! Collection of utility functions for various unit tests.

#include <algorithm>
#include <memory>
#include <vector>

//! Various common utils for unit tests.

namespace TestUtils
{
//! Returns full path to the main test folder, as defined by CMake at compile time.
//! Shoud point to CMAKE_BINARY_DIR/test_output
std::string GetTestOutputDir();

//! Creates vector of unique_ptr of given type.
template <typename B, typename D>
auto CreateRow(int ncolumns)
{
  std::vector<std::unique_ptr<B>> result;
  for (int i = 0; i < ncolumns; ++i)
    result.emplace_back(std::make_unique<D>());
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

}  // namespace TestUtils

#endif
