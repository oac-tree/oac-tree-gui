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

#include "oac_tree_gui/mainwindow/sequencer_main_window_context.h"

#include <oac_tree_gui/domain/domain_library_loader.h>
#include <oac_tree_gui/domain/domain_object_type_registry.h>
#include <oac_tree_gui/domain/domain_plugin_service.h>
#include <oac_tree_gui/model/sequencer_settings_model.h>

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

  // check type registry
  EXPECT_TRUE(
      context.GetObjectTypeRegistry().GetObjectNames(domainconstants::kCorePluginName).empty());

  // check command service
  EXPECT_NE(dynamic_cast<sup::gui::AppCommandService*>(&context.GetCommandService()), nullptr);

  // check plugin service
  auto plugin_service =
      dynamic_cast<DomainPluginService<DomainLibraryLoader, DomainObjectTypeRegistry>*>(
          &context.GetDomainPluginService());
  ASSERT_NE(plugin_service, nullptr);
  EXPECT_EQ(plugin_service->GetLoadedPlugins().size(), 0);
  EXPECT_EQ(plugin_service->GetPluginLoadInfo().size(), 0);
  EXPECT_TRUE(plugin_service->GetObjectNames(domainconstants::kCorePluginName).empty());

  // check settings
  EXPECT_NE(dynamic_cast<SequencerSettingsModel*>(&context.GetSettingsModel()), nullptr);
}

TEST_F(SequencerMainWindowContextTest, LoadNonExistingPlugin)
{
  SequencerMainWindowContext context;

  EXPECT_NE(dynamic_cast<sup::gui::AppCommandService*>(&context.GetCommandService()), nullptr);

  auto plugin_service =
      dynamic_cast<DomainPluginService<DomainLibraryLoader, DomainObjectTypeRegistry>*>(
          &context.GetDomainPluginService());
  ASSERT_NE(plugin_service, nullptr);

  plugin_service->LoadPluginFiles({"libnon-existing-plugin.so"});

  EXPECT_EQ(plugin_service->GetLoadedPlugins().size(), 0);
  const std::vector<std::pair<std::string, bool>> expected_info = {
      {"libnon-existing-plugin.so", false}};
  EXPECT_EQ(plugin_service->GetPluginLoadInfo(), expected_info);

  // check that core instruction and variables are there
  auto object_names = plugin_service->GetObjectNames(domainconstants::kCorePluginName);
  EXPECT_NE(std::find(object_names.begin(), object_names.end(),
                      domainconstants::kSequenceInstructionType),
            object_names.end());
  EXPECT_NE(
      std::find(object_names.begin(), object_names.end(), domainconstants::kWaitInstructionType),
      object_names.end());
}

TEST_F(SequencerMainWindowContextTest, FindSequencerMainWindowContext)
{
  EXPECT_EQ(FindSequencerMainWindowContext(), nullptr);
}

}  // namespace oac_tree_gui::test
