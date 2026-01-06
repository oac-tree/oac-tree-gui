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

#include "oac_tree_gui/domain/domain_object_type_registry.h"

#include <oac_tree_gui/core/exceptions.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests of DomainObjectTypeRegistry class.
 */
class DomainObjectTypeRegistryTest : public ::testing::Test
{
};

TEST_F(DomainObjectTypeRegistryTest, UpdateViaObjectList)
{
  const std::string kUndefined("undefined");
  DomainObjectTypeRegistry registry;

  registry.Update("plugin1", {"a1", "a2"});
  EXPECT_EQ(registry.GetPluginName("a1").value_or(kUndefined), "plugin1");
  EXPECT_EQ(registry.GetPluginName("a2").value_or(kUndefined), "plugin1");
  EXPECT_EQ(registry.GetPluginName("not-registered-object").value_or(kUndefined), kUndefined);

  // registerging extended list of objects for another plugin name
  registry.Update("plugin2", {"a1", "a2", "a3"});

  // two previous objects ket their plugin name
  EXPECT_EQ(registry.GetPluginName("a1").value_or(kUndefined), "plugin1");
  EXPECT_EQ(registry.GetPluginName("a2").value_or(kUndefined), "plugin1");

  // only last object got new plugin name
  EXPECT_EQ(registry.GetPluginName("a3").value_or(kUndefined), "plugin2");

  // empty plugin names are allowed
  registry.Update("", {"a4"});
  EXPECT_EQ(registry.GetPluginName("a4").value_or(kUndefined), "");
}

TEST_F(DomainObjectTypeRegistryTest, UpdateViaCallback)
{
  const std::string kUndefined("undefined");

  auto callback = []() -> std::vector<std::string> { return {"a1", "a2"}; };

  DomainObjectTypeRegistry registry(callback);

  registry.Update("plugin1");
  EXPECT_EQ(registry.GetPluginName("a1").value_or(kUndefined), "plugin1");
  EXPECT_EQ(registry.GetPluginName("a2").value_or(kUndefined), "plugin1");
  EXPECT_EQ(registry.GetPluginName("not-registered-object").value_or(kUndefined), kUndefined);
}

TEST_F(DomainObjectTypeRegistryTest, UpdateViaCallbackFailure)
{
  DomainObjectTypeRegistry registry;  // callback is not defined

  EXPECT_THROW(registry.Update("plugin1"), RuntimeException);
}

TEST_F(DomainObjectTypeRegistryTest, GetObjectNames)
{
  DomainObjectTypeRegistry registry;

  registry.Update("", {"a1", "a2"});
  registry.Update("plugin1", {"a4", "a1", "a2", "a3"});
  registry.Update("plugin2", {"a5"});

  EXPECT_EQ(registry.GetObjectNames(""), std::vector<std::string>({"a1", "a2"}));
  EXPECT_EQ(registry.GetObjectNames("plugin1"), std::vector<std::string>({"a3", "a4"}));
  EXPECT_EQ(registry.GetObjectNames("plugin2"), std::vector<std::string>({"a5"}));
  EXPECT_TRUE(registry.GetObjectNames("non-existing").empty());
}

}  // namespace oac_tree_gui::test
