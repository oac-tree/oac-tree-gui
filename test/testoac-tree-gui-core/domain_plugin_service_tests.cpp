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
   * @brief The MockObjectTypeRegistry is a mock to test calls to DomainObjectTypeRegistry.
   */
  class MockObjectTypeRegistry
  {
  public:
    MOCK_METHOD(void, Update, (const std::string&), ());
    MOCK_METHOD(std::vector<std::string>, GetObjectNames, (const std::string&), ());
    MOCK_METHOD(std::optional<std::string>, GetPluginName, (const std::string&), ());
  };

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
  MockObjectTypeRegistry object_registry;
  MockLibraryLoader library_loader;
  EXPECT_CALL(object_registry, Update(testing::_)).Times(0);
  EXPECT_CALL(library_loader, LoadLibrary(testing::_)).Times(0);
  EXPECT_CALL(object_registry, GetObjectNames(::testing::_)).Times(1);
  EXPECT_CALL(library_loader, GetLoadedLibraries()).Times(1);
  EXPECT_CALL(library_loader, GetLibraryInfo()).Times(1);

  const DomainPluginService<MockLibraryLoader, MockObjectTypeRegistry> service(library_loader,
                                                                               object_registry);
  EXPECT_TRUE(service.GetLoadedPlugins().empty());
  EXPECT_TRUE(service.GetPluginLoadInfo().empty());
  EXPECT_TRUE(service.GetObjectNames(domainconstants::kCorePluginName).empty());
}

TEST_F(DomainPluginServiceTests, GetLoadedPlugins)
{
  MockObjectTypeRegistry object_registry;
  MockLibraryLoader library_loader;
  EXPECT_CALL(object_registry, Update(testing::_)).Times(0);
  EXPECT_CALL(library_loader, LoadLibrary(testing::_)).Times(0);

  const DomainPluginService<MockLibraryLoader, MockObjectTypeRegistry> service(library_loader,
                                                                               object_registry);

  const std::vector<std::string> loaded_plugins({"plugin1", "plugin2"});
  ON_CALL(library_loader, GetLoadedLibraries()).WillByDefault(testing::Return(loaded_plugins));
  EXPECT_CALL(library_loader, GetLoadedLibraries()).Times(1);

  EXPECT_EQ(service.GetLoadedPlugins(), loaded_plugins);
}

TEST_F(DomainPluginServiceTests, GetPluginLoadInfo)
{
  MockObjectTypeRegistry object_registry;
  MockLibraryLoader library_loader;
  EXPECT_CALL(object_registry, Update(testing::_)).Times(0);
  EXPECT_CALL(library_loader, LoadLibrary(testing::_)).Times(0);

  const DomainPluginService<MockLibraryLoader, MockObjectTypeRegistry> service(library_loader,
                                                                               object_registry);

  const std::vector<std::pair<std::string, bool>> load_info = {{"plugin1", true},
                                                               {"plugin2", false}};
  ON_CALL(library_loader, GetLibraryInfo()).WillByDefault(testing::Return(load_info));
  EXPECT_CALL(library_loader, GetLibraryInfo()).Times(1);

  EXPECT_EQ(service.GetPluginLoadInfo(), load_info);
}

TEST_F(DomainPluginServiceTests, GetObjectNames)
{
  MockObjectTypeRegistry object_registry;
  MockLibraryLoader library_loader;
  EXPECT_CALL(object_registry, Update(testing::_)).Times(0);
  EXPECT_CALL(library_loader, LoadLibrary(testing::_)).Times(0);

  const DomainPluginService<MockLibraryLoader, MockObjectTypeRegistry> service(library_loader,
                                                                               object_registry);

  const std::string plugin_name("test_plugin");
  const std::vector<std::string> object_names({"obj1", "obj2"});
  ON_CALL(object_registry, GetObjectNames(plugin_name))
      .WillByDefault(testing::Return(object_names));
  EXPECT_CALL(object_registry, GetObjectNames(::testing::_)).Times(1);

  EXPECT_EQ(service.GetObjectNames(plugin_name), object_names);
}

TEST_F(DomainPluginServiceTests, GetPluginName)
{
  MockObjectTypeRegistry object_registry;
  MockLibraryLoader library_loader;
  EXPECT_CALL(object_registry, Update(testing::_)).Times(0);
  EXPECT_CALL(library_loader, LoadLibrary(testing::_)).Times(0);

  const DomainPluginService<MockLibraryLoader, MockObjectTypeRegistry> service(library_loader,
                                                                               object_registry);

  const std::string plugin_name("plugin_name");
  const std::string object_name("test_object");
  const std::vector<std::string> object_names({"obj1", "obj2"});
  ON_CALL(object_registry, GetPluginName(object_name)).WillByDefault(testing::Return(plugin_name));
  EXPECT_CALL(object_registry, GetPluginName(object_name)).Times(1);

  EXPECT_EQ(service.GetPluginName(object_name).value_or(""), plugin_name);
}

TEST_F(DomainPluginServiceTests, LoadPluginFilesEmptyList)
{
  MockObjectTypeRegistry object_registry;
  MockLibraryLoader library_loader;
  EXPECT_CALL(object_registry, Update(domainconstants::kCorePluginName)).Times(1);
  EXPECT_CALL(object_registry, GetObjectNames(::testing::_)).Times(1);
  EXPECT_CALL(library_loader, GetLoadedLibraries()).Times(1);
  EXPECT_CALL(library_loader, GetLibraryInfo()).Times(1);

  DomainPluginService<MockLibraryLoader, MockObjectTypeRegistry> service(library_loader,
                                                                         object_registry);
  EXPECT_TRUE(service.GetLoadedPlugins().empty());
  EXPECT_TRUE(service.GetPluginLoadInfo().empty());
  EXPECT_TRUE(service.GetObjectNames(domainconstants::kCorePluginName).empty());

  service.LoadPluginFiles({});
}

TEST_F(DomainPluginServiceTests, LoadNonEmptyPluginFileList)
{
  MockObjectTypeRegistry object_registry;
  MockLibraryLoader library_loader;

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(object_registry, Update(domainconstants::kCorePluginName)).Times(1);
    EXPECT_CALL(library_loader, LoadLibrary("libplugin1.so")).Times(1);
    EXPECT_CALL(object_registry, Update("plugin1")).Times(1);
    EXPECT_CALL(library_loader, LoadLibrary("libplugin2.so")).Times(1);
    EXPECT_CALL(object_registry, Update("plugin2")).Times(1);
  }

  DomainPluginService<MockLibraryLoader, MockObjectTypeRegistry> service(library_loader,
                                                                         object_registry);

  EXPECT_CALL(library_loader, GetLoadedLibraries()).Times(1);
  EXPECT_CALL(library_loader, GetLibraryInfo()).Times(1);
  EXPECT_CALL(object_registry, GetObjectNames(::testing::_)).Times(1);

  EXPECT_TRUE(service.GetLoadedPlugins().empty());
  EXPECT_TRUE(service.GetPluginLoadInfo().empty());
  EXPECT_TRUE(service.GetObjectNames(domainconstants::kCorePluginName).empty());

  const std::vector<std::string> plugin_file_names = {"libplugin1.so", "libplugin2.so"};
  service.LoadPluginFiles(plugin_file_names);
}

}  // namespace oac_tree_gui::test
