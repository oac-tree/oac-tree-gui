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

#include "oac_tree_gui/style/graphics_scene_style.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/style/style_helper.h>
#include <oac_tree_gui/style/style_resource_constants.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for GraphicsSceneStyle.
 */
class GraphicsSceneStyleTest : public ::testing::Test
{
public:
};

TEST_F(GraphicsSceneStyleTest, GraphicsSceneStyle)
{
  auto dark_style = style::CreateDefaulGraphicsSceneStyle(mvvm::ColorFlavor::kForDarkThemes);
  auto light_style = style::CreateDefaulGraphicsSceneStyle(mvvm::ColorFlavor::kForLightThemes);
  EXPECT_EQ(dark_style.base_instruction_color, light_style.base_instruction_color);
}

}  // namespace oac_tree_gui::test
