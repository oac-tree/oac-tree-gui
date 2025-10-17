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

#ifndef OAC_TREE_GUI_GRAPHICS_SCENE_STYLE_H_
#define OAC_TREE_GUI_GRAPHICS_SCENE_STYLE_H_

//! @file
//! Collection of style related types. They will be used to initialize corresponding widgets and
//! shapes. Initial values will be obtained from json.

#include <mvvm/style/color_flavor.h>

#include <QColor>

namespace oac_tree_gui::style
{

/**
 * @brief The GraphicsSceneStyle struct contains all parameters to configure graphics scene.
 */
struct GraphicsSceneStyle
{
  bool shadow_enabled{false};
  QColor base_instruction_color;
  QColor decorator_instruction_color;
  QColor compound_instruction_color;
};

/**
 * @brief Creates default graphics view style for the given color flavor.
 */
GraphicsSceneStyle CreateDefaulGraphicsSceneStyle(
    mvvm::ColorFlavor color_flavor = mvvm::ColorFlavor::kUnspecified);

/**
 * @brief Returns default graphics view style.
 */
const GraphicsSceneStyle& GetDefaultGraphicsSceneStyle();

}  // namespace oac_tree_gui::style

#endif  // OAC_TREE_GUI_GRAPHICS_SCENE_STYLE_H_
