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

#include "folderbasedtest.h"

#include "test_utils.h"

#include <filesystem>

FolderBasedTest::FolderBasedTest(std::string folder_name) : m_folder_name(std::move(folder_name))
{
  std::filesystem::create_directory(GetTestDirectory());
}

//! Returns full path to the test folder. Located in CMAKE_BINARY_DIR/test_output/<m_test_dir>.

std::string FolderBasedTest::GetTestDirectory() const
{
  return TestUtils::GetTestOutputDir() + std::string("/") + m_folder_name;
}

//! Returns full path to the file with given name located in test directory.

std::string FolderBasedTest::GetFilePath(const std::string &file_name) const
{
  return GetTestDirectory() + std::string("/") + file_name;
}

FolderBasedTest::~FolderBasedTest() = default;
