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

#include <oac_tree_gui/model/plugin_settings_item.h>

#include <sup/gui/core/environment.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for PluginsSettingsItem class.
 */
class PluginsSettingsItemTest : public ::testing::Test
{
};

TEST_F(PluginsSettingsItemTest, GetSettingStringFromVector)
{
  EXPECT_EQ(GetSettingStringFromVector({}), std::string());
  EXPECT_EQ(GetSettingStringFromVector({"abc"}), std::string("abc"));
  EXPECT_EQ(GetSettingStringFromVector({"abc", "def"}), std::string("abc;def"));
}

TEST_F(PluginsSettingsItemTest, GetVectorFromSettingString)
{
  EXPECT_TRUE(GetVectorFromSettingString("").empty());
  EXPECT_EQ(GetVectorFromSettingString("abc"), std::vector<std::string>({"abc"}));
  EXPECT_EQ(GetVectorFromSettingString("abc;def"), std::vector<std::string>({"abc", "def"}));
}

TEST_F(PluginsSettingsItemTest, PluginSettingsItemInitialState)
{
  const PluginSettingsItem item;

  EXPECT_TRUE(item.UsePluginDirList());

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

}  // namespace oac_tree_gui::test
