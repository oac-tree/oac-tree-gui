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

#include <oac_tree_gui/domain/domain_library_loader.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

TEST(DomainLibraryLoaderTests, EmptyLibraryList)
{
  // Arrange
  std::vector<std::string> libraries;  // empty

  // Act
  DomainLibraryLoader loader(libraries);

  // Assert
  EXPECT_TRUE(loader.GetLoadedLibraries().empty());
}

TEST(DomainLibraryLoaderTests, NonEmptyList_AllMissing)
{
  // Arrange: choose file names that should not exist on the system
  const std::vector<std::string> libraries = {
      "lib__nonexistent_1_9be2c2e7d2__.so",
      "/tmp/lib__nonexistent_2_c1a4deadbeef__.so",
      "./lib__surely_missing__4f3a2c1b.so"};

  // Act
  DomainLibraryLoader loader(libraries);

  // Assert: none should load
  EXPECT_TRUE(loader.GetLoadedLibraries().empty());
}

TEST(DomainLibraryLoaderTests, LoadsExistingDummyLibrary)
{
#ifdef DUMMY_LIB_PATH
  // Arrange: include both a bogus path and the actual dummy lib path
  const std::vector<std::string> libraries = {
      "./lib__definitely_missing__abc123.so",
      std::string(DUMMY_LIB_PATH)};

  // Act
  DomainLibraryLoader loader(libraries);

  // Assert: only one should load, and it should be the dummy path
  const auto& loaded = loader.GetLoadedLibraries();
  ASSERT_EQ(loaded.size(), 1u);
  EXPECT_EQ(loaded[0], std::string(DUMMY_LIB_PATH));
#else
  GTEST_SKIP() << "DUMMY_LIB_PATH is not defined";
#endif
}

}  // namespace oac_tree_gui::test
