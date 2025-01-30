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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

#include "oac-tree-gui/model/procedure_preamble_items.h"

namespace oac_tree_gui::test
{

//! Testing ProcedureItem class.

class ProcedurePreambleItemsTest : public ::testing::Test
{
};

TEST_F(ProcedurePreambleItemsTest, TypeRegistrationInfoItem)
{
  TypeRegistrationItem item;

  EXPECT_TRUE(item.GetRegistrationString().empty());
  EXPECT_EQ(item.GetRegistrationMode(), 0);

  item.SetRegistrationMode(1);
  EXPECT_EQ(item.GetRegistrationMode(), 1);

  item.SetRegistrationString("abc");
  EXPECT_EQ(item.GetRegistrationString(), std::string("abc"));
}

TEST_F(ProcedurePreambleItemsTest, ProcedurePreambleItem)
{
  ProcedurePreambleItem item;
  EXPECT_TRUE(item.GetPluginPaths().empty());
  EXPECT_TRUE(item.GetTypeRegistrations().empty());
  EXPECT_TRUE(item.GetTypeRegistrationContainer());
  EXPECT_TRUE(item.GetPluginContainer());
}

TEST_F(ProcedurePreambleItemsTest, AddTypeRegistrationItem)
{
  ProcedurePreambleItem item;

  auto registration_item0 = item.GetTypeRegistrationContainer()->InsertItem<TypeRegistrationItem>(
      mvvm::TagIndex::Append());
  registration_item0->SetRegistrationString("abc");
  registration_item0->SetRegistrationMode(0);

  auto registration_item1 = item.GetTypeRegistrationContainer()->InsertItem<TypeRegistrationItem>(
      mvvm::TagIndex::Append());
  registration_item1->SetRegistrationString("def");
  registration_item1->SetRegistrationMode(1);

  item.AddTypeRegistration(0, "aaa");

  std::vector<std::pair<int, std::string> > expected_info = {{0, "abc"}, {1, "def"}, {0, "aaa"}};
  EXPECT_EQ(item.GetTypeRegistrations(), expected_info);
}

TEST_F(ProcedurePreambleItemsTest, AddPluginPath)
{
  ProcedurePreambleItem item;

  item.AddPluginPath("abc");
  item.AddPluginPath("def");

  const std::vector<std::string> expected{"abc", "def"};
  EXPECT_EQ(item.GetPluginPaths(), expected);
}

TEST_F(ProcedurePreambleItemsTest, SetPluginPath)
{
  ProcedurePreambleItem item;

  item.AddPluginPath("a1");

  const std::vector<std::string> expected{"a2", "a3"};

  item.SetPluginPaths(expected);

  EXPECT_EQ(item.GetPluginPaths(), expected);

  // setting again the same
  item.SetPluginPaths(expected);
  EXPECT_EQ(item.GetPluginPaths(), expected);
}

}  // namespace oac_tree_gui::test
