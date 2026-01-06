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

#include "oac_tree_gui/style/graphics_view_style.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/style/style_helper.h>
#include <oac_tree_gui/style/style_resource_constants.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for GraphicsViewStyle.
 */
class GraphicsViewStyleTest : public ::testing::Test
{
public:
};

TEST_F(GraphicsViewStyleTest, GraphicsViewStyle)
{
  auto dark_style = style::CreateDefaulGraphicsViewStyle(mvvm::ColorFlavor::kForDarkThemes);
  auto light_style = style::CreateDefaulGraphicsViewStyle(mvvm::ColorFlavor::kForLightThemes);
  EXPECT_NE(dark_style.background_color, light_style.background_color);
  EXPECT_NE(dark_style.fine_grid_color, light_style.fine_grid_color);
  EXPECT_NE(dark_style.coarse_grid_color, light_style.coarse_grid_color);
}

}  // namespace oac_tree_gui::test
