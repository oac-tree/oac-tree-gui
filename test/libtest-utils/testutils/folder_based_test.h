/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_FOLDER_BASED_TEST_H_
#define LIBTEST_UTILS_TESTUTILS_FOLDER_BASED_TEST_H_

#include <mvvm/test/folder_output_based_test.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

namespace testutils
{

class FolderBasedTest : public mvvm::test::FolderOutputBasedTest
{
public:
  explicit FolderBasedTest(std::string test_home_dirname)
      : FolderOutputBasedTest(GetTestSuiteOutputDir(), std::move(test_home_dirname))
  {
  }
};

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_FOLDER_BASED_TEST_H_
