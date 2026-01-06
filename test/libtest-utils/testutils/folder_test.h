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

#ifndef LIBTEST_UTILS_TESTUTILS_FOLDER_TEST_H_
#define LIBTEST_UTILS_TESTUTILS_FOLDER_TEST_H_

#include <mvvm/test/folder_output_based_test.h>

#include <testutils/cmake_info.h>

namespace oac_tree_gui::test
{

/**
 * @brief The FolderTest class is intended for unit tests that producing some output on
 * disk.
 */
class FolderTest : public mvvm::test::FolderOutputBasedTest
{
public:
  explicit FolderTest(std::string test_home_dirname);
};

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_FOLDER_TEST_H_
