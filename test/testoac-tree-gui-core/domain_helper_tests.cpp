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

#include <oac_tree_gui/domain/domain_helper.h>

#include <mvvm/core/platform.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests of ApplicationModels class.
 */
class DomainHelperTests : public ::testing::Test
{
};

TEST_F(DomainHelperTests, GetPluginFileName)
{
  if (mvvm::IsLinuxHost())
  {
    EXPECT_EQ(GetPluginFileName("abc"), std::string("libabc.so"));
    EXPECT_EQ(GetPluginFileName("libabc"), std::string("libabc.so"));
    EXPECT_EQ(GetPluginFileName("abc.so"), std::string("libabc.so"));
    EXPECT_EQ(GetPluginFileName("libabc.so"), std::string("libabc.so"));
    EXPECT_EQ(GetPluginFileName("abc.dylib"), std::string("libabc.so"));
    EXPECT_EQ(GetPluginFileName("libabc.dylib"), std::string("libabc.so"));
  }

  if (mvvm::IsMacHost())
  {
    EXPECT_EQ(GetPluginFileName("abc"), std::string("libabc.dylib"));
    EXPECT_EQ(GetPluginFileName("libabc"), std::string("libabc.dylib"));
    EXPECT_EQ(GetPluginFileName("abc.so"), std::string("libabc.dylib"));
    EXPECT_EQ(GetPluginFileName("libabc.so"), std::string("libabc.dylib"));
    EXPECT_EQ(GetPluginFileName("abc.dylib"), std::string("libabc.dylib"));
    EXPECT_EQ(GetPluginFileName("libabc.dylib"), std::string("libabc.dylib"));
  }
}

}  // namespace oac_tree_gui::test
