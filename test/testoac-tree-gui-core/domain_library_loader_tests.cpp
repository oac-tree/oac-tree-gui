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

#include <oac_tree_gui/domain/domain_library_loader.h>

#include <dlfcn.h>
#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for DomainLibraryLoader class.
 */
class DomainLibraryLoaderTests : public ::testing::Test
{
};

TEST_F(DomainLibraryLoaderTests, DefaultCtor)
{
  const DomainLibraryLoader loader;
  EXPECT_TRUE(loader.GetLoadedLibraries().empty());
  EXPECT_TRUE(loader.GetLibraryInfo().empty());
}

TEST_F(DomainLibraryLoaderTests, EmptyLibraryList)
{
  const std::vector<std::string> libraries;
  const DomainLibraryLoader loader(libraries);
  EXPECT_TRUE(loader.GetLoadedLibraries().empty());
  EXPECT_TRUE(loader.GetLibraryInfo().empty());
}

TEST_F(DomainLibraryLoaderTests, NonEmptyListAllMissing)
{
  const std::vector<std::string> libraries = {"lib_nonexistent1.so", "lib_nonexistent2.so"};
  const DomainLibraryLoader loader(libraries);
  EXPECT_TRUE(loader.GetLoadedLibraries().empty());
  const std::vector<std::pair<std::string, bool>> expected_info(
      {{"lib_nonexistent1.so", false}, {"lib_nonexistent2.so", false}});
  EXPECT_EQ(loader.GetLibraryInfo(), expected_info);
}

TEST_F(DomainLibraryLoaderTests, LoadsExistingDummyLibrary)
{
  const std::string dummy_lib(DUMMY_LIB_PATH);
  const std::vector<std::string> libraries = {"./lib_nonexistent1.so", dummy_lib};

  const DomainLibraryLoader loader(libraries);
  // only one should load, and it should be the dummy path
  const auto loaded = loader.GetLoadedLibraries();
  EXPECT_EQ(loaded, std::vector<std::string>({dummy_lib}));
  const std::vector<std::pair<std::string, bool>> expected_info(
      {{"./lib_nonexistent1.so", false}, {dummy_lib, true}});
  EXPECT_EQ(loader.GetLibraryInfo(), expected_info);
}

TEST_F(DomainLibraryLoaderTests, DummyLibraryExplicitUnload)
{
  const std::string path = std::string(DUMMY_LIB_PATH);

  // Baseline: is it already loaded by someone else?
  void* baseline = dlopen(path.c_str(), RTLD_LAZY | RTLD_NOLOAD);
  const bool baseline_loaded = (baseline != nullptr);
  if (baseline_loaded)
  {
    dlclose(baseline);
  }

  {
    // Load via DomainLibraryLoader
    DomainLibraryLoader loader({path});
    void* during = dlopen(path.c_str(), RTLD_LAZY | RTLD_NOLOAD);
    EXPECT_NE(during, nullptr) << "Library should be present while loader is alive";
    if (during)
    {
      dlclose(during);
    }
    loader.UnloadAll();
  }

  // should return to baseline state
  void* after = dlopen(path.c_str(), RTLD_NOLOAD);
  EXPECT_EQ(after, nullptr) << "Library should be unloaded after UnloadAll call";

  if (after)
  {
    dlclose(after);
  }
}

}  // namespace oac_tree_gui::test
