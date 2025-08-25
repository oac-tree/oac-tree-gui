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

#include "oac_tree_gui/model/sequencer_settings_helper.h"

#include <sup/gui/core/environment.h>

#include <mvvm/core/platform.h>
#include <mvvm/test/test_helper.h>
#include <mvvm/utils/file_utils.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

#include <filesystem>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for helper functions in sequencere_settings_helper.h
 */
class SequencerSettingsHelperTest : public test::FolderTest
{
public:
  SequencerSettingsHelperTest() : FolderTest("SequencerSettingsHelperTest") {}

  /**
   * @brief Creates a symlink to a file.
   */
  void CreateSymLink(const std::string& file_name, const std::string& link_name)
  {
    std::filesystem::create_symlink(file_name, link_name);
  }
};

TEST_F(SequencerSettingsHelperTest, GetDefaultPluginDirList)
{
  if (sup::gui::IsOnCodac())
  {
    EXPECT_EQ(GetDefaultPluginDirList(),
              std::vector<std::string>({"/opt/codac/lib/oac-tree/plugins"}));
  }
  else
  {
    EXPECT_TRUE(GetDefaultPluginDirList().empty());
  }
}

TEST_F(SequencerSettingsHelperTest, FindSharedLibraries)
{
  if (!mvvm::IsLinuxHost() && !mvvm::IsMacHost())
  {
    GTEST_SKIP() << "This test is for Linux or Mac only";
  }

  const auto test_dir = CreateEmptyDir("FindSharedLibraries");

  const std::string file_name_a = mvvm::utils::Join(test_dir, "libA.so.1.2.3");
  const std::string link_name_a1 = mvvm::utils::Join(test_dir, "libA.so.1.2");
  const std::string link_name_a2 = mvvm::utils::Join(test_dir, "libA.so");
  mvvm::test::CreateTextFile(file_name_a, "file with content");
  CreateSymLink(file_name_a, link_name_a1);
  CreateSymLink(link_name_a1, link_name_a2);

  const std::string file_name_b = mvvm::utils::Join(test_dir, "libABC.so.1.2.3");
  const std::string link_name_b1 = mvvm::utils::Join(test_dir, "libABC.so.1.2");
  const std::string link_name_b2 = mvvm::utils::Join(test_dir, "libABC.so");
  mvvm::test::CreateTextFile(file_name_b, "file with content");
  CreateSymLink(file_name_b, link_name_b1);
  CreateSymLink(link_name_b1, link_name_b2);

  const std::string file_name_c = mvvm::utils::Join(test_dir, "libABC.dylib.1.2.3");
  const std::string link_name_c1 = mvvm::utils::Join(test_dir, "libABC.dylib.1.2");
  const std::string link_name_c2 = mvvm::utils::Join(test_dir, "libABC.dylib");
  mvvm::test::CreateTextFile(file_name_c, "file with content");
  CreateSymLink(file_name_c, link_name_c1);
  CreateSymLink(link_name_c1, link_name_c2);

  const std::string non_lib_file = mvvm::utils::Join(test_dir, "not_a_lib.txt");
  mvvm::test::CreateTextFile(non_lib_file, "file with content");

  EXPECT_EQ(FindSharedLibraries(test_dir), std::vector<std::string>({link_name_a2, link_name_b2, link_name_c2}));
}

}  // namespace oac_tree_gui::test
