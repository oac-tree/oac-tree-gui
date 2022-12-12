/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "folder_based_test.h"

#include "test_utils.h"

#include <mvvm/utils/file_utils.h>

namespace testutils
{

FolderBasedTest::FolderBasedTest(std::string test_home_dirname)
    : m_test_home_dirname(std::move(test_home_dirname))
{
  mvvm::utils::CreateDirectory(GetTestHomeDir());
}

std::string FolderBasedTest::GetTestHomeDirName() const
{
  return m_test_home_dirname;
}

//! Returns full path to the test folder. Located in
//! CMAKE_BINARY_DIR/test_output/<m_test_home_dirname>.

std::string FolderBasedTest::GetTestHomeDir() const
{
  return testutils::GetTestSuiteOutputDir() + std::string("/") + GetTestHomeDirName();
}

//! Returns full path to the file with given name located in test home directory.

std::string FolderBasedTest::GetFilePath(const std::string &file_name) const
{
  return GetTestHomeDir() + std::string("/") + file_name;
}

//! Creates an empty directory in main test folder.
//! Remove recursively previous one with the same name, if exist.

std::string FolderBasedTest::CreateEmptyDir(const std::string &subdir) const
{
  auto path = mvvm::utils::Join(GetTestHomeDir(), subdir);
  mvvm::utils::RemoveAll(path);
  mvvm::utils::CreateDirectory(path);
  return path;
}

}  // namespace testutils
