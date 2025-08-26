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

#include "oac_tree_gui/model/text_edit_item.h"

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for TextEditItemTest class.
 */
class TextEditItemTest : public ::testing::Test
{
};

TEST_F(TextEditItemTest, GetSettingStringFromVector)
{
  EXPECT_EQ(GetSettingStringFromVector({}), std::string());
  EXPECT_EQ(GetSettingStringFromVector({"abc"}), std::string("abc"));
  EXPECT_EQ(GetSettingStringFromVector({"abc", "def"}), std::string("abc;def"));
}

TEST_F(TextEditItemTest, GetVectorFromSettingString)
{
  EXPECT_TRUE(GetVectorFromSettingString("").empty());
  EXPECT_EQ(GetVectorFromSettingString("abc"), std::vector<std::string>({"abc"}));
  EXPECT_EQ(GetVectorFromSettingString("abc;def"), std::vector<std::string>({"abc", "def"}));
}

TEST_F(TextEditItemTest, InitialState)
{
  const TextEditItem item;

  EXPECT_TRUE(item.IsEditorEnabled());
  EXPECT_TRUE(item.GetText().empty());
}

TEST_F(TextEditItemTest, GetterAndSetter)
{
  TextEditItem item;

  EXPECT_TRUE(item.IsEditorEnabled());
  item.SetEditorEnabled(false);
  EXPECT_FALSE(item.IsEditorEnabled());
  item.SetEditorEnabled(true);
  EXPECT_TRUE(item.IsEditorEnabled());

  EXPECT_TRUE(item.GetText().empty());
  item.SetText({"line1", "line2", "line3"});
  EXPECT_EQ(item.GetText(), (std::vector<std::string>{"line1", "line2", "line3"}));
  item.SetText({});
  EXPECT_TRUE(item.GetText().empty());
}

}  // namespace oac_tree_gui::test
