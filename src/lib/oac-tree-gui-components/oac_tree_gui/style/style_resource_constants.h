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

#ifndef OAC_TREE_GUI_STYLE_RESOURCE_CONSTANTS_H_
#define OAC_TREE_GUI_STYLE_RESOURCE_CONSTANTS_H_

//! @file
//! Collection of constants related to the parsing of json style files.

namespace oac_tree_gui::style
{

constexpr auto kDefaultLightStyleResourceName = ":/oac-tree/style/default_light_style.json";
constexpr auto kDefaultDarkStyleResourceName = ":/oac-tree/style/default_dark_style.json";

constexpr auto NodeGraphicsViewStyleKey = "NodeGraphicsViewStyle";
constexpr auto RenderBackgroundKey = "RenderBackground";
constexpr auto BackgroundColorKey = "BackgroundColor";
constexpr auto FineGridSizeKey = "FineGridSize";
constexpr auto FineGridColorKey = "FineGridColor";
constexpr auto CoarseGridFactorKey = "CoarseGridFactor";
constexpr auto CoarseGridColorKey = "CoarseGridColor";

constexpr auto NodeGraphicsSceneStyleKey = "NodeGraphicsSceneStyle";
constexpr auto ShadowEnabledKey = "ShadowEnabled";
constexpr auto BaseInstructionColorKey = "BaseInstructionColor";
constexpr auto DecoratorInstructionColorKey = "DecoratorInstructionColor";
constexpr auto CompoundInstructionColorKey = "CompoundInstructionColor";

}  // namespace oac_tree_gui::style

#endif  // OAC_TREE_GUI_STYLE_RESOURCE_CONSTANTS_H_
