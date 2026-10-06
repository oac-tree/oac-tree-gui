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

#include "oac_tree_gui/settings/settings_model_helper.h"

#include <oac_tree_gui/model/plugin_settings_item.h>
#include <oac_tree_gui/model/sequencer_settings_model.h>

#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/model/undo_settings_item.h>
#include <sup/gui/settings/in_memory_settings_storage.h>
#include <sup/gui/settings/settings_manager.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for helper functions connecting SequencerSettingsModel and the settings manager.
 */
class SettingsModelHelperTest : public ::testing::Test
{
public:
  SettingsModelHelperTest()
  {
    m_manager.SetStorage(sup::gui::SettingsScope::kApplication, &m_storage);
  }

  sup::gui::InMemorySettingsStorage m_storage;
  sup::gui::SettingsManager m_manager;
};

TEST_F(SettingsModelHelperTest, CreateSettingsModelFromEmptyStorage)
{
  auto model = CreateSettingsModel(m_manager);

  const auto undo_info = model->Get<sup::gui::UndoSettingsItem>()->GetUndoInfo();
  EXPECT_EQ(undo_info.is_enabled, sup::gui::constants::kUseUndoDefault);
  EXPECT_EQ(undo_info.undo_limit, sup::gui::constants::kUndoLimitDefault);

  EXPECT_EQ(model->Get<PluginSettingsItem>()->GetPluginSettingsInfo(),
            CreateDefaultPluginSettingsInfo());
}

TEST_F(SettingsModelHelperTest, CreateSettingsModelFromStoredSettings)
{
  const PluginSettingsInfo plugin_info{true, {"dir"}, false, {"abc"}};
  m_manager.Set(sup::gui::kUndoSettingsGroup, sup::gui::UndoInfo{false, 42},
                sup::gui::SettingsScope::kApplication);
  m_manager.Set(kPluginSettingsGroup, plugin_info, sup::gui::SettingsScope::kApplication);

  auto model = CreateSettingsModel(m_manager);

  const auto undo_info = model->Get<sup::gui::UndoSettingsItem>()->GetUndoInfo();
  EXPECT_FALSE(undo_info.is_enabled);
  EXPECT_EQ(undo_info.undo_limit, 42);

  EXPECT_EQ(model->Get<PluginSettingsItem>()->GetPluginSettingsInfo(), plugin_info);
}

TEST_F(SettingsModelHelperTest, CreateSettingsModelWithoutStorage)
{
  const sup::gui::SettingsManager manager;
  EXPECT_THROW(CreateSettingsModel(manager), sup::gui::RuntimeException);
}

TEST_F(SettingsModelHelperTest, WriteSettingsModel)
{
  const PluginSettingsInfo plugin_info{true, {"dir"}, false, {"abc"}};
  SequencerSettingsModel model;
  model.Get<sup::gui::UndoSettingsItem>()->SetUndoInfo(sup::gui::UndoInfo{false, 42});
  model.Get<PluginSettingsItem>()->SetPluginSettingsInfo(plugin_info);

  WriteSettingsModel(model, m_manager);

  const auto undo_info = m_manager.Get<sup::gui::UndoInfo>(sup::gui::kUndoSettingsGroup,
                                                           sup::gui::SettingsScope::kApplication);
  EXPECT_FALSE(undo_info.is_enabled);
  EXPECT_EQ(undo_info.undo_limit, 42);

  EXPECT_EQ(m_manager.Get<PluginSettingsInfo>(kPluginSettingsGroup,
                                              sup::gui::SettingsScope::kApplication),
            plugin_info);
}

TEST_F(SettingsModelHelperTest, WriteSettingsModelNotifiesSubscribers)
{
  ::testing::MockFunction<void(const sup::gui::UndoInfo&)> undo_callback;
  ::testing::MockFunction<void(const PluginSettingsInfo&)> plugin_callback;
  auto undo_connection = m_manager.Subscribe<sup::gui::UndoInfo>(
      sup::gui::kUndoSettingsGroup, sup::gui::SettingsScope::kApplication,
      undo_callback.AsStdFunction());
  auto plugin_connection = m_manager.Subscribe<PluginSettingsInfo>(
      kPluginSettingsGroup, sup::gui::SettingsScope::kApplication, plugin_callback.AsStdFunction());

  EXPECT_CALL(undo_callback, Call(::testing::_)).Times(1);
  EXPECT_CALL(plugin_callback, Call(::testing::_)).Times(1);

  WriteSettingsModel(SequencerSettingsModel{}, m_manager);
}

TEST_F(SettingsModelHelperTest, RoundTrip)
{
  const PluginSettingsInfo plugin_info{false, {}, true, {"abc", "def"}};
  SequencerSettingsModel model;
  model.Get<sup::gui::UndoSettingsItem>()->SetUndoInfo(sup::gui::UndoInfo{true, 7});
  model.Get<PluginSettingsItem>()->SetPluginSettingsInfo(plugin_info);

  WriteSettingsModel(model, m_manager);
  auto restored = CreateSettingsModel(m_manager);

  const auto undo_info = restored->Get<sup::gui::UndoSettingsItem>()->GetUndoInfo();
  EXPECT_TRUE(undo_info.is_enabled);
  EXPECT_EQ(undo_info.undo_limit, 7);

  EXPECT_EQ(restored->Get<PluginSettingsItem>()->GetPluginSettingsInfo(), plugin_info);
}

}  // namespace oac_tree_gui::test
