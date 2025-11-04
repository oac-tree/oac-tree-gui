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

#include "oac_tree_gui/style/json_style_factory.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/style/graphics_scene_style.h>
#include <oac_tree_gui/style/graphics_view_style.h>
#include <oac_tree_gui/style/style_helper.h>
#include <oac_tree_gui/style/style_resource_constants.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for helper methods in json_style_factory.h
 */
class JsonStyleFactoryTest : public ::testing::Test
{
public:
};

TEST_F(JsonStyleFactoryTest, PopulateGraphicsViewStyle)
{
  auto dark_json_style = LoadDefaultJsonStyle(mvvm::ColorFlavor::kForDarkThemes);
  style::GraphicsViewStyle dark_style;
  style::PopulateStyleFromJSON(dark_json_style, dark_style);

  auto light_json_style = LoadDefaultJsonStyle(mvvm::ColorFlavor::kForLightThemes);
  style::GraphicsViewStyle light_style;
  style::PopulateStyleFromJSON(light_json_style, light_style);

  EXPECT_NE(dark_style.background_color, light_style.background_color);
  EXPECT_NE(dark_style.fine_grid_color, light_style.fine_grid_color);
  EXPECT_NE(dark_style.coarse_grid_color, light_style.coarse_grid_color);
}

TEST_F(JsonStyleFactoryTest, PopulateGraphicsSceneStyle)
{
  auto dark_json_style = LoadDefaultJsonStyle(mvvm::ColorFlavor::kForDarkThemes);
  style::GraphicsSceneStyle dark_style;
  style::PopulateStyleFromJSON(dark_json_style, dark_style);

  auto light_json_style = LoadDefaultJsonStyle(mvvm::ColorFlavor::kForLightThemes);
  style::GraphicsSceneStyle light_style;
  style::PopulateStyleFromJSON(light_json_style, light_style);

  EXPECT_NE(dark_style.base_instruction_color, light_style.base_instruction_color);
}

}  // namespace oac_tree_gui::test
