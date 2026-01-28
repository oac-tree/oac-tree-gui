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

#include "oac_tree_gui/domain/domain_plugin_service.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_object_type_registry.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for DomainPluginService class.
 */
class DomainPluginServiceTests : public ::testing::Test
{
public:
  /**
   * @brief The MockLibraryLoader is a mock to test calls to DomainLibraryLoader.
   */
  class MockLibraryLoader
  {
  public:
    MOCK_METHOD(std::vector<std::string>, GetLoadedLibraries, (), (const));
    MOCK_METHOD(void, LoadLibrary, (const std::string&), ());
    MOCK_METHOD((std::vector<std::pair<std::string, bool>>), GetLibraryInfo, (), (const));
  };
};

TEST_F(DomainPluginServiceTests, InitialState)
{
  MockLibraryLoader library_loader;
  EXPECT_CALL(library_loader, LoadLibrary(testing::_)).Times(0);
  EXPECT_CALL(library_loader, GetLoadedLibraries()).Times(1);
  EXPECT_CALL(library_loader, GetLibraryInfo()).Times(1);

  const DomainPluginService<MockLibraryLoader> service(library_loader);
  EXPECT_TRUE(service.GetLoadedPlugins().empty());
  EXPECT_TRUE(service.GetPluginLoadInfo().empty());
}

TEST_F(DomainPluginServiceTests, GetLoadedPlugins)
{
  MockLibraryLoader library_loader;
  EXPECT_CALL(library_loader, LoadLibrary(testing::_)).Times(0);

  const DomainPluginService<MockLibraryLoader> service(library_loader);

  const std::vector<std::string> loaded_plugins({"plugin1", "plugin2"});
  ON_CALL(library_loader, GetLoadedLibraries()).WillByDefault(testing::Return(loaded_plugins));
  EXPECT_CALL(library_loader, GetLoadedLibraries()).Times(1);

  EXPECT_EQ(service.GetLoadedPlugins(), loaded_plugins);
}

TEST_F(DomainPluginServiceTests, GetPluginLoadInfo)
{
  MockLibraryLoader library_loader;
  EXPECT_CALL(library_loader, LoadLibrary(testing::_)).Times(0);

  const DomainPluginService<MockLibraryLoader> service(library_loader);

  const std::vector<std::pair<std::string, bool>> load_info = {{"plugin1", true},
                                                               {"plugin2", false}};
  ON_CALL(library_loader, GetLibraryInfo()).WillByDefault(testing::Return(load_info));
  EXPECT_CALL(library_loader, GetLibraryInfo()).Times(1);

  EXPECT_EQ(service.GetPluginLoadInfo(), load_info);
}

TEST_F(DomainPluginServiceTests, LoadPluginFilesEmptyList)
{
  MockLibraryLoader library_loader;
  EXPECT_CALL(library_loader, GetLoadedLibraries()).Times(1);
  EXPECT_CALL(library_loader, GetLibraryInfo()).Times(1);

  DomainPluginService<MockLibraryLoader> service(library_loader);
  EXPECT_TRUE(service.GetLoadedPlugins().empty());
  EXPECT_TRUE(service.GetPluginLoadInfo().empty());

  service.LoadPluginFiles({});
}

TEST_F(DomainPluginServiceTests, LoadNonEmptyPluginFileList)
{
  MockLibraryLoader library_loader;

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(library_loader, LoadLibrary("libplugin1.so")).Times(1);
    EXPECT_CALL(library_loader, LoadLibrary("libplugin2.so")).Times(1);
  }

  DomainPluginService<MockLibraryLoader> service(library_loader);

  EXPECT_CALL(library_loader, GetLoadedLibraries()).Times(1);
  EXPECT_CALL(library_loader, GetLibraryInfo()).Times(1);

  EXPECT_TRUE(service.GetLoadedPlugins().empty());
  EXPECT_TRUE(service.GetPluginLoadInfo().empty());

  const std::vector<std::string> plugin_file_names = {"libplugin1.so", "libplugin2.so"};
  service.LoadPluginFiles(plugin_file_names);
}

}  // namespace oac_tree_gui::test
