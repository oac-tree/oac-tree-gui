/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "mvvm/utils/file_utils.h"

#include <mvvm/test/test_helper.h>
#include <mvvm/utils/container_utils.h>

#include <testutils/folder_test.h>

#include "oac_tree_gui/mainwindow/main_window_helper.h"

//! Testing methods from main_window_helper.h

namespace oac_tree_gui::test
{

class MainWindowHelperTest : public test::FolderTest
{
public:
  MainWindowHelperTest() : FolderTest("MainWindowHelperTest") {}
};

TEST_F(MainWindowHelperTest, GetProcedureFiles)
{
  auto file_name0 = mvvm::utils::Join(GetTestHomeDir(), "name0.xml");
  auto file_name1 = mvvm::utils::Join(GetTestHomeDir(), "name1.xml");
  auto file_name2 = mvvm::utils::Join(GetTestHomeDir(), "a.txt");

  mvvm::test::CreateTextFile(file_name0, "");
  mvvm::test::CreateTextFile(file_name1, "");
  mvvm::test::CreateTextFile(file_name2, "");

  auto found_files = GetProcedureFiles(GetTestHomeDir());

  ASSERT_EQ(found_files.size(), 2);
  EXPECT_TRUE(mvvm::utils::Contains(found_files, file_name0));
  EXPECT_TRUE(mvvm::utils::Contains(found_files, file_name1));

  found_files = GetProcedureFiles(file_name0);
  ASSERT_EQ(found_files.size(), 1);
  EXPECT_TRUE(mvvm::utils::Contains(found_files, file_name0));

  // not a procedure file
  found_files = GetProcedureFiles(file_name2);
  ASSERT_EQ(found_files.size(), 0);
}

}  // namespace oac_tree_gui::test
