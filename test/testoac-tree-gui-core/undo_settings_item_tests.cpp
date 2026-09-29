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

#include "oac_tree_gui/model/undo_settings_item.h"

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for UndoSettingsItem class.
 */
class UndoSettingsItemTest : public ::testing::Test
{
};

TEST_F(UndoSettingsItemTest, InitialState)
{
  const UndoSettingsItem item;

  const auto info = item.GetUndoInfo();
  EXPECT_EQ(info.is_enabled, constants::kUseUndoDefault);
  EXPECT_EQ(info.undo_limit, constants::kUndoLimitDefault);
  EXPECT_TRUE(item.GetItem(constants::kUndoLimitSetting).IsEnabled());
}

TEST_F(UndoSettingsItemTest, SetUndoInfo)
{
  UndoSettingsItem item;

  item.SetUndoInfo(UndoInfo{false, 42});

  EXPECT_FALSE(item.Property<bool>(constants::kUseUndoSetting));
  EXPECT_EQ(item.Property<mvvm::int32>(constants::kUndoLimitSetting), 42);
  EXPECT_FALSE(item.GetItem(constants::kUndoLimitSetting).IsEnabled());

  const auto info = item.GetUndoInfo();
  EXPECT_FALSE(info.is_enabled);
  EXPECT_EQ(info.undo_limit, 42);
}

TEST_F(UndoSettingsItemTest, UndoFlagEnablesLimit)
{
  UndoSettingsItem item;

  item.SetProperty(constants::kUseUndoSetting, false);
  EXPECT_FALSE(item.GetItem(constants::kUndoLimitSetting).IsEnabled());

  item.SetProperty(constants::kUseUndoSetting, true);
  EXPECT_TRUE(item.GetItem(constants::kUndoLimitSetting).IsEnabled());
}

TEST_F(UndoSettingsItemTest, Clone)
{
  UndoSettingsItem item;
  item.SetUndoInfo(UndoInfo{true, 42});

  auto clone = item.Clone();
  auto cloned_item = dynamic_cast<UndoSettingsItem*>(clone.get());
  ASSERT_NE(cloned_item, nullptr);
  EXPECT_EQ(cloned_item->GetUndoInfo().undo_limit, 42);

  // strategy is active in the clone
  cloned_item->SetProperty(constants::kUseUndoSetting, false);
  EXPECT_FALSE(cloned_item->GetItem(constants::kUndoLimitSetting).IsEnabled());
}

}  // namespace oac_tree_gui::test
