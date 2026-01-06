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

#include "oac_tree_gui/core/factory.h"

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

class FactoryTest : public ::testing::Test
{
public:
  class TestFactory : public Factory<std::string, int>
  {
  };
};

TEST_F(FactoryTest, DomainTypeConstants)
{
  TestFactory factory;
  EXPECT_FALSE(factory.Contains("abc"));
  factory.Register("abc", 42);
  EXPECT_TRUE(factory.Contains("abc"));

  EXPECT_EQ(factory.GetValue("abc"), 42);
  EXPECT_EQ(factory.GetKeys(), std::vector<std::string>({"abc"}));
}

}  // namespace oac_tree_gui::test
