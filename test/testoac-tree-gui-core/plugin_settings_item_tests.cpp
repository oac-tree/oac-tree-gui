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

#include "oac_tree_gui/model/plugin_settings_item.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/sequencer_settings_constants.h>
#include <oac_tree_gui/model/text_edit_item.h>

#include <mvvm/test/test_helper.h>
#include <mvvm/utils/file_utils.h>

#include <sup/platform/environment.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

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

TEST_F(PluginsSettingsItemTest, CreateDefaultPluginSettingsInfo)
{
  const auto info = CreateDefaultPluginSettingsInfo();

  if (sup::platform::IsOnCodac())
  {
    EXPECT_EQ(info.dir_list, std::vector<std::string>({"/opt/codac/lib/oac-tree/plugins"}));
    EXPECT_TRUE(info.use_dir_list);
    EXPECT_FALSE(info.use_plugin_list);
  }
  else
  {
    EXPECT_TRUE(info.dir_list.empty());
    EXPECT_FALSE(info.use_dir_list);
    EXPECT_TRUE(info.use_plugin_list);
  }

  EXPECT_EQ(PluginSettingsItem().GetPluginSettingsInfo(), info);
}

TEST_F(PluginsSettingsItemTest, SetPluginSettingsInfo)
{
  PluginSettingsItem item;
  const PluginSettingsInfo info{true, {"abc", "def"}, false, {"ghi"}};

  item.SetPluginSettingsInfo(info);

  const auto& dir_list = item.GetItem<TextEditItem>(constants::kPluginDirListProperty);
  EXPECT_TRUE(dir_list.IsEditorEnabled());
  EXPECT_EQ(dir_list.GetText(), std::vector<std::string>({"abc", "def"}));

  const auto& plugin_list = item.GetItem<TextEditItem>(constants::kPluginListProperty);
  EXPECT_FALSE(plugin_list.IsEditorEnabled());
  EXPECT_EQ(plugin_list.GetText(), std::vector<std::string>({"ghi"}));

  EXPECT_EQ(item.GetPluginSettingsInfo(), info);
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

  PluginSettingsInfo info{true, {test_dir1, test_dir2}, false, {}};

  EXPECT_TRUE(mvvm::utils::HaveSameElements(
      GetPluginFileNames(info), std::vector<std::string>({file_name_a, file_name_b, file_name_c})));

  info.use_dir_list = false;
  EXPECT_TRUE(GetPluginFileNames(info).empty());
}

TEST_F(PluginsSettingsItemTest, GetPluginFileNamesForNonEmptyList)
{
  PluginSettingsInfo info{
      false,
      {},
      true,
      {domainconstants::kEpicsCAPluginName, domainconstants::kEpicsPVXSPluginName}};

  const std::vector<std::string> expected_file_names = {
      GetPluginFileName(domainconstants::kEpicsCAPluginName),
      GetPluginFileName(domainconstants::kEpicsPVXSPluginName)};
  EXPECT_EQ(GetPluginFileNames(info), expected_file_names);

  info.use_plugin_list = false;
  EXPECT_TRUE(GetPluginFileNames(info).empty());
}

TEST_F(PluginsSettingsItemTest, GetPluginFileNamesForListContainingPath)
{
  const std::vector<std::string> expected_names({"/home/user/libA.so", "/home/user/libB.so"});
  PluginSettingsInfo info{false, {}, true, expected_names};

  EXPECT_EQ(GetPluginFileNames(info), expected_names);

  info.use_plugin_list = false;
  EXPECT_TRUE(GetPluginFileNames(info).empty());
}

TEST_F(PluginsSettingsItemTest, GetPluginFileNamesForListContainingMixtureOfPluginNamesAndFileNames)
{
  PluginSettingsInfo info{false,
                          {},
                          true,
                          {domainconstants::kEpicsCAPluginName, "/home/user/libA.so",
                           domainconstants::kEpicsPVXSPluginName}};

  const std::vector<std::string> expected_file_names = {
      GetPluginFileName(domainconstants::kEpicsCAPluginName), "/home/user/libA.so",
      GetPluginFileName(domainconstants::kEpicsPVXSPluginName)};
  EXPECT_EQ(GetPluginFileNames(info), expected_file_names);

  info.use_plugin_list = false;
  EXPECT_TRUE(GetPluginFileNames(info).empty());
}

TEST_F(PluginsSettingsItemTest, GetPluginFileNamesForDirListAndFileList)
{
  const auto test_dir = CreateEmptyDir("plugin_dir2");
  const std::string file_name_a = mvvm::utils::Join(test_dir, "libA.so");
  mvvm::test::CreateTextFile(file_name_a, "file with content");

  const PluginSettingsInfo info{
      true,
      {test_dir},
      true,
      {domainconstants::kEpicsCAPluginName, domainconstants::kEpicsPVXSPluginName}};

  const std::vector<std::string> expected_file_names = {
      file_name_a, GetPluginFileName(domainconstants::kEpicsCAPluginName),
      GetPluginFileName(domainconstants::kEpicsPVXSPluginName)};
  EXPECT_TRUE(mvvm::utils::HaveSameElements(GetPluginFileNames(info), expected_file_names));
}

}  // namespace oac_tree_gui::test
