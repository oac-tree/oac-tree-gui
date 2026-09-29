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

#include "oac_tree_gui/settings/in_memory_settings_storage.h"

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for InMemorySettingsStorage class.
 */
class InMemorySettingsStorageTest : public ::testing::Test
{
};

TEST_F(InMemorySettingsStorageTest, InitialState)
{
  const InMemorySettingsStorage storage;
  EXPECT_TRUE(storage.GetKeys().empty());
  EXPECT_FALSE(storage.Read("key").has_value());
}

TEST_F(InMemorySettingsStorageTest, WriteAndRead)
{
  InMemorySettingsStorage storage;

  storage.Write("Group/flag", mvvm::variant_t{true});
  storage.Write("Group/count", mvvm::variant_t{mvvm::int32{42}});
  storage.Write("Group/name", mvvm::variant_t{std::string("abc")});

  EXPECT_EQ(storage.Read("Group/flag"), mvvm::variant_t{true});
  EXPECT_EQ(storage.Read("Group/count"), mvvm::variant_t{mvvm::int32{42}});
  EXPECT_EQ(storage.Read("Group/name"), mvvm::variant_t{std::string("abc")});
  EXPECT_FALSE(storage.Read("Group").has_value());
  EXPECT_FALSE(storage.Read("Group/unknown").has_value());

  const std::vector<std::string> expected_keys({"Group/count", "Group/flag", "Group/name"});
  EXPECT_EQ(storage.GetKeys(), expected_keys);
}

TEST_F(InMemorySettingsStorageTest, Overwrite)
{
  InMemorySettingsStorage storage;

  storage.Write("key", mvvm::variant_t{mvvm::int32{1}});
  storage.Write("key", mvvm::variant_t{std::string("abc")});

  EXPECT_EQ(storage.Read("key"), mvvm::variant_t{std::string("abc")});
  EXPECT_EQ(storage.GetKeys().size(), 1);
}

}  // namespace oac_tree_gui::test
