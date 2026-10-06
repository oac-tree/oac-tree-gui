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

#include "oac_tree_gui/mainwindow/sequencer_main_window_context.h"

#include <oac_tree_gui/domain/domain_library_loader.h>
#include <oac_tree_gui/settings/app_settings_storage.h>
#include <oac_tree_gui/settings/i_settings_manager.h>
#include <oac_tree_gui/settings/in_memory_settings_storage.h>

#include <sup/gui/app/app_command_service.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

class SequencerMainWindowContextTest : public ::testing::Test
{
};

TEST_F(SequencerMainWindowContextTest, InitialState)
{
  SequencerMainWindowContext context;

  // check command service
  EXPECT_NE(dynamic_cast<sup::gui::AppCommandService*>(&context.GetCommandService()), nullptr);

  // check plugin service
  auto plugin_service = dynamic_cast<DomainLibraryLoader*>(&context.GetDomainPluginService());
  ASSERT_NE(plugin_service, nullptr);
  EXPECT_EQ(plugin_service->GetPluginLoadInfo().size(), 0);

  // check settings manager
  auto& settings_manager = context.GetSettingsManager();
  EXPECT_NE(
      dynamic_cast<AppSettingsStorage*>(settings_manager.GetStorage(SettingsScope::kApplication)),
      nullptr);
  EXPECT_EQ(settings_manager.GetStorage(SettingsScope::kProject), nullptr);
}

TEST_F(SequencerMainWindowContextTest, SetProjectSettingsStorage)
{
  SequencerMainWindowContext context;
  InMemorySettingsStorage storage;

  context.SetProjectSettingsStorage(&storage);
  EXPECT_EQ(context.GetSettingsManager().GetStorage(SettingsScope::kProject), &storage);

  context.SetProjectSettingsStorage(nullptr);
  EXPECT_EQ(context.GetSettingsManager().GetStorage(SettingsScope::kProject), nullptr);
}

TEST_F(SequencerMainWindowContextTest, LoadNonExistingPlugin)
{
  SequencerMainWindowContext context;

  EXPECT_NE(dynamic_cast<sup::gui::AppCommandService*>(&context.GetCommandService()), nullptr);

  auto plugin_service = dynamic_cast<DomainLibraryLoader*>(&context.GetDomainPluginService());
  ASSERT_NE(plugin_service, nullptr);

  plugin_service->LoadPluginFiles({"libnon-existing-plugin.so"});

  const std::vector<std::pair<std::string, bool>> expected_info = {
      {"libnon-existing-plugin.so", false}};
  EXPECT_EQ(plugin_service->GetPluginLoadInfo(), expected_info);
}
}  // namespace oac_tree_gui::test
