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

#include "graphics_view_style.h"

#include <mvvm/style/mvvm_style_helper.h>

namespace oac_tree_gui::style
{

namespace
{

/**
 * @brief Creates default style for ConnectableShape in light desktop environment.
 */
GraphicsViewStyle CreateDefaultStyleForLightThemes()
{
  GraphicsViewStyle result;

  result.render_background = true;
  result.background_color = QColor("whitesmoke");
  result.fine_grid_size = 20;
  result.fine_grid_color = QColor("oldlace");
  result.coarse_grid_size = 200;
  result.coarse_grid_color = QColor("gainsboro");
  return result;
}

/**
 * @brief Creates default style for ConnectableShape in dark desktop environment.
 */
GraphicsViewStyle CreateDefaultStyleForDarkThemes()
{
  GraphicsViewStyle result = CreateDefaultStyleForLightThemes();
  result.background_color = QColor("darkslategrey");
  result.fine_grid_color = QColor("lightslategrey");
  result.coarse_grid_color = QColor(Qt::black);
  return result;
}

}  // namespace

GraphicsViewStyle CreateDefaulGraphicsViewStyle(mvvm::ColorFlavor color_flavor)
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

const GraphicsViewStyle &GetDefaultGraphicsViewStyle()
{
  static const GraphicsViewStyle kGraphicsViewStyle =
      CreateDefaulGraphicsViewStyle(mvvm::ColorFlavor::kUnspecified);
  return kGraphicsViewStyle;
}

}  // namespace oac_tree_gui::style
