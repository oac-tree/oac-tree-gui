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

#include <sup/gui/model/undo_settings_item.h>

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
  const SequencerSettingsModel model;

  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 2);

  ASSERT_NE(model.Get<sup::gui::UndoSettingsItem>(), nullptr);
  const auto undo_info = model.Get<sup::gui::UndoSettingsItem>()->GetUndoInfo();
  EXPECT_EQ(undo_info.is_enabled, sup::gui::constants::kUseUndoDefault);
  EXPECT_EQ(undo_info.undo_limit, sup::gui::constants::kUndoLimitDefault);

  ASSERT_NE(model.Get<PluginSettingsItem>(), nullptr);
  EXPECT_EQ(model.Get<PluginSettingsItem>()->GetPluginSettingsInfo(),
            CreateDefaultPluginSettingsInfo());
}

TEST_F(SequencerSettingsModelTest, Clear)
{
  SequencerSettingsModel model;

  model.Get<sup::gui::UndoSettingsItem>()->SetUndoInfo(sup::gui::UndoInfo{false, 42});
  model.Clear();

  // after clearing all setting items, and their values have been re-initialised
  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 2);
  const auto undo_info = model.Get<sup::gui::UndoSettingsItem>()->GetUndoInfo();
  EXPECT_EQ(undo_info.is_enabled, sup::gui::constants::kUseUndoDefault);
  EXPECT_EQ(undo_info.undo_limit, sup::gui::constants::kUndoLimitDefault);
}

TEST_F(SequencerSettingsModelTest, Clone)
{
  SequencerSettingsModel model;
  model.Get<sup::gui::UndoSettingsItem>()->SetUndoInfo(sup::gui::UndoInfo{false, 42});

  auto clone = model.Clone();
  auto cloned_model = dynamic_cast<SequencerSettingsModel*>(clone.get());
  ASSERT_NE(cloned_model, nullptr);
  EXPECT_EQ(cloned_model->GetRootItem()->GetTotalItemCount(), 2);
  EXPECT_EQ(cloned_model->Get<sup::gui::UndoSettingsItem>()->GetUndoInfo().undo_limit, 42);
  EXPECT_NE(cloned_model->Get<PluginSettingsItem>(), nullptr);
}

//! Reproduces how settings dialog propagates edited settings back to the model.
TEST_F(SequencerSettingsModelTest, ReplaceRootItemWithEditedCopy)
{
  SequencerSettingsModel model;
  auto working_copy = model.Clone();
  auto working_model = dynamic_cast<SequencerSettingsModel*>(working_copy.get());
  ASSERT_NE(working_model, nullptr);
  working_model->Get<sup::gui::UndoSettingsItem>()->SetUndoInfo(sup::gui::UndoInfo{false, 42});

  model.ReplaceRootItem(mvvm::utils::CopyItem(*working_model->GetRootItem()));

  auto undo_settings = model.Get<sup::gui::UndoSettingsItem>();
  ASSERT_NE(undo_settings, nullptr);
  EXPECT_FALSE(undo_settings->GetUndoInfo().is_enabled);
  EXPECT_EQ(undo_settings->GetUndoInfo().undo_limit, 42);
}

}  // namespace oac_tree_gui::test
