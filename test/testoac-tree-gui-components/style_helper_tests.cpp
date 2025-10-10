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

#include "oac_tree_gui/style/style_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/style/style_resource_constants.h>

#include <gtest/gtest.h>

#include <QJsonObject>
#include <QString>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for functions defined in style_helper.h header.
 */
class StyleHelperTest : public ::testing::Test
{
public:
};

TEST_F(StyleHelperTest, LoadJsonFromResource)
{
  {  // non existing resource
    auto json = LoadJsonFromResource("no-such-resource");
    EXPECT_TRUE(json.empty());
    ASSERT_FALSE(json.contains(style::NodeGraphicsViewStyleKey));
  }

  {  // default dark style
    auto json = LoadJsonFromResource(style::kDefaultDarkStyleResourceName);
    ASSERT_TRUE(json.contains(style::NodeGraphicsViewStyleKey));
  }

  {  // default light style
    auto json = LoadJsonFromResource(style::kDefaultLightStyleResourceName);
    ASSERT_TRUE(json.contains(style::NodeGraphicsViewStyleKey));
  }
}

TEST_F(StyleHelperTest, ValidateStyleKey)
{
  {  // non existing group
    auto json = LoadJsonFromResource("");
    EXPECT_THROW(ValidateStyleKey(json, "no-such-group", {}), RuntimeException);
  }

  {  // existing group, but missing keys
    auto json = LoadJsonFromResource(style::kDefaultDarkStyleResourceName);
    EXPECT_THROW(ValidateStyleKey(json, style::NodeGraphicsViewStyleKey, {"no-such-key"}),
                 RuntimeException);
  }

  {  // existing group and all keys
    auto json = LoadJsonFromResource(style::kDefaultDarkStyleResourceName);
    EXPECT_NO_THROW(ValidateStyleKey(
        json, style::NodeGraphicsViewStyleKey,
        {style::BackgroundColorKey, style::FineGridColorKey, style::CoarseGridColorKey}));
  }
}

}  // namespace oac_tree_gui::test
