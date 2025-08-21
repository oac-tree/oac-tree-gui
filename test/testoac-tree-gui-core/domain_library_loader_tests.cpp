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

}  // namespace oac_tree_gui::test
