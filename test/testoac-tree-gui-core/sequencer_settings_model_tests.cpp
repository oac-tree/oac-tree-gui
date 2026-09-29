/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "oac_tree_gui/model/sequencer_settings_model.h"

#include <oac_tree_gui/model/plugin_settings_item.h>
#include <oac_tree_gui/model/sequencer_settings_constants.h>
#include <oac_tree_gui/model/undo_settings_item.h>

#include <mvvm/model/item_utils.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for SequencerSettingsModel class.
 */
class SequencerSettingsModelTest : public ::testing::Test
{
};

TEST_F(SequencerSettingsModelTest, InitialState)
{
  SequencerSettingsModel model;

  EXPECT_EQ(model.GetSettingsItems().size(), 2);

  EXPECT_EQ(model.Data<bool>(constants::kUseUndoSetting), constants::kUseUndoDefault);
  EXPECT_EQ(model.Data<int>(constants::kUndoLimitSetting), constants::kUndoLimitDefault);
}

TEST_F(SequencerSettingsModelTest, Clear)
{
  SequencerSettingsModel model;

  model.GetSettingsItems().at(0)->SetProperty(constants::kUndoLimitSetting, 42);
  EXPECT_EQ(model.Data<int>(constants::kUndoLimitSetting), 42);
  model.Clear();

  // after clearing all setting items, and their values have been re-initialised
  ASSERT_EQ(model.GetSettingsItems().size(), 2);
  EXPECT_EQ(model.Data<bool>(constants::kUseUndoSetting), constants::kUseUndoDefault);
  EXPECT_EQ(model.Data<int>(constants::kUndoLimitSetting), constants::kUndoLimitDefault);
}

TEST_F(SequencerSettingsModelTest, AutoDisableSettings)
{
  SequencerSettingsModel model;

  auto common_settings = model.GetSettingsItems().at(0);
  ASSERT_EQ(model.GetSettingsItems().size(), 2);

  EXPECT_TRUE(common_settings->GetItem(constants::kUseUndoSetting).IsEnabled());
  EXPECT_TRUE(common_settings->GetItem(constants::kUndoLimitSetting).IsEnabled());

  // changing the value of the setting
  common_settings->GetItem(constants::kUseUndoSetting).SetData(false);

  EXPECT_TRUE(common_settings->GetItem(constants::kUseUndoSetting).IsEnabled());
  EXPECT_FALSE(common_settings->GetItem(constants::kUndoLimitSetting).IsEnabled());
}

TEST_F(SequencerSettingsModelTest, Clone)
{
  SequencerSettingsModel model;

  auto clone = model.Clone();
  auto cloned_model = dynamic_cast<SequencerSettingsModel*>(clone.get());
  ASSERT_NE(cloned_model, nullptr);
  EXPECT_EQ(cloned_model->GetSettingsItems().size(), 2);
  EXPECT_EQ(cloned_model->Data<bool>(constants::kUseUndoSetting), constants::kUseUndoDefault);
  EXPECT_EQ(cloned_model->Data<int>(constants::kUndoLimitSetting), constants::kUndoLimitDefault);
}

TEST_F(SequencerSettingsModelTest, GetItem)
{
  SequencerSettingsModel model;

  EXPECT_NE(model.Get<PluginSettingsItem>(), nullptr);
  EXPECT_NE(model.Get<UndoSettingsItem>(), nullptr);
}

//! Reproduces how settings dialog propagates edited settings back to the model.
TEST_F(SequencerSettingsModelTest, ReplaceRootItemWithEditedCopy)
{
  SequencerSettingsModel model;
  auto working_copy = model.Clone();
  auto working_model = dynamic_cast<SequencerSettingsModel*>(working_copy.get());
  ASSERT_NE(working_model, nullptr);
  working_model->Get<UndoSettingsItem>()->SetUndoInfo(UndoInfo{false, 42});

  model.ReplaceRootItem(mvvm::utils::CopyItem(*working_model->GetRootItem()));

  auto undo_settings = model.Get<UndoSettingsItem>();
  ASSERT_NE(undo_settings, nullptr);
  EXPECT_FALSE(undo_settings->GetUndoInfo().is_enabled);
  EXPECT_EQ(undo_settings->GetUndoInfo().undo_limit, 42);
}

}  // namespace oac_tree_gui::test
