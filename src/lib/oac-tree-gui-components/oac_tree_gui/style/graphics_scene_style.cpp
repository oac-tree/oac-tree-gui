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

#include "graphics_scene_style.h"

#include <mvvm/style/mvvm_style_helper.h>

namespace oac_tree_gui::style
{

namespace
{

/**
 * @brief Creates default style for ConnectableShape in light desktop environment.
 */
GraphicsSceneStyle CreateDefaultStyleForLightThemes()
{
  GraphicsSceneStyle result;

  result.shadow_enabled = true;
  result.base_instruction_color = QColor("lightgray");
  result.decorator_instruction_color = QColor("lightseagreen");
  result.compound_instruction_color = QColor("royalblue");
  return result;
}

/**
 * @brief Creates default style for ConnectableShape in dark desktop environment.
 */
GraphicsSceneStyle CreateDefaultStyleForDarkThemes()
{
  GraphicsSceneStyle result = CreateDefaultStyleForLightThemes();
  // same as for light theme so far
  return result;
}

}  // namespace

GraphicsSceneStyle CreateDefaulGraphicsSceneStyle(mvvm::ColorFlavor color_flavor)
{
  if (color_flavor == mvvm::ColorFlavor::kForDarkThemes)
  {
    return CreateDefaultStyleForDarkThemes();
  }

  if (color_flavor == mvvm::ColorFlavor::kForLightThemes)
  {
    return CreateDefaultStyleForLightThemes();
  }

  return mvvm::style::IsDarkTheme() ? CreateDefaultStyleForDarkThemes()
                                    : CreateDefaultStyleForLightThemes();
}

const GraphicsSceneStyle &GetDefaultGraphicsSceneStyle()
{
  static const GraphicsSceneStyle kStyle = CreateDefaulGraphicsSceneStyle(mvvm::ColorFlavor::kAuto);
  return kStyle;
}

}  // namespace oac_tree_gui::style
