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

#include "oac_tree_gui/style/style_types.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/style/style_helper.h>
#include <oac_tree_gui/style/style_resource_constants.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for
 */
class StyleTypesTest : public ::testing::Test
{
public:
};

TEST_F(StyleTypesTest, GraphicsViewStyle)
{
  {  // non existing style

    auto json = LoadJsonFromResource("non-existing-style");
    EXPECT_THROW(style::GraphicsViewStyle::CreateFromStyle(json), RuntimeException);
  }

  {  // dark and white styles
    auto json = LoadJsonFromResource(style::kDefaultDarkStyleResourceName);
    auto dark_style = style::GraphicsViewStyle::CreateFromStyle(json);

    json = LoadJsonFromResource(style::kDefaultLightStyleResourceName);
    auto light_style = style::GraphicsViewStyle::CreateFromStyle(json);

    EXPECT_NE(dark_style.background_color, light_style.background_color);
    EXPECT_NE(dark_style.fine_grid_color, light_style.fine_grid_color);
    EXPECT_NE(dark_style.corase_grid_color, light_style.corase_grid_color);
  }
}

}  // namespace oac_tree_gui::test
