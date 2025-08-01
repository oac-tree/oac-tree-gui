/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "oac_tree_gui/model/sequencer_settings_model.h"

#include <sup/gui/model/settings_constants.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

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
  const SequencerSettingsModel model;

  EXPECT_EQ(model.GetSettingsItems().size(), 1);

  EXPECT_EQ(model.Data<bool>(sup::gui::constants::kUseUndoSetting),
            sup::gui::constants::kUseUndoDefault);
  EXPECT_EQ(model.Data<int>(sup::gui::constants::kUndoLimitSetting),
            sup::gui::constants::kUndoLimitDefault);
}

TEST_F(SequencerSettingsModelTest, Clear)
{
  SequencerSettingsModel model;

  model.GetSettingsItems().at(0)->SetProperty(sup::gui::constants::kUndoLimitSetting, 42);
  EXPECT_EQ(model.Data<int>(sup::gui::constants::kUndoLimitSetting), 42);
  model.Clear();

  // after clearing all setting items, and their values have been re-initialised
  ASSERT_EQ(model.GetSettingsItems().size(), 1);
  EXPECT_EQ(model.Data<bool>(sup::gui::constants::kUseUndoSetting),
            sup::gui::constants::kUseUndoDefault);
  EXPECT_EQ(model.Data<int>(sup::gui::constants::kUndoLimitSetting),
            sup::gui::constants::kUndoLimitDefault);
}

TEST_F(SequencerSettingsModelTest, AutoDisableSettings)
{
  const SequencerSettingsModel model;

  auto common_settings = model.GetSettingsItems().at(0);
  ASSERT_EQ(model.GetSettingsItems().size(), 1);

  EXPECT_TRUE(common_settings->GetItem(sup::gui::constants::kUseUndoSetting)->IsEnabled());
  EXPECT_TRUE(common_settings->GetItem(sup::gui::constants::kUndoLimitSetting)->IsEnabled());

  // changing the value of the setting
  common_settings->GetItem(sup::gui::constants::kUseUndoSetting)->SetData(false);

  EXPECT_TRUE(common_settings->GetItem(sup::gui::constants::kUseUndoSetting)->IsEnabled());
  EXPECT_FALSE(common_settings->GetItem(sup::gui::constants::kUndoLimitSetting)->IsEnabled());
}

TEST_F(SequencerSettingsModelTest, Clone)
{
  const SequencerSettingsModel model;

  auto clone = model.Clone();
  auto cloned_model = dynamic_cast<SequencerSettingsModel*>(clone.get());
  ASSERT_NE(cloned_model, nullptr);
  EXPECT_EQ(cloned_model->GetSettingsItems().size(), 1);
  EXPECT_EQ(cloned_model->Data<bool>(sup::gui::constants::kUseUndoSetting),
            sup::gui::constants::kUseUndoDefault);
  EXPECT_EQ(cloned_model->Data<int>(sup::gui::constants::kUndoLimitSetting),
            sup::gui::constants::kUndoLimitDefault);
}

}  // namespace oac_tree_gui::test
