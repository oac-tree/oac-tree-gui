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

#include "oac_tree_gui/model/plugin_settings_item.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>

#include <sup/gui/core/environment.h>

#include <mvvm/test/test_helper.h>
#include <mvvm/utils/file_utils.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>
#include <testutils/test_utils.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for PluginsSettingsItem class.
 */
class PluginsSettingsItemTest : public test::FolderTest
{
public:
  PluginsSettingsItemTest() : FolderTest("PluginsSettingsItemTest") {}
};

TEST_F(PluginsSettingsItemTest, PluginSettingsItemInitialState)
{
  const PluginSettingsItem item;

  EXPECT_TRUE(item.UsePluginDirList());
  EXPECT_TRUE(item.UsePluginList());

  if (sup::gui::IsOnCodac())
  {
    EXPECT_EQ(item.GetPluginDirList(),
              std::vector<std::string>({"/opt/codac/lib/oac-tree/plugins"}));
  }
  else
  {
    EXPECT_TRUE(item.GetPluginDirList().empty());
  }
}

TEST_F(PluginsSettingsItemTest, UsePluginDirListProperty)
{
  PluginSettingsItem item;

  item.SetUsePluginDirList(false);
  EXPECT_FALSE(item.UsePluginDirList());

  item.SetUsePluginDirList(true);
  EXPECT_TRUE(item.UsePluginDirList());
}

TEST_F(PluginsSettingsItemTest, PluginDirListProperty)
{
  PluginSettingsItem item;

  item.SetPluginDirList({"abc", "def"});
  EXPECT_EQ(item.GetPluginDirList(), std::vector<std::string>({"abc", "def"}));

  item.SetPluginDirList({});
  EXPECT_TRUE(item.GetPluginDirList().empty());
}

TEST_F(PluginsSettingsItemTest, UsePluginListProperty)
{
  PluginSettingsItem item;

  item.SetUsePluginList(false);
  EXPECT_FALSE(item.UsePluginList());

  item.SetUsePluginList(true);
  EXPECT_TRUE(item.UsePluginList());
}

TEST_F(PluginsSettingsItemTest, PluginListProperty)
{
  PluginSettingsItem item;

  item.SetPluginList({"abc", "def"});
  EXPECT_EQ(item.GetPluginList(), std::vector<std::string>({"abc", "def"}));

  item.SetPluginList({});
  EXPECT_TRUE(item.GetPluginList().empty());
}

TEST_F(PluginsSettingsItemTest, GetPluginFileNamesForEmptyItem)
{
  PluginSettingsItem item;
  item.SetUsePluginDirList(false);
  EXPECT_FALSE(GetPluginFileNames(item).empty());
  item.SetUsePluginDirList(true);
  EXPECT_FALSE(GetPluginFileNames(item).empty());
}

TEST_F(PluginsSettingsItemTest, GetPluginFileNamesForNonEmptyDirList)
{
  const auto test_dir1 = CreateEmptyDir("plugin_dir1");
  const std::string file_name_a = mvvm::utils::Join(test_dir1, "libA.so");
  mvvm::test::CreateTextFile(file_name_a, "file with content");
  const std::string file_name_b = mvvm::utils::Join(test_dir1, "libB.so");
  mvvm::test::CreateTextFile(file_name_b, "file with content");

  const auto test_dir2 = CreateEmptyDir("plugin_dir2");
  const std::string file_name_c = mvvm::utils::Join(test_dir2, "libC.so");
  mvvm::test::CreateTextFile(file_name_c, "file with content");

  PluginSettingsItem item;
  item.SetPluginDirList({test_dir1, test_dir2});
  item.SetUsePluginDirList(true);
  item.SetUsePluginList(false);

  EXPECT_TRUE(HaveSameElements(GetPluginFileNames(item),
                               std::vector<std::string>({file_name_a, file_name_b, file_name_c})));

  item.SetUsePluginDirList(false);
  EXPECT_TRUE(GetPluginFileNames(item).empty());
}

TEST_F(PluginsSettingsItemTest, GetPluginFileNamesForNonEmptyList)
{
  PluginSettingsItem item;
  item.SetUsePluginDirList(false);
  item.SetPluginList({domainconstants::kEpicsCAPluginName, domainconstants::kEpicsPVXSPluginName});

  item.SetUsePluginList(true);
  const std::vector<std::string> expected_file_names = {
      GetPluginFileName(domainconstants::kEpicsCAPluginName),
      GetPluginFileName(domainconstants::kEpicsPVXSPluginName)};
  EXPECT_EQ(GetPluginFileNames(item), expected_file_names);

  item.SetUsePluginList(false);
  EXPECT_TRUE(GetPluginFileNames(item).empty());
}

TEST_F(PluginsSettingsItemTest, GetPluginFileNamesForDirListAndFileList)
{
  PluginSettingsItem item;
  item.SetUsePluginList(true);
  item.SetPluginList({domainconstants::kEpicsCAPluginName, domainconstants::kEpicsPVXSPluginName});

  const auto test_dir = CreateEmptyDir("plugin_dir2");
  const std::string file_name_a = mvvm::utils::Join(test_dir, "libA.so");
  mvvm::test::CreateTextFile(file_name_a, "file with content");
  item.SetUsePluginDirList(true);
  item.SetPluginDirList({test_dir});

  const std::vector<std::string> expected_file_names = {
      file_name_a, GetPluginFileName(domainconstants::kEpicsCAPluginName),
      GetPluginFileName(domainconstants::kEpicsPVXSPluginName)};
  EXPECT_TRUE(HaveSameElements(GetPluginFileNames(item), expected_file_names));
}

}  // namespace oac_tree_gui::test
